#define WIN32_LEAN_AND_MEAN

#include "dive_routines.hpp"

namespace dive
{
    String ListNodes(const Nodes& nodes)
    {
        String output{};

        logger::Info(headerDive, utils::string::Format("Number of selected nodes: {:d}", nodes.size()));

        for (auto& node : nodes)
        {
            if (node == nullptr)
            {
                continue;
            }

            auto tag = node->GetTag();
            auto& point = node->GetPoint();
            auto& value = node->GetValue();

            output += utils::string::Format("{:8d}\t", tag);

            for (Index i = 0; i < node->GetPoint().GetRows(); ++i)
            {
                output += utils::string::Format("{:12.4e}\t", point(i));
            }

            for (Index i = 0; i < value.GetRows(); ++i)
            {
                for (Index j = 0; j < value.GetCols(); ++j)
                {
                    output += utils::string::Format("{:12.4e}\t", value(i, j));
                }
            }
            output += "\n";
        }

        return output;
    }
    String ListElements(const Elements& elements)
    {
        String output{};

        logger::Info(dive::headerDive, utils::string::Format("Number of selected elements: {:d}", elements.size()));

        for (auto& element : elements)
        {
            if (element == nullptr)
            {
                continue;
            }

            auto tag = element->GetTag();

            output += utils::string::Format("{:8d}\t", tag);

            for (NodeIndex i = 0; i < element->GetNumberNodes(); ++i)
            {
                auto node = element->GetNode(i);

                if (node == nullptr)
                {
                    output += utils::string::Format("{:8}\t", '-');
                }
                else
                {
                    output += utils::string::Format("{:8d}\t", node->GetTag());
                }
            }
            output += "\n";
        }

        return output;
    }    
    String ListVolumes(const Elements& elements)
    {
        String output{};
        String aux{};

        logger::Info(dive::headerDive, utils::string::Format("Number of selected elements: {:d}", elements.size()));

        for (const auto& element : elements)
        {
            if (element == nullptr)
            {
                continue;
            }

            auto element3D = std::dynamic_pointer_cast<elements::IElementSolid>(element);
            
            if (element3D != nullptr)
            {
                output += utils::string::Format("{:8d}\t", element3D->GetTag());
                output += utils::string::Format("{:12.4e}\n", element3D->Volume());
            }
        }

        return output;
    }
    String ListDofMeshIndices(const DofMeshIndices& dofMeshIndices)
    {
        String output{};

        logger::Info(headerDive, utils::string::Format("Number of dof indices: {:d}", dofMeshIndices.size()));

        for (auto& dofMeshIndex : dofMeshIndices)
        {
            auto tag = dofMeshIndex.node->GetTag();
            auto dofIndex = dofMeshIndex.dofIndex;
            auto globalIndex = dofMeshIndex.globalIndex;

            output += utils::string::Format("{:8d}\t", tag);
            output += utils::string::Format("{:8d}\t", dofIndex);
            output += utils::string::Format("{:8d}\n", globalIndex);
        }

        return output;
    }
    String ListNodeMeshIndices(const NodeMeshIndices& nodeMeshIndices)
    {
        String output{};

        logger::Info(dive::headerDive, utils::string::Format("Number of node indices: {:d}", nodeMeshIndices.size()));

        for (Index i = 0; i < nodeMeshIndices.size(); ++i)
        {
            for (Index j = 0; j < nodeMeshIndices[i].size(); ++j)
            {
                auto tag = nodeMeshIndices[i][j].node->GetTag();
                auto index = nodeMeshIndices[i][j].index;

                output += utils::string::Format("{:8d}\t", tag);
                output += utils::string::Format("{:8d}\t", index);

                for (Index k = 0; k < nodeMeshIndices[i][j].dofIndices.size(); ++k)
                {
                    auto dofMeshIndex = nodeMeshIndices[i][j].dofIndices[k];
                    output += utils::string::Format("{:8d}\t", dofMeshIndex);
                }

                output += "\n";
            }

            output += "\n";
        }

        return output;
    }
    String ListDirichletMeshIndices(const DirichletMeshIndices& dirichletMeshIndices)
    {
        String output{};

         logger::Info(dive::headerDive, utils::string::Format("Number of dirichlet indices: {:d}", dirichletMeshIndices.size()));

        for (auto& dirichletMeshIndex : dirichletMeshIndices)
        {
            auto tag = dirichletMeshIndex.node->GetTag();
            auto dofIndex = dirichletMeshIndex.dofIndex;

            output += utils::string::Format("{:8d}\t", tag);
            output += utils::string::Format("{:8d}\n", dofIndex);
        }

        return output;
    }
}