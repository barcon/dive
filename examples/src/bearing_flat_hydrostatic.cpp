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

auto momentumWall0 = CreateValueScalarCoordinates(3, 0.0);
auto momentumWall1 = CreateValueScalarCoordinates(3, 0.0);
auto momentumWall2 = CreateValueScalarCoordinates(3, 0.0);

auto pressureInlet = CreateValueScalarCoordinates(3, pressure);
auto pressureOutlet = CreateValueScalarCoordinates(3, 0.0);

int main()
{
    try
    {
        InitializeGmsh("./bearing_flat_hydrostatic.msh");

        auto fluidProblem = CreateProblemFluid(1, "fluid");
        auto thermalProblem = CreateProblemThermal(2, "thermal");       
        auto pressureProblem = CreateProblemPressure(3, "pressure");
        
        ApplyLoad(fluidProblem, "fluid:dirichlet:0:top", momentumWall0);
        ApplyLoad(fluidProblem, "fluid:dirichlet:1:top", momentumWall1);
        ApplyLoad(fluidProblem, "fluid:dirichlet:2:top", momentumWall2);
        
        ApplyLoad(fluidProblem, "fluid:dirichlet:0:bottom", momentumWall0);
        ApplyLoad(fluidProblem, "fluid:dirichlet:1:bottom", momentumWall1);
        ApplyLoad(fluidProblem, "fluid:dirichlet:2:bottom", momentumWall2);
        
        ApplyLoad(pressureProblem, "pressure:dirichlet:0:inlet", pressureInlet);
        ApplyLoad(pressureProblem, "pressure:dirichlet:0:outlet", pressureOutlet);
        ApplyLoad(pressureProblem, "pressure:load:volume:divergence", CreateValueVectorCoordinatesCongruent(fluidProblem->GetMesh()));
        ApplyLoad(pressureProblem, "pressure:load:volume:gradient", CreateValueVectorCoordinatesCongruent(fluidProblem->GetMesh()));
        
        auto probeFluid = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "fluid:domain");
        auto probeFluidInlet = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "fluid:probe:inlet");
        auto probeFluidOutlet = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "fluid:probe:outlet");

        auto probePressure = mesh::GmshGetNodesForPhysicalGroup(pressureProblem->GetMesh(), "pressure:domain");
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
        auto dt = 0.2 * dt1;

        logger::Info("EXAMPLES", utils::string::Format("Size element = {:.3e}", sizeElement));
        logger::Info("EXAMPLES", utils::string::Format("Time step = {:.3e}", dt));

        //-------------------------------------------------------------------------------------------------

        fluidProblem->Initialize();
        thermalProblem->Initialize();
        pressureProblem->Initialize();

        auto totalDofMomentum = fluidProblem->GetTotalDof();
        auto pivotMomentum = fluidProblem->GetPivot();

        auto totalDofPressure = pressureProblem->GetTotalDof();
        auto pivotPressure = pressureProblem->GetPivot();

		logger::Info("EXAMPLES", utils::string::Format("Total dof momentum = {} | Pivot momentum = {}", totalDofMomentum, pivotMomentum));
		logger::Info("EXAMPLES", utils::string::Format("Total dof pressure = {} | Pivot pressure = {}", totalDofPressure, pivotPressure));

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
            auto v0 = problem::Partition<eilig::Vectors>(fluidProblem->Velocity(), totalDofMomentum, pivotMomentum);
            auto q1 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto q2 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto dq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto dqq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);            
            eilig::IterativeBiCGStab(M[3], dq[1], - dt * (K[2] * v0[0] + K[3] * v0[1]), callbackIterativeMomentum);

            q1[0] = q0[0] + dq[0];
            q1[1] = q0[1] + dq[1];

            fluidProblem->UpdateMeshValues(q1[0], q1[1]);

            auto p0 = problem::Partition<eilig::Vectors>(pressureProblem->Pressure(), totalDofPressure, pivotPressure);
            auto p1 = problem::Partition<eilig::Vectors>(Vector(totalDofPressure, 0.0), totalDofPressure, pivotPressure);
            auto dp = problem::Partition<eilig::Vectors>(Vector(totalDofPressure, 0.0), totalDofPressure, pivotPressure);
            auto fu = problem::Partition<eilig::Vectors>(pressureProblem->LoadDistributedVolumeDivergence(fluidProblem), totalDofPressure, pivotPressure);
            auto fp = problem::Partition<eilig::Vectors>(pressureProblem->LoadDistributedVolumeGradient(), totalDofPressure, pivotPressure);
            eilig::IterativeBiCGStab(Mp[3], dp[1], -dt * fu[1] -(dt * dt) * fp[1]), callbackIterativePressure);

            p1[0] = p0[0] + dp[0];
            p1[1] = p0[1] + dp[1];

            pressureProblem->UpdateMeshValues(p1[0], p1[1]);

            auto fc = problem::Partition<eilig::Vectors>(G_Transpose * pressureProblem->Pressure(), totalDofMomentum, pivotMomentum);
            eilig::IterativeBiCGStab(M[3], dqq[1], -dt * fc[1], callbackIterativeMomentum);

            q2[0] = q0[0] + dq[0] + dqq[0];
            q2[1] = q0[1] + dq[1] + dqq[1];

            fluidProblem->UpdateMeshValues(q2[0], q2[1]);

            auto numerator = eilig::NormP2(q2[1] - q0[1]);
            auto denominator = eilig::NormP2(q0[1]);
            denominator = (denominator < 1e-10) ? 1.0 : denominator;
            auto norm = numerator / denominator;

            steps.push_back(step);
            norms.push_back(norm);

            logger::Info("EXAMPLES", utils::string::Format("Step = {} | Norm = {:.3e}", step, norm));

            if (norm < 1.0e-3 || step == 250)
            {
                break;
            }

            step += 1;
        }

        SaveToFile("bearing_flat_hydrostatic_fluid.dat", probeFluid);
        SaveToFile("bearing_flat_hydrostatic_fluid_inlet.dat", probeFluidInlet);
        SaveToFile("bearing_flat_hydrostatic_fluid_outlet.dat", probeFluidOutlet);

        SaveToFile("bearing_flat_hydrostatic_pressure.dat", probePressure);
        SaveToFile("bearing_flat_hydrostatic_pressure_top.dat", probePressureTop);
        SaveToFile("bearing_flat_hydrostatic_pressure_bottom.dat", probePressureBottom);
    }
    catch (const std::exception& error)
    {
        logger::Error("DIVE", error.what());
    }

    return 0;
}

