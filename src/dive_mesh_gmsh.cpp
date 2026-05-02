#include "dive_mesh_gmsh.hpp"
#include "dive_status.hpp"
#include "dive_element_hexa.hpp"
#include "dive_selection.hpp"

#include "gmsh.h"

namespace dive {
	namespace mesh {
		using EntityTags = std::vector<int>;
		using ElementTypes = std::vector<int>;
		using ElementTags = std::vector<std::size_t>;
		using NodeTags = std::vector<std::size_t>;

		const Index lookUpTableHexa8[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
		const Index lookUpTableHexa20[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 13, 9, 16, 18, 19, 17, 10, 12, 14, 15 };

		PhysicalGroup GmshGetPhysicalGroupByName(const String& groupName)
		{
			PhysicalGroups res;

			gmsh::model::getPhysicalGroups(res);

			for (auto& group : res)
			{
				String name;
				gmsh::model::getPhysicalName(group.first, group.second, name);

				if (groupName == name)
				{
					return group;
				}
			}

			return PhysicalGroup(-1, -1);
		}
		
		void GmshInitialize()
		{
			gmsh::initialize();
		}
		void GmshFinalize()
		{
			gmsh::finalize();
		}

		PhysicalGroups GmshGetPhysicalGroups()
		{
			PhysicalGroups res;

			gmsh::model::getPhysicalGroups(res, -1);

			return res;
		}
		String GmshGetPhysicalName(int dim, int tag)
		{
			String res;

			gmsh::model::getPhysicalName(dim, tag, res);

			return res;
		}
		Nodes GmshGetNodesForPhysicalGroup(IMeshPtr mesh, const String& groupName)
		{
			Nodes nodes;
			NodeTags nodeTags;
			Scalars coordinates;
			Status status;
			
			auto group = GmshGetPhysicalGroupByName(groupName);
			if (group.first == -1 || group.second == -1)
			{
				logger::Error(headerDive, utils::string::Format("Gmsh physical group {} not found", groupName));
				return nodes;
			}

			gmsh::model::mesh::getNodesForPhysicalGroup(group.first, group.second, nodeTags, coordinates);
			for (Index i = 0; i < nodeTags.size(); ++i)
			{
				auto node = mesh->GetNodeSorted(nodeTags[i], status);

				nodes.emplace_back(node);
			}

			return nodes;
		}
		EdgePairs GmshGetEdgesForPhysicalGroup(IMeshPtr mesh, const String& groupName)
		{
			EdgePairs edgePairs;
			Status status;

			auto group = GmshGetPhysicalGroupByName(groupName);
			if (group.first == -1 || group.second == -1)
			{
				logger::Error(headerDive, utils::string::Format("Gmsh physical group {} not found", groupName));
				return edgePairs;
			}

			if (group.first != 1)
			{
				logger::Error(headerDive, "Physical group is not a edge group");
				return edgePairs;
			}

			EntityTags entities;
			gmsh::model::getEntitiesForPhysicalGroup(group.first, group.second, entities);

			for (auto& entity : entities)
			{
				NumberNodes counter{ 0 };
				NumberNodes numberNodes{ 0 };
				ElementTypes elementTypes;
				std::vector<ElementTags> elementTags;
				std::vector<NodeTags> nodeTags;

				gmsh::model::mesh::getElements(elementTypes, elementTags, nodeTags, group.first, entity);

				if (elementTypes[0] == 1)
				{
					numberNodes = 2;
				}
				else if (elementTypes[0] == 8)
				{
					numberNodes = 3;
				}
				else
				{
					logger::Error(headerDive, "Physical group contains unsupported element type");
					continue;
				}

				for (ElementIndex i = 0; i < elementTags[0].size(); ++i)
				{
					Nodes nodes;
					for (NodeIndex k = 0; k < numberNodes; ++k)
					{
						auto nodeTag = static_cast<Tag>(nodeTags[0][counter + k]);
						auto node = mesh->GetNodeSorted(nodeTag, status);

						nodes.emplace_back(node);
					}
					counter += numberNodes;

					auto elements = selection::FilterElementsByNodesIntersection(nodes);

					edgePairs.emplace_back(selection::FilterEdgeByNodes(elements[0], nodes));
				}
			}

			return edgePairs;
		}
		FacePairs GmshGetFacesForPhysicalGroup(IMeshPtr mesh, const String& groupName)
		{
			FacePairs facePairs;
			Status status;

			auto group = GmshGetPhysicalGroupByName(groupName);
			if (group.first == -1 || group.second == -1)
			{
				logger::Error(headerDive, utils::string::Format("Gmsh physical group {} not found", groupName));
				return facePairs;
			}

			if (group.first != 2)
			{
				logger::Error(headerDive, "Physical group is not a face group");
				return facePairs;
			}

			EntityTags entities;
			gmsh::model::getEntitiesForPhysicalGroup(group.first, group.second, entities);

			for (auto& entity : entities)
			{
				NumberNodes counter{ 0 };
				NumberNodes numberNodes{ 0 };
				ElementTypes elementTypes;
				std::vector<ElementTags> elementTags;
				std::vector<NodeTags> nodeTags;

				gmsh::model::mesh::getElements(elementTypes, elementTags, nodeTags, group.first, entity);

				if (elementTypes[0] == 3)
				{
					numberNodes = 4;
				}
				else if (elementTypes[0] == 16)
				{
					numberNodes = 8;
				}
				else
				{
					logger::Error(headerDive, "Physical group contains unsupported element type");
					continue;
				}

				for (ElementIndex i = 0; i < elementTags[0].size(); ++i)
				{
					Nodes nodes;
					for (NodeIndex k = 0; k < numberNodes; ++k)
					{
						auto nodeTag = static_cast<Tag>(nodeTags[0][counter + k]);
						auto node = mesh->GetNodeSorted(nodeTag, status);

						nodes.emplace_back(node);
					}
					counter += numberNodes;

					auto elements = selection::FilterElementsByNodesIntersection(nodes);

					facePairs.emplace_back(selection::FilterFaceByNodes(elements[0], nodes));
				}
			}

			return facePairs;
		}
		Elements GmshGetElementsForPhysicalGroup(IMeshPtr mesh, const String& groupName)
		{
			Elements elements;
			Status status;

			auto group = GmshGetPhysicalGroupByName(groupName);
			if (group.first == -1 || group.second == -1)
			{
				logger::Error(headerDive, utils::string::Format("Gmsh physical group {} not found", groupName));
				return elements;
			}

			{
				EntityTags entities;
				gmsh::model::getEntitiesForPhysicalGroup(group.first, group.second, entities);

				for (auto& entity : entities)
				{
					NumberNodes counter{ 0 };
					ElementTypes elementTypes;
					std::vector<ElementTags> elementTags;
					std::vector<NodeTags> nodeTags;

					gmsh::model::mesh::getElements(elementTypes, elementTags, nodeTags, group.first, entity);

					for (Index i = 0; i < elementTags[0].size(); ++i)
					{
						auto element = mesh->GetElementSorted(elementTags[0][i], status);
						elements.emplace_back(element);
					}
				}
			}

			return elements;
		}
		IMeshPtr GmshGetMeshForPhysicalGroup(Tag meshTag, NumberCoordinates numberCoordinates, const String& groupName)
		{
			IMeshPtr mesh{ nullptr };
			Status status;

			auto group = GmshGetPhysicalGroupByName(groupName);
			if (group.first == -1 || group.second == -1)
			{
				logger::Error(headerDive, utils::string::Format("Gmsh physical group {} not found", groupName));
				return mesh;
			}

			auto dimension = group.first;
			auto tag = group.second;
			
			mesh = CreateMesh(meshTag, numberCoordinates);
			{
				Scalars coordinates;
				NodeTags nodeTags;
				gmsh::model::mesh::getNodesForPhysicalGroup(dimension, tag, nodeTags, coordinates);
			
				for (NodeIndex i = 0; i < nodeTags.size(); ++i)
				{
					auto nodeTag = nodeTags[i];
					auto coordinate = Scalars(3);
					auto parametricCoordinate = Scalars(3);
					
					auto x = coordinates[3 * i + 0];
					auto y = coordinates[3 * i + 1];
					auto z = coordinates[3 * i + 2];
					auto node = node::CreateNode(nodeTag, x, y, z);

					mesh->AddNode(node, status, false);
				}

				mesh->SortNodesByTag();
			}

			{
				EntityTags entities;
				gmsh::model::getEntitiesForPhysicalGroup(dimension, tag, entities);

				for (auto& entity : entities)
				{
					NumberNodes counter{ 0 };
					ElementTypes elementTypes;
					std::vector<ElementTags> elementTags;
					std::vector<NodeTags> nodeTags;

					gmsh::model::mesh::getElements(elementTypes, elementTags, nodeTags, dimension, entity);

					for (ElementIndex i = 0; i < elementTags[0].size(); ++i)
					{
						if (elementTypes[0] == 5)
						{
							auto elementTag = static_cast<Tag>((elementTags[0][i]));
							auto element = element::CreateElementHexa82(elementTag);

							mesh->AddElement(element, status, false);
							auto numberNodes = element->GetNumberNodes();

							for(NodeIndex j = 0; j < numberNodes; ++j)
							{
								auto nodeTag = static_cast<Tag>(nodeTags[0][counter + lookUpTableHexa8[j]]);
								auto node = mesh->GetNodeSorted(nodeTag, status);
								element->SetNode(j, node);
							}
							
							counter += numberNodes;
						}
						else if (elementTypes[0] == 17)
						{
							auto elementTag = static_cast<Tag>((elementTags[0][i]));
							auto element = element::CreateElementHexa202(elementTag);

							mesh->AddElement(element, status, false);
							auto numberNodes = element->GetNumberNodes();

							for (NodeIndex j = 0; j < numberNodes; ++j)
							{
								auto nodeTag = static_cast<Tag>(nodeTags[0][counter + lookUpTableHexa20[j]]);
								auto node = mesh->GetNodeSorted(nodeTag, status);
								element->SetNode(j, node);
							}

							counter += numberNodes;
						}
					}
				}

				mesh->SortElementsByTag();
			}

			return mesh;
		}
	} // namespace mesh
} // namespace dive