#include "routines.hpp"

using namespace dive;

auto cartesian = CreateBasisCartesian(1);

Scalar pi = utils::math::Pi<Scalar>();
Scalar temperature = 273.15;
Scalar pressure = 101325.1;

auto state = CreateState(temperature, pressure);
auto fluidMaterial = CreateOilVG100(1, temperature, pressure);
auto density = fluidMaterial->GetDensity(state);
auto dynamicViscosity = fluidMaterial->GetDynamicViscosity(state);
auto kinematicViscosity = dynamicViscosity / density;

auto velocityWall0 = CreateValueScalarCoordinates(3, 0.0);
auto velocityWall1 = CreateValueScalarCoordinates(3, 0.0);
auto velocityWall2 = CreateValueScalarCoordinates(3, 0.0);

auto pressureInlet = CreateValueScalarCoordinates(3, pressure);

Values loadCase =
{
    {"fluid:dirichlet:0:top", velocityWall0},
    {"fluid:dirichlet:1:top", velocityWall1},
    {"fluid:dirichlet:2:top", velocityWall2},

    {"fluid:dirichlet:0:bottom", velocityWall0},
    {"fluid:dirichlet:1:bottom", velocityWall0},
    {"fluid:dirichlet:2:bottom", velocityWall0},

    {"pressure:dirichlet:0:inlet", pressureInlet},
};

