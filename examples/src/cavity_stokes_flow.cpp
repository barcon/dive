#include "routines.hpp"

using namespace dive;

Scalar temperature = 273.15;
Scalar pressure = 101325.1;

auto cartesian  = CreateBasisCartesian(1);
auto state = CreateState(temperature, pressure);
auto fluidMaterial = CreateOilVG100(1, temperature, pressure);
auto density = fluidMaterial->GetDensity(state);
auto dynamicViscosity = fluidMaterial->GetDynamicViscosity(state);
auto kinematicViscosity = dynamicViscosity / density;

auto sizeDomain = 1.0;
auto reynolds   = 10.0;
auto speed = reynolds * dynamicViscosity / (density * sizeDomain);

auto velocityTop0 = CreateValueScalarCoordinates(3, speed);
auto velocityTop1 = CreateValueScalarCoordinates(3, 0.0);
auto velocityTop2 = CreateValueScalarCoordinates(3, 0.0);
auto velocityWall0 = CreateValueScalarCoordinates(3, 0.0);
auto velocityWall1 = CreateValueScalarCoordinates(3, 0.0);
auto velocityWall2 = CreateValueScalarCoordinates(3, 0.0);
auto velocitySymmetry = CreateValueScalarCoordinates(3, 0.0);
auto pressureCorner = CreateValueScalarCoordinates(3, 0.0);

Values loadCase =
{
    {"fluid:dirichlet:0:top", velocityTop0},
    {"fluid:dirichlet:1:top", velocityTop1},
    {"fluid:dirichlet:2:top", velocityTop2},
    {"fluid:dirichlet:0:wall", velocityWall0},
    {"fluid:dirichlet:1:wall", velocityWall1},
    {"fluid:dirichlet:2:wall", velocityWall2},
    {"fluid:dirichlet:2:symmetry", velocitySymmetry},
    {"pressure:dirichlet:0:corner", pressureCorner},
};

CallbackIterative callbackIterative = [](Index iteration, Scalar residual) -> long long int
    {
        Scalar tolerance{ 1e-6 };

        //logger::Info("DIVE", utils::string::Format("Iteration: {}, Residual: {:.3e}", iteration, residual));

        if (std::isnan(residual))
        {
            return eilig::EILIG_NOT_CONVERGED;
        }

        if (residual < tolerance)
        {
            return eilig::EILIG_SUCCESS;
        }


        return eilig::EILIG_CONTINUE;
    };

int main()
{
    try
    {
        InitializeGmsh("./cavity_stokes_flow.msh");

        auto fluidMaterial = CreateOilVG100(1, temperature, pressure);
        auto fluidProblem = CreateProblemFluid(1, "fluid", loadCase);
        auto thermalProblem = CreateProblemThermal(2, "thermal", loadCase);
        auto pressureProblem = CreateProblemPressure(3, "pressure", loadCase);
        auto probeVelocity = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "probe");
        auto probePressure = mesh::GmshGetNodesForPhysicalGroup(pressureProblem->GetMesh(), "probe");

        thermalProblem->SetPressure(std::static_pointer_cast<value::IScalarCoordinates>(pressureProblem->GetPressure()));
        pressureProblem->SetTemperature(std::static_pointer_cast<value::IScalarCoordinates>(thermalProblem->GetTemperature()));
        fluidProblem->SetPressure(std::static_pointer_cast<value::IScalarCoordinates>(pressureProblem->GetPressure()));
        fluidProblem->SetTemperature(std::static_pointer_cast<value::IScalarCoordinates>(thermalProblem->GetTemperature()));

        mesh::ApplyMaterial(fluidProblem->GetMesh(), fluidMaterial);
        mesh::ApplyMaterial(thermalProblem->GetMesh(), fluidMaterial);
        mesh::ApplyMaterial(pressureProblem->GetMesh(), fluidMaterial);
        mesh::ApplyValue(thermalProblem->GetMesh(), CreateValueMatrixCoordinates(3, { {temperature} }));
        mesh::ApplyValue(pressureProblem->GetMesh(), CreateValueMatrixCoordinates(3, { {pressure} }));

        FinalizeGmsh();

//-------------------------------------------------------------------------------------------------

        auto sizeElement = mesh::GetSizeMinimum(fluidProblem->GetMesh()->GetElements());

        auto dt1 = (sizeElement * sizeElement) / (2.0 * kinematicViscosity);
        auto dt2 = sizeElement / (speed);
        auto dt = 0.1 * std::min(dt1, dt2);

		std::cout << utils::string::Format("Size element = {:.3e} | Speed fluid = {:.3e} | Time step = {:.3e}", sizeElement, speed, dt) << std::endl;

//-------------------------------------------------------------------------------------------------

		fluidProblem->Initialize();
		thermalProblem->Initialize();
		pressureProblem->Initialize();

        auto totalDofMomentum = fluidProblem->GetTotalDof();
        auto pivotMomentum = fluidProblem->GetPivot();
        
        auto totalDofPressure = pressureProblem->GetTotalDof();      
        auto pivotPressure = pressureProblem->GetPivot();

		std::cout << utils::string::Format("Total dof pressure = {} | Pivot pressure = {}", totalDofPressure, pivotPressure) << std::endl;

        auto M = problem::Partition<Sparses>(fluidProblem->Mass(false), totalDofMomentum, pivotMomentum);
        auto K = problem::Partition<Sparses>(fluidProblem->Stiffness(), totalDofMomentum, pivotMomentum);     
        auto H = problem::Partition<Sparses>(pressureProblem->Stiffness(), totalDofPressure, pivotPressure);
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
            
            eilig::IterativeBiCGStab(M[3], dq[1], - dt * (K[2] * q0[0] + K[3] * q0[1]), callbackIterative);

            q1[0] = q0[0] + dq[0];
            q1[1] = q0[1] + dq[1];

            fluidProblem->UpdateMeshValuesMomentum(q1[0], q1[1]);

            auto q = fluidProblem->Momentum();
            auto fg = problem::Partition<eilig::Vectors>(G * q, totalDofPressure, pivotPressure);
            auto p = problem::Partition<eilig::Vectors>(pressureProblem->Pressure(), totalDofPressure, pivotPressure);
            eilig::IterativeBiCGStab(H[3], p[1], -H[2] * p[0] + (1.0 / dt) * (fg[1]), callbackIterative);
            pressureProblem->UpdateMeshValues(p[0], p[1]);

            auto p1 = pressureProblem->Pressure();
            auto fc = problem::Partition<eilig::Vectors>(G_Transpose * p1, totalDofMomentum, pivotMomentum);
            eilig::IterativeBiCGStab(M[3], dqq[1], -dt * (fc[1]), callbackIterative);

            q2[0] = q1[0] + dqq[0];
            q2[1] = q1[1] + dqq[1];

            fluidProblem->UpdateMeshValuesMomentum(q2[0], q2[1]);

            auto numerator = eilig::NormP2(q2[1] - q0[1]);
			auto denominator = eilig::NormP2(q0[1]);
			denominator = (denominator < 1e-10) ? 1.0 : denominator;
            auto norm = numerator / denominator;

            steps.push_back(step);
            norms.push_back(norm);
           
			std::cout << utils::string::Format("Step = {} | Norm = {:.3e}", step, norm) << std::endl;

            if (norm < 1e-4) 
            {
                break;
            }

            step += 1;            
        }

        SaveToFile("cavity_stokes_flow_velocity.dat", probeVelocity);
        SaveToFile("cavity_stokes_flow_pressure.dat", probePressure);
    }
    catch (const std::exception& error)
    {
        logger::Error("DIVE", error.what());
    }

    return 0;
}