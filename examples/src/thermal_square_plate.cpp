#include "routines.hpp"

using namespace dive;

Scalar temperature = 273.15;
Scalar pressure = 101325.1;

auto cartesian  = CreateBasisCartesian(1);
auto state = CreateState(temperature, pressure);
auto fluidMaterial = CreateOilVG100(1, temperature, pressure);

auto temperatureTop = CreateValueScalarCoordinates(3, 100.0);
auto temperatureLeft = CreateValueScalarCoordinates(3, 0.0);
auto temperatureRight = CreateValueScalarCoordinates(3, 0.0);
auto temperatureBottom = CreateValueScalarCoordinates(3, 0.0);

int main()
{
    try
    {
        InitializeGmsh("./thermal_square_plate.msh");

        auto thermalProblem = CreateProblemThermal(2, "thermal");
        auto probeThermal = GmshGetNodesForPhysicalGroup(thermalProblem->GetMesh(), "thermal:probe");

        thermalProblem->SetPressure(value::CreateValueScalarCoordinates(3, pressure));

        ApplyMaterial(thermalProblem->GetMesh(), fluidMaterial);

		ApplyLoad(thermalProblem, "thermal:dirichlet:0:top", temperatureTop);
		ApplyLoad(thermalProblem, "thermal:dirichlet:0:left", temperatureLeft);
		ApplyLoad(thermalProblem, "thermal:dirichlet:0:right", temperatureRight);
		ApplyLoad(thermalProblem, "thermal:dirichlet:0:bottom", temperatureBottom);

        FinalizeGmsh();

        //-------------------------------------------------------------------------------------------------

        thermalProblem->Initialize();

        auto totalDofThermal = thermalProblem->GetTotalDof();
        auto pivotThermal = thermalProblem->GetPivot();

        logger::Info("EXAMPLES", utils::string::Format("Total dof thermal = {}", totalDofThermal));
        logger::Info("EXAMPLES", utils::string::Format("Pivot thermal = {}", pivotThermal));

        auto K = problem::Partition<Sparses>(thermalProblem->Stiffness(), totalDofThermal, pivotThermal);
        auto q0 = problem::Partition<eilig::Vectors>(thermalProblem->Energy(), totalDofThermal, pivotThermal);
        eilig::IterativeBiCGStab(K[3], q0[1], -K[2] * q0[0], callbackIterativeThermal);

        thermalProblem->UpdateMeshValues(q0[0], q0[1]);

        SaveToFile("thermal_square_plate.dat", probeThermal);
    }
    catch (const std::exception& error)
    {
        logger::Error("EXAMPLES", error.what());
    }

    return 0;
}