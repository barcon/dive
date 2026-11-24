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
auto reynolds   = 1000.0;
auto speed = reynolds * dynamicViscosity / (density * sizeDomain);

auto momentumTop0 = CreateValueScalarCoordinates(3, density * speed);
auto momentumTop1 = CreateValueScalarCoordinates(3, 0.0);
auto momentumTop2 = CreateValueScalarCoordinates(3, 0.0);
auto momentumWall0 = CreateValueScalarCoordinates(3, 0.0);
auto momentumWall1 = CreateValueScalarCoordinates(3, 0.0);
auto momentumWall2 = CreateValueScalarCoordinates(3, 0.0);
auto momentumSymmetry = CreateValueScalarCoordinates(3, 0.0);
auto pressureCorner = CreateValueScalarCoordinates(3, 0.0);

int main()
{
    try
    {
        InitializeGmsh("./cavity_flow.msh");

        auto fluidMaterial = CreateOilVG100(1, temperature, pressure);
        auto fluidProblem = CreateProblemFluid(1, "fluid");
        auto thermalProblem = CreateProblemThermal(2, "thermal");
        auto pressureProblem = CreateProblemPressure(3, "pressure");
        auto probeMomentum = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "probe");
        auto probePressure = mesh::GmshGetNodesForPhysicalGroup(pressureProblem->GetMesh(), "probe");

        thermalProblem->SetPressure(std::static_pointer_cast<value::IScalarCoordinates>(pressureProblem->GetPressure()));
        pressureProblem->SetTemperature(std::static_pointer_cast<value::IScalarCoordinates>(thermalProblem->GetTemperature()));
        fluidProblem->SetPressure(std::static_pointer_cast<value::IScalarCoordinates>(pressureProblem->GetPressure()));
        fluidProblem->SetTemperature(std::static_pointer_cast<value::IScalarCoordinates>(thermalProblem->GetTemperature()));

        ApplyMaterial(fluidProblem->GetMesh(), fluidMaterial);
        ApplyMaterial(thermalProblem->GetMesh(), fluidMaterial);
        ApplyMaterial(pressureProblem->GetMesh(), fluidMaterial);
        
        ApplyValue(thermalProblem->GetMesh(), CreateValueMatrixCoordinates(3, { {temperature} }));
        ApplyValue(pressureProblem->GetMesh(), CreateValueMatrixCoordinates(3, { {0.0} }));

        ApplyLoad(pressureProblem, "pressure:dirichlet:0:corner", pressureCorner);
        ApplyLoad(fluidProblem, "fluid:dirichlet:0:top", momentumTop0);
        ApplyLoad(fluidProblem, "fluid:dirichlet:1:top", momentumTop1);
        ApplyLoad(fluidProblem, "fluid:dirichlet:2:top", momentumTop2);

        FinalizeGmsh();

//-------------------------------------------------------------------------------------------------

        auto sizeElement = mesh::GetSizeMinimum(fluidProblem->GetMesh()->GetElements());

        auto dt1 = (sizeElement * sizeElement) / (2.0 * kinematicViscosity);
        auto dt2 = sizeElement / (speed);
        auto dt = 0.5 * dt1 * dt2 / (dt1 + dt2);

		logger::Info("EXAMPLES", utils::string::Format("Size element = {:.3e}", sizeElement));
		logger::Info("EXAMPLES", utils::string::Format("Speed fluid = {:.3e}", speed));
		logger::Info("EXAMPLES", utils::string::Format("Time step = {:.3e}", dt));

//-------------------------------------------------------------------------------------------------

		fluidProblem->Initialize();
		thermalProblem->Initialize();
		pressureProblem->Initialize();

        auto totalDofMomentum = fluidProblem->GetTotalDof();
        auto pivotMomentum = fluidProblem->GetPivot();
        
        auto totalDofPressure = pressureProblem->GetTotalDof();      
        auto pivotPressure = pressureProblem->GetPivot();
	
		logger::Info("EXAMPLES", utils::string::Format("Total dof momentum = {}", totalDofMomentum));
		logger::Info("EXAMPLES", utils::string::Format("Pivot momentum = {}", pivotMomentum));

        auto M = problem::Partition<Sparses>(fluidProblem->Mass(false), totalDofMomentum, pivotMomentum);
        auto K = problem::Partition<Sparses>(fluidProblem->Stiffness(), totalDofMomentum, pivotMomentum);     

        auto Mp = problem::Partition<eilig::Sparses>(pressureProblem->Mass(fluidProblem, false), totalDofPressure, pivotPressure);
        auto H = problem::Partition<eilig::Sparses>(pressureProblem->Stiffness(), totalDofPressure, pivotPressure);
        auto G = pressureProblem->Crossed(fluidProblem);
        auto G_Transpose = G.Transpose();

        Index step{ 1 };
		Indices steps;
        Scalars norms;
        
        while (true)
        {
            //auto C = problem::Partition<Sparses>(fluidProblem->Convection(), totalDofMomentum, pivotMomentum);

            auto v0 = problem::Partition<eilig::Vectors>(fluidProblem->Velocity(), totalDofMomentum, pivotMomentum);
            auto q0 = problem::Partition<eilig::Vectors>(fluidProblem->Momentum(), totalDofMomentum, pivotMomentum);
            auto q1 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto q2 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto dq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
            auto dqq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);

            eilig::IterativeBiCGStab(M[3], dq[1], -dt * (K[2] * v0[0] + K[3] * v0[1]), callbackIterativeMomentum);

            q1[0] = q0[0] + dq[0];
            q1[1] = q0[1] + dq[1];

            fluidProblem->UpdateMeshValues(q1[0], q1[1]);

            auto p0 = problem::Partition<eilig::Vectors>(pressureProblem->Pressure(), totalDofPressure, pivotPressure);
            auto p1 = problem::Partition<eilig::Vectors>(pressureProblem->Pressure(), totalDofPressure, pivotPressure);
            auto fd = problem::Partition<eilig::Vectors>(pressureProblem->Load("pressure:load:volume:divergence"), totalDofPressure, pivotPressure);
            eilig::IterativeBiCGStab(H[3], p1[1], -H[2] * p1[0] - 1.0 / dt * (fd[1]), callbackIterativePressure);

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

            std::cout << utils::string::Format("Step = {} | Norm = {:.3e}", step, norm) << std::endl;

            if (norm < 1.0e-4)
            {
                break;
            }

            step += 1;
        }

        SaveToFile("cavity_flow_momentum.dat", probeMomentum);
        SaveToFile("cavity_flow_pressure.dat", probePressure);
    }
    catch (const std::exception& error)
    {
        logger::Error("EXAMPLES", error.what());
    }

    return 0;
}

