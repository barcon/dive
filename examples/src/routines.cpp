#include "routines.hpp"

void InitializeGmsh(const dive::String& meshFile)
{
    gmsh::initialize();
    gmsh::open(meshFile);
}
void FinalizeGmsh()
{
    gmsh::finalize();
}

dive::problem::ProblemFluidPtr CreateProblemFluid(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads)
{
	dive::mesh::IMeshPtr mesh{ nullptr };
    dive::problem::ProblemFluidPtr problem{ nullptr };

    auto physicalGroups = dive::mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if(groupNameSplit[0] == "fluid")
            {
                mesh = dive::mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = dive::problem::CreateProblemFluid(problemTag, mesh);

                break;
            }
        }
    }

    if (problem == nullptr)
    {
        throw std::runtime_error("Mesh file does not have specified domain");
    }

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<dive::DofIndex>(groupNameSplit[2]);
                if (dofIndex >= problem->GetNumberDof())
                {
                    throw std::runtime_error("Mesh file with invalid dof index");
                }

                auto it = loads.find(groupName);
                if (it == loads.end())
                {
                    logger::Warning("GRADIENT", "Missing load for physical group: " + groupName);
                    continue;
                }

                auto load = std::dynamic_pointer_cast<value::IScalarCoordinates>(it->second);
                if (load != nullptr)
                {
                    auto nodes = dive::mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = dive::load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}
dive::problem::ProblemThermalPtr CreateProblemThermal(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads)
{
    dive::mesh::IMeshPtr mesh{ nullptr };
    dive::problem::ProblemThermalPtr problem{ nullptr };

    auto physicalGroups = dive::mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "thermal")
            {
                mesh = dive::mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = dive::problem::CreateProblemThermal(problemTag, mesh);

                break;
            }
        }
    }

    if (problem == nullptr)
    {
        throw std::runtime_error("Mesh file does not have specified domain");
    }

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });
                
        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<dive::DofIndex>(groupNameSplit[2]);
                if (dofIndex >= problem->GetNumberDof())
                {
                    throw std::runtime_error("Mesh file with invalid dof index");
                }

                auto it = loads.find(groupName);
                if (it == loads.end())
                {
                    logger::Warning("GRADIENT", "Missing load for physical group: " + groupName);
                    continue;
                }

                auto load = std::dynamic_pointer_cast<value::IScalarCoordinates>(it->second);
                if (load != nullptr)
                {
                    auto nodes = dive::mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = dive::load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}
dive::problem::ProblemPressurePtr CreateProblemPressure(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads)
{
    dive::mesh::IMeshPtr mesh{ nullptr };
    dive::problem::ProblemPressurePtr problem{ nullptr };

    auto physicalGroups = dive::mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "pressure")
            {
                mesh = dive::mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = dive::problem::CreateProblemPressure(problemTag, mesh);

                break;
            }
        }
    }

    if (problem == nullptr)
    {
        throw std::runtime_error("Mesh file does not have specified domain");
    }

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<dive::DofIndex>(groupNameSplit[2]);
                if (dofIndex >= problem->GetNumberDof())
                {
                    throw std::runtime_error("Mesh file with invalid dof index");
                }

                auto it = loads.find(groupName);
                if (it == loads.end())
                {
                    logger::Warning("GRADIENT", "Missing load for physical group: " + groupName);
                    continue;
                }

                auto load = std::dynamic_pointer_cast<value::IScalarCoordinates>(it->second);
                if (load != nullptr)
                {
                    auto nodes = dive::mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = dive::load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}
dive::problem::ProblemStructuralPtr CreateProblemStructural(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads)
{
    dive::mesh::IMeshPtr mesh{ nullptr };
    dive::problem::ProblemStructuralPtr problem{ nullptr };

    auto physicalGroups = dive::mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "structural")
            {
                mesh = dive::mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = dive::problem::CreateProblemStructural(problemTag, mesh);

                break;
            }
        }
    }

    if (problem == nullptr)
    {
        throw std::runtime_error("Mesh file does not have specified domain");
    }

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<dive::DofIndex>(groupNameSplit[2]);
                if (dofIndex >= problem->GetNumberDof())
                {
                    throw std::runtime_error("Mesh file with invalid dof index");
                }

                auto it = loads.find(groupName);
                if (it == loads.end())
                {
                    logger::Warning("GRADIENT", "Missing load for physical group: " + groupName);
                    continue;
                }

                auto load = std::dynamic_pointer_cast<value::IScalarCoordinates>(it->second);
                if (load != nullptr)
                {
                    auto nodes = dive::mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = dive::load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
            if (groupNameSplit[1] == "load")
            {
                if (groupNameSplit[2] == "node")
                {
                    auto it = loads.find(groupName);
                    if (it == loads.end())
                    {
                        logger::Warning("GRADIENT", "Missing load for physical group: " + groupName);
                        continue;
                    }

                    auto value = std::dynamic_pointer_cast<value::IVectorCoordinates>(it->second);

                    if (value != nullptr)
                    {
                        auto nodes = dive::mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                        for (auto& node : nodes)
                        {
                            auto loadNode = dive::load::CreateLoadNode(node, value);
                            problem->ApplyLoad(loadNode);

                            std::cout << "Applied Load" << std::endl;
                        }
                    }
                }
            }
        }
    }

    return problem;
}
dive::problem::ProblemDeformationLaplacePtr CreateProblemDeformationLaplace(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads)
{
    dive::mesh::IMeshPtr mesh{ nullptr };
    dive::problem::ProblemDeformationLaplacePtr problem{ nullptr };

    auto physicalGroups = dive::mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "deformation")
            {
                mesh = dive::mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = dive::problem::CreateProblemDeformationLaplace(problemTag, mesh);

                break;
            }
        }
    }

    if (problem == nullptr)
    {
        throw std::runtime_error("Mesh file does not have specified domain");
    }

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<dive::DofIndex>(groupNameSplit[2]);
                if (dofIndex >= problem->GetNumberDof())
                {
                    throw std::runtime_error("Mesh file with invalid dof index");
                }

                auto it = loads.find(groupName);
                if (it == loads.end())
                {
                    logger::Warning("GRADIENT", "Missing load for physical group: " + groupName);
                    continue;
                }

                auto load = std::dynamic_pointer_cast<value::IScalarCoordinates>(it->second);
                if (load != nullptr)
                {
                    auto nodes = dive::mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = dive::load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}
dive::problem::ProblemDeformationInterpolationPtr CreateProblemDeformationInterpolation(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads)
{
    dive::mesh::IMeshPtr mesh{ nullptr };
    dive::problem::ProblemDeformationInterpolationPtr problem{ nullptr };

    auto physicalGroups = dive::mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "deformation")
            {
                mesh = dive::mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = dive::problem::CreateProblemDeformationInterpolation(problemTag, mesh);

                break;
            }
        }
    }

    if (problem == nullptr)
    {
        throw std::runtime_error("Mesh file does not have specified domain");
    }

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = dive::mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<dive::DofIndex>(groupNameSplit[2]);
                if (dofIndex >= problem->GetNumberDof())
                {
                    throw std::runtime_error("Mesh file with invalid dof index");
                }

                auto it = loads.find(groupName);
                if (it == loads.end())
                {
                    logger::Warning("GRADIENT", "Missing load for physical group: " + groupName);
                    continue;
                }

                auto load = std::dynamic_pointer_cast<value::IScalarCoordinates>(it->second);
                if (load != nullptr)
                {
                    auto nodes = dive::mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = dive::load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}