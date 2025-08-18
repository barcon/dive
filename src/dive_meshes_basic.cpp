#include "dive_meshes_basic.hpp"
#include "dive_status.hpp"
#include "dive_elements.hpp"
#include "dive_elements_hexa.hpp"

#include "cgnslib.h"

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
			std::sort(nodes_.begin(), nodes_.end(), 
				[&](dive::INodePtr ptr1, dive::INodePtr ptr2) -> bool
				{
					return ptr1->GetTag() < ptr2->GetTag();
				});
		}
		void Mesh::SortElementsByTag()
		{
			std::sort(elements_.begin(), elements_.end(), 
				[&](dive::IElementPtr ptr1, dive::IElementPtr ptr2) -> bool
				{
					return ptr1->GetTag() < ptr2->GetTag();
				});
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

		void DeformByInterpolation(IMeshPtr mesh, IInterpolationPtr interpolation)
		{
			auto& nodes = mesh->GetNodes();

			for (auto& node : nodes)
			{
				auto point = node->GetPoint();
				auto aux = nodes::CreateNode(0, point);

				interpolation->GetValue(aux);

				auto disp = Vector(aux->GetValue(), 0);

				node->SetPoint(point + disp);
			}
		}

	} // namespace meshes
} // namespace dive