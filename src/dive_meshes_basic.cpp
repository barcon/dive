#include "dive_meshes_basic.hpp"
#include "dive_status.hpp"
#include "dive_elements.hpp"
#include "dive_elements_hexa.hpp"
#include "dive_selection.hpp"

#include "gmsh.h"

#include <algorithm>
#include <memory>
#include <limits>

namespace dive {
	namespace meshes {
		MeshPtr CreateMesh()
		{
			auto res = Mesh::Create();

			return res;
		}
		MeshPtr CreateMesh(Tag meshTag)
		{
			auto res = Mesh::Create();

			res->SetTag(meshTag);

			return res;
		}
		MeshPtr Mesh::Create()
		{
			class MakeSharedEnabler : public Mesh
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MeshPtr Mesh::GetPtr()
		{
			return std::dynamic_pointer_cast<Mesh>(shared_from_this());
		}
		ConstMeshPtr Mesh::GetPtr() const
		{
			return const_cast<Mesh*>(this)->GetPtr();
		}
		void Mesh::AddNode(INodePtr node, Status& status, bool IndexCheck)
		{
			if (node == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return;
			}

			if (nodes_.size() == std::numeric_limits<Index>::max() - 1)
			{
				status = DIVE_MAX_OF_RANGE;
				return;
			}

			if (IndexCheck)
			{
				auto index = GetIndexNode(node->GetTag(), status);


				if (status == DIVE_SUCCESS)
				{
					status = DIVE_ALREADY_EXISTS;
					return;
				}
			}

			nodes_.push_back(node);
			status = DIVE_SUCCESS;
		}
		void Mesh::AddElement(IElementPtr element, Status& status, bool IndexCheck)
		{
			if (element == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return;
			}

			if (elements_.size() == std::numeric_limits<Index>::max() - 1)
			{
				status = DIVE_MAX_OF_RANGE;
				return;
			}

			if (IndexCheck)
			{
				auto index = GetIndexElement(element->GetTag(), status);

				if (status == DIVE_SUCCESS)
				{
					status = DIVE_ALREADY_EXISTS;
					return;
				}
			}

			elements_.push_back(element);
			status = DIVE_SUCCESS;
		}
		void Mesh::RemoveNode(INodePtr node, Status& status)
		{
			if (node == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return;
			}

			auto index = GetIndexNode(node->GetTag(), status);

			if (status == DIVE_NOT_FOUND)
			{
				return;
			}

			nodes_.erase(nodes_.begin() + index);
			status = DIVE_SUCCESS;
		}
		void Mesh::RemoveNode(Tag tag, Status& status)
		{
			auto index = GetIndexNode(tag, status);

			if (status == DIVE_NOT_FOUND)
			{
				return;
			}

			auto ptr = nodes_[index];

			nodes_.erase(nodes_.begin() + index);
			status = DIVE_SUCCESS;
		}
		void Mesh::RemoveElement(IElementPtr element, Status& status)
		{
			if (element == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return;
			}

			auto index = GetIndexElement(element->GetTag(), status);

			if (status == DIVE_NOT_FOUND)
			{
				return;
			}

			auto ptr = elements_[index];

			elements_.erase(elements_.begin() + index);
			status = DIVE_SUCCESS;
		}
		void Mesh::RemoveElement(Tag tag, Status& status)
		{
			auto index = GetIndexElement(tag, status);

			if (status == DIVE_NOT_FOUND)
			{
				return;
			}

			auto ptr = elements_[index];

			elements_.erase(elements_.begin() + index);
			status = DIVE_SUCCESS;
		}
		const Nodes& Mesh::GetNodes() const
		{
			return nodes_;
		}
		const Elements& Mesh::GetElements() const
		{
			return elements_;
		}
		INodePtr Mesh::GetNode(Tag tag, Status& status) const
		{
			INodePtr res{ nullptr };

			auto index = GetIndexNode(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = nodes_[index];

			return res;
		}
		INodePtr Mesh::GetNodeIndex(NodeIndex index, Status& status) const
		{
			INodePtr res{ nullptr };

			if (index >= nodes_.size())
			{
				status = DIVE_OUT_OF_RANGE;
				return res;
			}

			if (nodes_[index] == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return res;
			}

			status = DIVE_SUCCESS;
			res = nodes_[index];

			return res;
		}
		INodePtr Mesh::GetNodeSorted(Tag tag, Status& status) const
		{
			INodePtr res{ nullptr };

			auto index = GetIndexNodeSorted(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = nodes_[index];

			return res;
		}
		IElementPtr Mesh::GetElement(Tag tag, Status& status) const
		{
			IElementPtr res{ nullptr };

			auto index = GetIndexElement(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = elements_[index];

			return res;
		}
		IElementPtr Mesh::GetElementIndex(ElementIndex index, Status& status) const
		{
			IElementPtr res{ nullptr };

			if (index >= elements_.size())
			{
				status = DIVE_OUT_OF_RANGE;
				return res;
			}

			if (elements_[index] == nullptr)
			{
				status = DIVE_NULL_POINTER;
				return res;
			}

			status = DIVE_SUCCESS;
			res = elements_[index];

			return res;
		}
		IElementPtr Mesh::GetElementSorted(Tag tag, Status& status) const
		{
			IElementPtr res{ nullptr };

			auto index = GetIndexElementSorted(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = elements_[index];

			return res;
		}
		NumberDof Mesh::GetTotalDof()
		{
			return totalDof_;
		}
		Index Mesh::GetPivot()
		{
			return pivot_;
		}
		Tag Mesh::GetTag() const
		{
			return tag_;
		}
		void Mesh::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void Mesh::SortNodesByTag()
		{
			nodes_ = selection::SortNodesByTag(nodes_);
		}
		void Mesh::SortElementsByTag()
		{
			elements_ = selection::SortElementsByTag(elements_);
		}
		Index Mesh::GetIndexNode(Tag tag, Status& status) const
		{
			auto it = std::find_if(nodes_.begin(), nodes_.end(),
				[&](INodePtr ptr1) -> bool
				{
					return ptr1->GetTag() == tag;
				});

			if (it == nodes_.end())
			{
				status = DIVE_NOT_FOUND;
				return std::numeric_limits<Index>::max();
			}

			status = DIVE_SUCCESS;
			return std::distance(nodes_.begin(), it);
		}
		Index Mesh::GetIndexNodeSorted(Tag tag, Status& status) const
		{
			auto it = std::lower_bound(nodes_.begin(), nodes_.end(), tag,
				[&](INodePtr ptr, Tag value) -> bool
				{
					return ptr->GetTag() < value;
				});

			if (it == nodes_.end())
			{
				status = DIVE_NOT_FOUND;
				return std::numeric_limits<Index>::max();
			}

			status = DIVE_SUCCESS;
			return std::distance(nodes_.begin(), it);
		}
		Index Mesh::GetIndexElement(Tag tag, Status& status) const
		{
			auto it = std::find_if(elements_.begin(), elements_.end(),
				[&](IElementPtr ptr1) -> bool
				{
					return ptr1->GetTag() == tag;
				});

			if (it == elements_.end())
			{
				status = DIVE_NOT_FOUND;
				return std::numeric_limits<Index>::max();
			}

			status = DIVE_SUCCESS;
			return std::distance(elements_.begin(), it);
		}
		Index Mesh::GetIndexElementSorted(Tag tag, Status& status) const
		{
			auto it = std::lower_bound(elements_.begin(), elements_.end(), tag,
				[&](IElementPtr ptr, Tag value) -> bool
				{
					return ptr->GetTag() < value;
				});

			if (it == elements_.end())
			{
				status = DIVE_NOT_FOUND;
				return std::numeric_limits<Index>::max();
			}

			status = DIVE_SUCCESS;
			return std::distance(elements_.begin(), it);
		}

		Scalar GetSizeMinimum(const Elements& elements)
		{
			Scalar size{ 0.0 };
			Scalar res{ std::numeric_limits<Scalar>::infinity() };

			for (const auto& element : elements)
			{
				size = element->SizeMinimum();

				if (size < res)
				{
					res = size;
				}
			}

			return res;
		}
		Scalar GetSizeMaximum(const Elements& elements)
		{
			Scalar size{ 0.0 };
			Scalar res{ 0.0 };

			for (const auto& element : elements)
			{
				size = element->SizeMaximum();

				if (size > res)
				{
					res = size;
				}
			}

			return res;
		}

		void ApplyMaterial(IMeshPtr mesh, IMaterialPtr material)
		{
			auto& elements = mesh->GetElements();
			for (auto& element : elements)
			{
				element->SetMaterial(material);
			}
		}
		void DeformByInterpolation(IMeshPtr mesh, IInterpolationPtr interpolation)
		{
			auto& nodes = mesh->GetNodes();

			for (auto& node : nodes)
			{
				auto point = node->GetPoint();
				auto aux = node::CreateNode(0, point);

				interpolation->GetValue(aux);

				auto disp = Vector(aux->GetValue(), 0);

				node->SetPoint(point + disp);
			}
		}

		using PhysicalGroup = std::pair<int, int>;
		using PhysicalGroups = std::vector<PhysicalGroup>;

		using EntityTags = std::vector<int>;
		using ElementTypes = std::vector<int>;
		using ElementTags = std::vector<std::size_t>;
		using NodeTags = std::vector<std::size_t>;

		const Index lookUpTableHexa8[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
		const Index lookUpTableHexa20[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 13, 9, 16, 18, 19, 17, 10, 12, 14, 15 };

		void GmshInitialize()
		{
			gmsh::initialize();
		}
		void GmshFinalize()
		{
			gmsh::finalize();
		}
		void GmshOpenFile(const String& fileName)
		{
			try {
				gmsh::open(fileName);
			}
			catch (...) {
				logger::Error(headerDive, "Gmsh could not open file " + fileName);
				exit(0);
			}
		}

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
		Nodes GmshGetNodesForPhysicalGroup(IMeshPtr mesh, const String& groupName)
		{
			Nodes nodes;
			NodeTags nodeTags;
			Scalars coordinates;
			Status status;
			
			auto group = GmshGetPhysicalGroupByName(groupName);
			if (group.first == -1 && group.second == -1)
			{
				logger::Error(headerDive, "Gmsh physical group " + groupName + " not found");
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
			if (group.first == -1 && group.second == -1)
			{
				logger::Error(headerDive, "Gmsh physical group " + groupName + " not found");
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
			if (group.first == -1 && group.second == -1)
			{
				logger::Error(headerDive, "Gmsh physical group " + groupName + " not found");
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
			if (group.first == -1 && group.second == -1)
			{
				logger::Error(headerDive, "Gmsh physical group " + groupName + " not found");
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
		IMeshPtr GmshGetMeshForPhysicalGroup(Tag meshTag, NumberDof numberDof, const String& groupName)
		{
			IMeshPtr mesh{ nullptr };
			Status status;

			auto group = GmshGetPhysicalGroupByName(groupName);
			if (group.first == -1 && group.second == -1)
			{
				logger::Error(headerDive, "Gmsh physical group " + groupName + " not found");
				return mesh;
			}

			auto dimension = group.first;
			auto tag = group.second;
			
			mesh = CreateMesh(meshTag);
			{
				Scalars coordinates;
				NodeTags nodeTags;
				gmsh::model::mesh::getNodesForPhysicalGroup(dimension, tag, nodeTags, coordinates);
			
				for (NodeIndex i = 0; i < nodeTags.size(); ++i)
				{
					auto nodeTag = nodeTags[i];
					auto x = coordinates[3 * i + 0];
					auto y = coordinates[3 * i + 1];
					auto z = coordinates[3 * i + 2];

					auto node = node::CreateNode(nodeTag, x, y, z);
					node->SetNumberDof(numberDof);
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
							auto element = elements::CreateElementHexa82(elementTag);
							element->SetNumberDof(numberDof);

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
							auto element = elements::CreateElementHexa202(elementTag);
							element->SetNumberDof(numberDof);

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

	} // namespace meshes
} // namespace dive