int main()
{
    try
    {
        InitializeGmsh("./bearing_flat_hydrostatic.msh");

        auto fluidMaterial = material::CreateOilVG100(1, temperature, pressure);
        auto fluidProblem = CreateProblemFluid(1, "fluid", loadCase);
        auto thermalProblem = CreateProblemThermal(2, "thermal", loadCase);
        auto pressureProblem = CreateProblemPressure(3, "pressure", loadCase);

        auto probeFluidInlet = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "fluid:probe:inlet");
        auto probeFluidOutlet = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "fluid:probe:outlet");

        auto probePressureTop = mesh::GmshGetNodesForPhysicalGroup(pressureProblem->GetMesh(), "pressure:probe:top");
        auto probePressureBottom = mesh::GmshGetNodesForPhysicalGroup(pressureProblem->GetMesh(), "pressure:probe:bottom");

        thermalProblem->SetPressure(std::static_pointer_cast<IScalarCoordinates>(pressureProblem->GetPressure()));
        pressureProblem->SetTemperature(std::static_pointer_cast<IScalarCoordinates>(thermalProblem->GetTemperature()));
        fluidProblem->SetPressure(std::static_pointer_cast<IScalarCoordinates>(pressureProblem->GetPressure()));
        fluidProblem->SetTemperature(std::static_pointer_cast<IScalarCoordinates>(thermalProblem->GetTemperature()));

        mesh::ApplyMaterial(fluidProblem->GetMesh(), fluidMaterial);
        mesh::ApplyMaterial(thermalProblem->GetMesh(), fluidMaterial);
        mesh::ApplyMaterial(pressureProblem->GetMesh(), fluidMaterial);
        mesh::ApplyValue(thermalProblem->GetMesh(), CreateValueMatrixCoordinates(3, { {temperature} }));

        FinalizeGmsh();
               
        //-------------------------------------------------------------------------------------------------

        auto sizeElement = mesh::GetSizeMinimum(fluidProblem->GetMesh()->GetElements());

        auto dt1 = (sizeElement * sizeElement) / kinematicViscosity;
        //auto dt2 = sizeElement / (speedSliding);
        //auto dt = 0.2 * dt1 * dt2 / (dt1 + dt2);
        auto dt = 0.1 * dt1;

        //std::cout << utils::string::Format("Size element = {:.3e} | Speed fluid = {:.3e} | Time step = {:.3e}", sizeElement, speedSliding, dt) << std::endl;

        //-------------------------------------------------------------------------------------------------

        fluidProblem->Initialize();
        thermalProblem->Initialize();
        pressureProblem->Initialize();

        auto totalDofMomentum = fluidProblem->GetTotalDof();
        auto pivotMomentum = fluidProblem->GetPivot();

        auto totalDofPressure = pressureProblem->GetTotalDof();
        auto pivotPressure = pressureProblem->GetPivot();

        std::cout << utils::string::Format("Total dof momentum = {} | Pivot momentum = {}", totalDofMomentum, pivotMomentum) << std::endl;
        std::cout << utils::string::Format("Total dof pressure = {} | Pivot pressure = {}", totalDofPressure, pivotPressure) << std::endl;

        auto M = problem::Partition<eilig::Sparses>(fluidProblem->Mass(false), totalDofMomentum, pivotMomentum);
        auto K = problem::Partition<eilig::Sparses>(fluidProblem->Stiffness(), totalDofMomentum, pivotMomentum);

        auto Mp = problem::Partition<eilig::Sparses>(pressureProblem->Mass(fluidProblem, false), totalDofPressure, pivotPressure);
        auto H = problem::Partition<eilig::Sparses>(pressureProblem->Stiffness(), totalDofPressure, pivotPressure);
        auto G = pressureProblem->Crossed(fluidProblem);
        auto G_Transpose = G.Transpose();

        Index step{ 1 };
        Indices steps;
        Scalars norms;
        
        while (true)
        {
            auto q0 = problem::Partition<eilig::Vectors>(fluidProblem->Momentum(), totalDofMomentum, pivotMomentum);
            auto q1 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto q2 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto dq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto dqq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto gp = problem::Partition<eilig::Vectors>(G_Transpose * pressureProblem->Pressure(), totalDofMomentum, pivotMomentum);

            eilig::IterativeBiCGStab(M[3], dq[1], -dt * (K[2] * q0[0] + K[3] * q0[1] + gp[1]), callbackIterativeMomentum);

            q1[0] = q0[0] + dq[0];
            q1[1] = q0[1] + dq[1];

            fluidProblem->UpdateMeshValuesMomentum(q1[0], q1[1]);

            auto gu = problem::Partition<eilig::Vectors>(G * fluidProblem->Momentum(), totalDofPressure, pivotPressure);
            auto p0 = problem::Partition<eilig::Vectors>(pressureProblem->Pressure(), totalDofPressure, pivotPressure);
            auto p1 = problem::Partition<eilig::Vectors>(Vector(totalDofPressure, 0.0), totalDofPressure, pivotPressure);
            auto dp = problem::Partition<eilig::Vectors>(Vector(totalDofPressure, 0.0), totalDofPressure, pivotPressure);
            eilig::IterativeBiCGStab(Mp[3], dp[1], dt * (gu[1]), callbackIterativePressure);

            p1[0] = p0[0] + dp[0];
            p1[1] = p0[1] + dp[1];

            pressureProblem->UpdateMeshValues(p1[0], p1[1]);

            auto numerator = eilig::NormP2(q1[1] - q0[1]);
            auto denominator = eilig::NormP2(q0[1]);
            denominator = (denominator < 1e-10) ? 1.0 : denominator;
            auto norm = numerator / denominator;

            steps.push_back(step);
            norms.push_back(norm);

            std::cout << utils::string::Format("Step = {} | Norm = {:.3e}", step, norm) << std::endl;

            if (norm < 1.0e-2)
            {
                break;
            }

            step += 1;
        }

        SaveToFile("bearing_flat_hydrostatic_fluid_inlet.dat", probeFluidInlet);
        SaveToFile("bearing_flat_hydrostatic_fluid_outlet.dat", probeFluidOutlet);

        SaveToFile("bearing_flat_hydrostatic_pressure_top.dat", probePressureTop);
        SaveToFile("bearing_flat_hydrostatic_pressure_bottom.dat", probePressureBottom);
    }
    catch (const std::exception& error)
    {
        logger::Error("DIVE", error.what());
    }

    return 0;
}