/*
        while (true)
        {
            auto q0 = problem::Partition<eilig::Vectors>(fluidProblem->Momentum(), totalDofMomentum, pivotMomentum);
            auto v0 = problem::Partition<eilig::Vectors>(fluidProblem->Velocity(), totalDofMomentum, pivotMomentum);
            auto q1 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto q2 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto dq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto dqq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            eilig::IterativeBiCGStab(M[3], dq[1], - dt * (K[2] * v0[0] + K[3] * v0[1]), callbackIterativeMomentum);

            q1[0] = q0[0] + dq[0];
            q1[1] = q0[1] + dq[1];

            fluidProblem->UpdateMeshValues(q1[0], q1[1]);

            auto p1 = problem::Partition<eilig::Vectors>(pressureProblem->Pressure(), totalDofPressure, pivotPressure);
            auto fu = problem::Partition<eilig::Vectors>(pressureProblem->LoadDistributedVolumeDivergence(fluidProblem), totalDofPressure, pivotPressure);
            eilig::IterativeBiCGStab(H[3], p1[1], - H[2] * p1[0] - 1.0 / dt * (fu[1]), callbackIterativePressure);

            pressureProblem->UpdateMeshValues(p1[0], p1[1]);

            auto fc = problem::Partition<eilig::Vectors>(G_Transpose * pressureProblem->Pressure(), totalDofMomentum, pivotMomentum);
            eilig::IterativeBiCGStab(M[3], dqq[1], -dt * fc[1], callbackIterativeMomentum);

            q2[0] = q0[0] + dq[0] + dqq[0];
            q2[1] = q0[1] + dq[1] + dqq[1];

            fluidProblem->UpdateMeshValues(q2[0], q2[1]);

            auto numerator = eilig::NormP2(q2[1] - q0[1]);
            auto denominator = eilig::NormP2(q0[1]);
            denominator = (denominator < 1e-10) ? 1.0 : denominator;
            auto norm = numerator / denominator;

            steps.push_back(step);
            norms.push_back(norm);

            logger::Info("EXAMPLES", utils::string::Format("Step = {} | Norm = {:.3e}", step, norm));

            if (norm < 1.0e-2)
            {
                break;
            }

            step += 1;
        }

*/