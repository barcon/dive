#include "routines.hpp"

using namespace dive;

auto cartesianBearing  = CreateBasisCartesian(1);
auto cartesianShaft    = CreateBasisCartesian(2);
auto cylindricalShaft  = CreateBasisCylindrical(3);

Scalar pi = utils::math::Pi<Scalar>();
Scalar diameterBearing = 0.12007;
Scalar diameterShaft = 0.10993;
Scalar clearance = (diameterBearing - diameterShaft) / 2.0;
Scalar attitude = 45.0 * (pi / 180.0);
Scalar eccentricityRelative = 0.9;
Scalar eccentricity = eccentricityRelative * clearance;

auto deformationBearing0 = CreateValueScalarCoordinates(3, 0.0);
auto deformationBearing1 = CreateValueScalarCoordinates(3, 0.0);
auto deformationBearing2 = CreateValueScalarCoordinates(3, 0.0);

auto deformationShaft0 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
		Vector offset{ eccentricity, attitude, 0.0 };

        return cylindricalShaft->LocalToGlobalVector(offset)(0);
    });
auto deformationShaft1 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
        Vector offset{ eccentricity, attitude, 0.0 };

        return cylindricalShaft->LocalToGlobalVector(offset)(1);
    });
auto deformationShaft2 = CreateValueScalarCoordinatesFunction([](Scalar x, Scalar y, Scalar z) -> Scalar
    {
        Vector offset{ eccentricity, attitude, 0.0 };

        return cylindricalShaft->LocalToGlobalVector(offset)(2);
    });

Values loadCase =
{
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
        InitializeGmsh("./mesh_deformation.msh");

        auto fluidProblem = CreateProblemFluid(1, "fluid", loadCase);
        auto deformationProblem = CreateProblemDeformationInterpolation(2, "deformation", loadCase);
        auto nodes = mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "fluid:domain");

        FinalizeGmsh();

        //---------------------------------------------------------------------------------------------------------------------------

        auto interpolation = interpolation::CreateInterpolationRBF(cartesianBearing, FunctionType::TPS, 1.0);
        deformationProblem->Initialize(interpolation);
        deformationProblem->DeformMesh(fluidProblem->GetMesh());

        SaveToFile("mesh_deformation_rbf.dat", nodes);
    }
    catch (const std::exception& error)
    {
        logger::Error("GRADIENT", error.what());
    }

    return 0;
}