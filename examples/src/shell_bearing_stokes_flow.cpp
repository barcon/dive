#include "routines.hpp"

using namespace dive;

auto cartesian = CreateBasisCartesian(1);

Scalar pi = utils::math::Pi<Scalar>();
Scalar temperature = 273.15;
Scalar pressure = 101325.1;
Scalar diameterBearing = 0.12007;
Scalar diameterShaft = 0.11993;
Scalar widthBearing = 0.06;
Scalar clearance = (diameterBearing - diameterShaft) / 2.0;
Scalar attitude = 335.0 * (pi / 180.0);
Scalar eccentricityRelative = 0.8;
Scalar eccentricity = eccentricityRelative * clearance;
Scalar speedRotational = 2000.0;
Scalar speedAngular = 2.0 * pi * speedRotational / 60.0;
Scalar speedSliding = speedAngular * (diameterShaft / 2.0);

auto velocityWall0 = CreateValueScalarCoordinates(3, 0.0);
auto velocityWall1 = CreateValueScalarCoordinates(3, 0.0);
auto velocityWall2 = CreateValueScalarCoordinates(3, 0.0);

auto velocitySliding0 = CreateValueScalarCoordinates(3, 1.0);
auto velocitySliding1 = CreateValueScalarCoordinates(3, 0.0);
auto velocitySliding2 = CreateValueScalarCoordinates(3, 0.0);

Values loadCase =
{
    {"fluid:dirichlet:0:wall", velocityWall0},
    {"fluid:dirichlet:1:wall", velocityWall1},
    {"fluid:dirichlet:2:wall", velocityWall2},

    {"fluid:dirichlet:0:sliding", velocitySliding0},
    {"fluid:dirichlet:1:sliding", velocitySliding1},
    {"fluid:dirichlet:2:sliding", velocitySliding2},

};

eilig::CallbackIterative callbackIterative = [](Index iteration, Scalar residual) -> long long int
    {
        Scalar tolerance{ 1e-5 };

        logger::Info("DIVE", utils::string::Format("Iteration: {}, Residual: {:.3e}", iteration, residual));

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
        InitializeGmsh("./mesh_cavity.msh");

        auto fluidMaterial = material::CreateOilVG100(1, temperature, pressure);
        auto fluidProblem = CreateProblemFluid(1, "fluid", loadCase);
        auto thermalProblem = CreateProblemThermal(2, "thermal", loadCase);
        auto pressureProblem = CreateProblemPressure(3, "pressure", loadCase);

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

        auto state = material::CreateState(temperature + 58.0, pressure);
        auto sizeElement = mesh::GetSizeMinimum(fluidProblem->GetMesh()->GetElements());
        auto rho = fluidMaterial->GetDensity(state);
        auto cp = fluidMaterial->GetSpecificHeat(state);
        auto mu = fluidMaterial->GetDynamicViscosity(state);
        auto kinematicViscosity = mu / rho;

        auto dt1 = (sizeElement * sizeElement) / kinematicViscosity;
        auto dt2 = sizeElement / (speedSliding);
        auto dt = 0.2 * std::min(dt1, dt2);

		std::cout << utils::string::Format("Size element = {:.3e} | Speed fluid = {:.3e} | Time step = {:.3e}", sizeElement, speedSliding, dt) << std::endl;

//-------------------------------------------------------------------------------------------------

    	fluidProblem->Initialize();
		thermalProblem->Initialize();
		pressureProblem->Initialize();

        auto totalDofMomentum = fluidProblem->GetTotalDof();
        auto totalDofPressure = pressureProblem->GetTotalDof();      
        auto pivotMomentum = fluidProblem->GetPivot();
        auto pivotPressure = pressureProblem->GetPivot();

        auto M = problem::Partition<eilig::Sparses>(fluidProblem->Mass(false), totalDofMomentum, pivotMomentum);
        auto K = problem::Partition<eilig::Sparses>(fluidProblem->Stiffness(), totalDofMomentum, pivotMomentum);     
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

            eilig::IterativeBiCGStab(M[3], dq[1], -dt * (K[2] * q0[0] + K[3] * q0[1]), callbackIterative);
            //eilig::IterativeBiCGStab(K[3], dq[1], - K[2] * q0[0] , callbackIterative);

            q1[0] = q0[0] + dq[0];
            q1[1] = q0[1] + dq[1];

            fluidProblem->UpdateMeshValuesMomentum(q1[0], q1[1]);

           /*auto q = fluidProblem->Momentum();
            auto fg = problem::Partition<eilig::Vectors>(G * q, totalDofPressure, pivotPressure);
            auto p = problem::Partition<eilig::Vectors>(pressureProblem->Pressure(), totalDofPressure, pivotPressure);
            eilig::IterativeBiCGStab(H[3], p[1], -H[2] * p[0] + (1.0 / dt) * (fg[1]), callbackIterative);
            pressureProblem->UpdateMeshValues(p[0], p[1]);

            auto p1 = pressureProblem->Pressure();
            auto fc = problem::Partition<eilig::Vectors>(G_Transpose * p1, totalDofMomentum, pivotMomentum);
            eilig::IterativeBiCGStab(M[3], dqq[1], -dt * (fc[1]), callbackIterative);

            q2[0] = q1[0] + dqq[0];
            q2[1] = q1[1] + dqq[1];

            fluidProblem->UpdateMeshValuesMomentum(q2[0], q2[1]);*/

            auto norm = eilig::NormP2(dq[1]);
            steps.push_back(step);
            norms.push_back(norm);
           
			std::cout << utils::string::Format("Step = {} | Norm = {:.3e}", step, norm) << std::endl;

            if (norm < 1e-5) 
            {
                break;
            }

            step += 1;            
        }

        //std::cout << ListNodes(nodesPressure) << std::endl;
    }
    catch (const std::exception& error)
    {
        logger::Error("DIVE", error.what());
    }

    return 0;
}