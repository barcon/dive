#include "routines.hpp"

void SaveToFile(const String& fileName, const Nodes& nodes)
{
    File file;
    String output;

    file.SetName(fileName);
    file.SetMode(utils::file::Write);

    if (file.Open() != utils::file::UTILS_SUCCESS)
    {
        logger::Error(headerEilig, "File could not be created");
    }

    for (auto& node : nodes)
    {
        const auto& point = node->GetPoint();
        const auto& value = node->GetValue();

        for(NumberCoordinates i = 0; i < node->GetNumberCoordinates(); i++)
        {
            output += utils::string::Format("{:.5e} ", point(i));
        }
		
        for (NumberDof i = 0; i < node->GetNumberDof(); i++)
        {
            output += utils::string::Format("{:.5e} ", value(i));
        }

        output += utils::string::Format("\n");
    }

    file.Write(output);
}
void InitializeGmsh(const String& meshFile)
{
    gmsh::initialize();
    gmsh::open(meshFile);
}
void FinalizeGmsh()
{
    gmsh::finalize();
}

problem::ProblemFluidPtr CreateProblemFluid(Tag problemTag, const String problemName, const Values& loads)
{
	mesh::IMeshPtr mesh{ nullptr };
    problem::ProblemFluidPtr problem{ nullptr };

    auto physicalGroups = mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if(groupNameSplit[0] == "fluid")
            {
                mesh = mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = problem::CreateProblemFluid(problemTag, mesh);

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
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<DofIndex>(groupNameSplit[2]);
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
                    auto nodes = mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}
problem::ProblemThermalPtr CreateProblemThermal(Tag problemTag, const String problemName, const Values& loads)
{
    mesh::IMeshPtr mesh{ nullptr };
    problem::ProblemThermalPtr problem{ nullptr };

    auto physicalGroups = mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "thermal")
            {
                mesh = mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = problem::CreateProblemThermal(problemTag, mesh);

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
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });
                
        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<DofIndex>(groupNameSplit[2]);
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
                    auto nodes = mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}
problem::ProblemPressurePtr CreateProblemPressure(Tag problemTag, const String problemName, const Values& loads)
{
    mesh::IMeshPtr mesh{ nullptr };
    problem::ProblemPressurePtr problem{ nullptr };

    auto physicalGroups = mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "pressure")
            {
                mesh = mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = problem::CreateProblemPressure(problemTag, mesh);

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
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<DofIndex>(groupNameSplit[2]);
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
                    auto nodes = mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}
problem::ProblemStructuralPtr CreateProblemStructural(Tag problemTag, const String problemName, const Values& loads)
{
    mesh::IMeshPtr mesh{ nullptr };
    problem::ProblemStructuralPtr problem{ nullptr };

    auto physicalGroups = mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "structural")
            {
                mesh = mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = problem::CreateProblemStructural(problemTag, mesh);

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
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<DofIndex>(groupNameSplit[2]);
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
                    auto nodes = mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = load::CreateLoadDirichlet(node, dofIndex, load);
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
                        auto nodes = mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                        for (auto& node : nodes)
                        {
                            auto loadNode = load::CreateLoadNode(node, value);
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
problem::ProblemDeformationInterpolationPtr CreateProblemDeformationInterpolation(Tag problemTag, const String problemName, const Values& loads)
{
    mesh::IMeshPtr mesh{ nullptr };
    problem::ProblemDeformationInterpolationPtr problem{ nullptr };

    auto physicalGroups = mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "deformation")
            {
                mesh = mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = problem::CreateProblemDeformationInterpolation(problemTag, mesh);

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
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<DofIndex>(groupNameSplit[2]);
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
                    auto nodes = mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}
problem::ProblemDeformationStrainPtr CreateProblemDeformationStrain(Tag problemTag, const String problemName, const Values& loads)
{
    mesh::IMeshPtr mesh{ nullptr };
    problem::ProblemDeformationStrainPtr problem{ nullptr };

    auto physicalGroups = mesh::GmshGetPhysicalGroups();

    for (auto& physicalGroup : physicalGroups)
    {
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[1] == "domain")
        {
            if (groupNameSplit[0] == "deformation")
            {
                mesh = mesh::GmshGetMeshForPhysicalGroup(problemTag, 3, groupName);
                problem = problem::CreateProblemDeformationStrain(problemTag, mesh);

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
        auto groupName = mesh::GmshGetPhysicalName(physicalGroup.first, physicalGroup.second);
        auto groupNameSplit = utils::string::Split(groupName, { ':' });

        if (groupNameSplit[0] == problemName)
        {
            if (groupNameSplit[1] == "dirichlet")
            {
                auto dofIndex = utils::string::ConvertTo<DofIndex>(groupNameSplit[2]);
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
                    auto nodes = mesh::GmshGetNodesForPhysicalGroup(mesh, groupName);

                    for (auto& node : nodes)
                    {
                        auto dirichlet = load::CreateLoadDirichlet(node, dofIndex, load);
                        problem->ApplyLoad(dirichlet);
                    }
                }
            }
        }
    }

    return problem;
}