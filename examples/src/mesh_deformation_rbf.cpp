#include "routines.hpp"

Json settings = {
    {"reference",
        {
            {"temperature", 273.15},
            {"pressure", 101325.1}
        }
    }
};

auto cartesianBearing  = basis::CreateBasisCartesian(1);
auto cartesianShaft    = basis::CreateBasisCartesian(2);
auto cylindricalShaft  = basis::CreateBasisCylindrical(3);

dive::Scalar pi = utils::math::Pi<dive::Scalar>();
dive::Scalar diameterBearing = 0.12007;
dive::Scalar diameterShaft = 0.10993;
dive::Scalar clearance = (diameterBearing - diameterShaft) / 2.0;
dive::Scalar attitude = 90.0 * (pi / 180.0);
dive::Scalar eccentricityRelative = 0.5;
dive::Scalar eccentricity = eccentricityRelative * clearance;

auto deformationBearing0 = value::CreateValueScalarCoordinates(3, 0.0);
auto deformationBearing1 = value::CreateValueScalarCoordinates(3, 0.0);
auto deformationBearing2 = value::CreateValueScalarCoordinates(3, 0.0);

auto deformationShaft0 = value::CreateValueScalarCoordinatesFunction([](dive::Scalar x, dive::Scalar y, dive::Scalar z) -> dive::Scalar
    {
		dive::Vector offset{ eccentricity, attitude, 0.0 };

        return cylindricalShaft->LocalToGlobalVector(offset)(0);
    });
auto deformationShaft1 = value::CreateValueScalarCoordinatesFunction([](dive::Scalar x, dive::Scalar y, dive::Scalar z) -> dive::Scalar
    {
        dive::Vector offset{ eccentricity, attitude, 0.0 };

        return cylindricalShaft->LocalToGlobalVector(offset)(1);
    });
auto deformationShaft2 = value::CreateValueScalarCoordinatesFunction([](dive::Scalar x, dive::Scalar y, dive::Scalar z) -> dive::Scalar
    {
        dive::Vector offset{ eccentricity, attitude, 0.0 };

        return cylindricalShaft->LocalToGlobalVector(offset)(2);
    });

dive::Values loadCase =
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
    dive::Vector offset{eccentricity, attitude, 0.0};

    cartesianShaft->Offset(cylindricalShaft->LocalToGlobalVector(offset));
    cylindricalShaft->Offset(cylindricalShaft->LocalToGlobalVector(offset));

    try
    {
        InitializeGmsh("./mesh_deformation.msh");

        auto fluidProblem = CreateProblemFluid(1, "fluid", loadCase);
        auto deformationProblem = CreateProblemDeformationInterpolation(2, "deformation", loadCase);
        auto nodes = dive::mesh::GmshGetNodesForPhysicalGroup(fluidProblem->GetMesh(), "pressure:dirichlet:0:upstream");

        auto nodesDeformation = deformationProblem->GetMesh()->GetNodes();

        FinalizeGmsh();

        //---------------------------------------------------------------------------------------------------------------------------

        auto interpolation = interpolation::CreateInterpolationRBF(cartesianBearing, interpolation::interpolation_function_TPS, 1.0);
        deformationProblem->Initialize(interpolation);
        deformationProblem->DeformMesh(fluidProblem->GetMesh());

        std::cout << dive::ListNodes(nodes) << std::endl;
    }
    catch (const std::exception& error)
    {
        logger::Error("GRADIENT", error.what());
    }

    return 0;
}