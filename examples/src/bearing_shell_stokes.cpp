#include "routines.hpp"

using namespace dive;

auto cartesian = CreateBasisCartesian(1);
auto cartesianShaft = CreateBasisCartesian(2);
auto cylindricalShaft = CreateBasisCylindrical(3);

Scalar pi = utils::math::Pi<Scalar>();
Scalar temperature = 273.15;
Scalar pressure = 101325.1;
Scalar diameterBearing = 0.12007;
Scalar diameterShaft = 0.11993;
Scalar widthBearing = 0.06;
Scalar clearance = (diameterBearing - diameterShaft) / 2.0;
Scalar attitude = 335.0 * (pi / 180.0);
Scalar eccentricityRelative = 0.7;
Scalar eccentricityAbsolute = eccentricityRelative * clearance;
Scalar speedRotational = 2000.0;
Scalar speedAngular = 2.0 * pi * speedRotational / 60.0;
Scalar speedSliding = speedAngular * (diameterShaft / 2.0);

auto state = CreateState(temperature, pressure);
auto fluidMaterial = CreateOilVG100(1, temperature, pressure);
auto density = fluidMaterial->GetDensity(state);
auto dynamicViscosity = fluidMaterial->GetDynamicViscosity(state);
auto kinematicViscosity = dynamicViscosity / density;

auto velocityBearing0 = CreateValueScalarCoordinates(3, 0.0);
auto velocityBearing1 = CreateValueScalarCoordinates(3, 0.0);
auto velocityBearing2 = CreateValueScalarCoordinates(3, 0.0);

auto velocityShaft0 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
        Vector radius{ x, y, z };
		Vector omega{ 0.0, 0.0, speedAngular };
		Vector res = Cross(omega, radius);

        return cartesianShaft->LocalToGlobalVector(res)(0);
    });
auto velocityShaft1 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
        Vector radius{ x, y, z };
        Vector omega{ 0.0, 0.0, speedAngular };
        Vector res = Cross(omega, radius);

        return cartesianShaft->LocalToGlobalVector(res)(1);
    });
auto velocityShaft2 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
        Vector radius{ x, y, z };
        Vector omega{ 0.0, 0.0, speedAngular };
        Vector res = Cross(omega, radius);

        return cartesianShaft->LocalToGlobalVector(res)(2);
    });

auto pressureInlet = CreateValueScalarCoordinates(3, pressure);
//auto pressureUpstream = CreateValueScalarCoordinates(3, 0.0);
//auto pressureDownstream = CreateValueScalarCoordinates(3, 0.0);

auto deformationBearing0 = CreateValueScalarCoordinates(3, 0.0);
auto deformationBearing1 = CreateValueScalarCoordinates(3, 0.0);
auto deformationBearing2 = CreateValueScalarCoordinates(3, 0.0);

auto deformationShaft0 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
        return cylindricalShaft->GetOrigin()(0);
    });
auto deformationShaft1 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
        return cylindricalShaft->GetOrigin()(1);
    });
auto deformationShaft2 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
        return cylindricalShaft->GetOrigin()(2);
    });

Values loadCase =
{
    {"fluid:dirichlet:0:bearing", velocityBearing0},
    {"fluid:dirichlet:1:bearing", velocityBearing1},
    {"fluid:dirichlet:2:bearing", velocityBearing2},

    {"fluid:dirichlet:0:shaft", velocityShaft0},
    {"fluid:dirichlet:1:shaft", velocityShaft1},
    {"fluid:dirichlet:2:shaft", velocityShaft2},
    
    {"pressure:dirichlet:0:inlet", pressureInlet},
    //{"pressure:dirichlet:0:upstream", pressureUpstream},
	//{"pressure:dirichlet:0:downstream", pressureDownstream},
    
    {"deformation:dirichlet:0:bearing", deformationBearing0},
    {"deformation:dirichlet:1:bearing", deformationBearing1},
    {"deformation:dirichlet:2:bearing", deformationBearing2},

    {"deformation:dirichlet:0:shaft", deformationShaft0},
    {"deformation:dirichlet:1:shaft", deformationShaft1},
    {"deformation:dirichlet:2:shaft", deformationShaft2},
};