/*
while (true)
{
    auto C = problem::Partition<Sparses>(fluidProblem->Convection(), totalDofMomentum, pivotMomentum);

    auto q0 = problem::Partition<eilig::Vectors>(fluidProblem->Momentum(), totalDofMomentum, pivotMomentum);
    auto q1 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
    auto q2 = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
    auto dq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);
    auto dqq = problem::Partition<eilig::Vectors>(Vector(totalDofMomentum, 0.0), totalDofMomentum, pivotMomentum);

    eilig::IterativeBiCGStab(M[3], dq[1], -dt * (K[2] * q0[0] + C[2] * q0[0] + K[3] * q0[1] + C[3] * q0[1]), callbackIterativeMomentum);
    //eilig::IterativeBiCGStab(M[3], dq[1], -dt * (K[2] * q0[0] + K[3] * q0[1]), callbackIterativeMomentum);

    q1[0] = q0[0] + dq[0];
    q1[1] = q0[1] + dq[1];

    fluidProblem->UpdateMeshValuesMomentum(q1[0], q1[1]);

    auto fg = problem::Partition<eilig::Vectors>(G * fluidProblem->Momentum(), totalDofPressure, pivotPressure);
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

    if (norm < 5.0e-4)
    {
        break;
    }

    step += 1;
}
*/