int main()
{
    try 
    {
		auto offset = cylindricalShaft->LocalToGlobalVector(Vector{ eccentricityAbsolute, attitude, 0.0 });
		cartesianShaft->Offset(offset);
		cylindricalShaft->Offset(offset);

        InitializeGmsh("./bearing_shell_stokes.msh");

        auto fluidMaterial = material::CreateOilVG100(1, temperature, pressure);
        auto fluidProblem = CreateProblemFluid(1, "fluid", loadCase);
        auto thermalProblem = CreateProblemThermal(2, "thermal", loadCase);
        auto pressureProblem = CreateProblemPressure(3, "pressure", loadCase);
        auto deformationProblem = CreateProblemDeformationStrain(4, "deformation", loadCase);
        auto probePressureBearing = mesh::GmshGetNodesForPhysicalGroup(pressureProblem->GetMesh(), "probe:pressure:bearing");
        auto probePressureShaft = mesh::GmshGetNodesForPhysicalGroup(pressureProblem->GetMesh(), "probe:pressure:shaft");

        auto nodes = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "fluid:domain");
            
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
        auto dt2 = sizeElement / (speedSliding);
        auto dt = 0.2 * dt1 * dt2 / (dt1 + dt2);

		std::cout << utils::string::Format("Size element = {:.3e} | Speed fluid = {:.3e} | Time step = {:.3e}", sizeElement, speedSliding, dt) << std::endl;

//-------------------------------------------------------------------------------------------------

        //auto interpolation = interpolation::CreateInterpolationRBF(cartesian, FunctionType::TPS, 1.0);
        deformationProblem->Initialize();
        deformationProblem->DeformMesh(fluidProblem->GetMesh());
        deformationProblem->DeformMesh(thermalProblem->GetMesh());
        deformationProblem->DeformMesh(pressureProblem->GetMesh());	
    
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

            eilig::IterativeBiCGStab(M[3], dq[1], -dt * (K[2] * q0[0] + K[3] * q0[1]), callbackIterativeMomentum);

            q1[0] = q0[0] + dq[0];
            q1[1] = q0[1] + dq[1];

            fluidProblem->UpdateMeshValuesMomentum(q1[0], q1[1]);
            
            auto fg= problem::Partition<eilig::Vectors>(G * fluidProblem->Momentum(), totalDofPressure, pivotPressure);                       
            auto p0 = problem::Partition<eilig::Vectors>(pressureProblem->Pressure(), totalDofPressure, pivotPressure);
            auto p1 = problem::Partition<eilig::Vectors>(Vector(totalDofPressure, 0.0), totalDofPressure, pivotPressure);
            auto dp = problem::Partition<eilig::Vectors>(Vector(totalDofPressure, 0.0), totalDofPressure, pivotPressure);
            eilig::IterativeBiCGStab(Mp[3], dp[1], dt * (fg[1] - dt * H[2] * p0[0] - dt * H[3] * p0[1]), callbackIterativePressure);

            p1[0] = p0[0] + dp[0];
            p1[1] = p0[1] + dp[1];

            pressureProblem->UpdateMeshValues(p1[0], p1[1]);

            auto p = pressureProblem->Pressure();
            auto fc = problem::Partition<eilig::Vectors>(G_Transpose * p, totalDofMomentum, pivotMomentum);
            eilig::IterativeBiCGStab(M[3], dqq[1], -dt * (fc[1]), callbackIterativeMomentum);

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

        SaveToFile("bearing_shell_stokes_pressure.dat", probePressureBearing);
        SaveToFile("bearing_shell_stokes_pressure_shaft.dat", probePressureShaft);
    }
    catch (const std::exception& error)
    {
        logger::Error("DIVE", error.what());
    }

    return 0;
}