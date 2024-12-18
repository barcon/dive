#include "dive_meshes.hpp"
#include "dive_status.hpp"

#include <algorithm>
#include <memory>

namespace dive {
	namespace meshes {
		IMeshPtr CreateMesh()
		{
			auto res = IMesh::Create();

			return res;
		}
		IMeshPtr CreateMesh(Tag tag)
		{
			auto res = IMesh::Create();

			res->SetTag(tag);

			return res;
		}
		IMeshPtr IMesh::Create()
		{
			class MakeSharedEnabler : public IMesh
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		IMeshPtr IMesh::GetPtr()
		{
			return std::dynamic_pointer_cast<IMesh>(shared_from_this());
		}
		ConstIMeshPtr IMesh::GetPtr() const
		{
			return const_cast<IMesh*>(this)->GetPtr();
		}
		void IMesh::AddNode(NodePtr node, Status& status, bool IndexCheck)
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
		void IMesh::AddElement(ElementPtr element, Status& status, bool IndexCheck)
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
		void IMesh::RemoveNode(NodePtr node, Status& status)
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
		void IMesh::RemoveNode(Tag tag, Status& status)
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
		void IMesh::RemoveElement(ElementPtr element, Status& status)
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
		void IMesh::RemoveElement(Tag tag, Status& status)
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
		const Nodes& IMesh::GetNodes() const
		{
			return nodes_;
		}
		const Elements& IMesh::GetElements() const
		{
			return elements_;
		}
		const DofMeshIndices& IMesh::GetDofIndices() const
		{
			return dofIndices_;
		}
		const NodeMeshIndices& IMesh::GetNodeIndices() const
		{
			return nodeIndices_;
		}
		NodePtr IMesh::GetNode(Tag tag, Status& status) const
		{
			NodePtr res{ nullptr };

			auto index = GetIndexNode(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = nodes_[index];

			return res;
		}
		NodePtr IMesh::GetNodeSorted(Tag tag, Status& status) const
		{
			NodePtr res{ nullptr };

			auto index = GetIndexNodeSorted(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = nodes_[index];

			return res;
		}
		ElementPtr IMesh::GetElement(Tag tag, Status& status) const
		{
			ElementPtr res{ nullptr };

			auto index = GetIndexElement(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = elements_[index];

			return res;
		}
		ElementPtr IMesh::GetElementSorted(Tag tag, Status& status) const
		{
			ElementPtr res{ nullptr };

			auto index = GetIndexElementSorted(tag, status);
			if (status != DIVE_SUCCESS)
			{
				return res;
			}

			res = elements_[index];

			return res;
		}
		NumberDof IMesh::GetTotalDof()
		{
			return totalDof_;
		}
		Index IMesh::GetPivot()
		{
			return pivot_;
		}
		Tag IMesh::GetTag() const
		{
			return tag_;
		}
		void IMesh::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void IMesh::SortNodesTag()
		{
			std::sort(nodes_.begin(), nodes_.end(), [&](dive::NodePtr ptr1, dive::NodePtr ptr2) -> bool
				{
					return ptr1->GetTag() < ptr2->GetTag();
				});
		}
		void IMesh::SortElementsTag()
		{
			std::sort(elements_.begin(), elements_.end(), [&](dive::ElementPtr ptr1, dive::ElementPtr ptr2) -> bool
				{
					return ptr1->GetTag() < ptr2->GetTag();
				});
		}
		void IMesh::SortDofIndices()
		{
			std::sort(dofIndices_.begin(), dofIndices_.end(), [&](DofMeshIndex dofIndex1, DofMeshIndex dofIndex2) -> bool
				{
					if (dofIndex1.node != dofIndex2.node)
					{
						return dofIndex1.node->GetTag() < dofIndex2.node->GetTag();
					}

					return dofIndex1.dofIndex < dofIndex2.dofIndex;
				});
		}
		Index IMesh::GetIndexNode(Tag tag, Status& status) const
		{
			auto it = std::find_if(nodes_.begin(), nodes_.end(), 
					[&](NodePtr ptr1) -> bool
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
		Index IMesh::GetIndexNodeSorted(Tag tag, Status& status) const
		{
			auto it = std::lower_bound(nodes_.begin(), nodes_.end(), tag,
				[&](NodePtr ptr, Tag value) -> bool
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
		Index IMesh::GetIndexElement(Tag tag, Status& status) const
		{
			auto it = std::find_if(elements_.begin(), elements_.end(),
				[&](ElementPtr ptr1) -> bool
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
		Index IMesh::GetIndexElementSorted(Tag tag, Status& status) const
		{
			auto it = std::lower_bound(elements_.begin(), elements_.end(), tag,
				[&](ElementPtr ptr, Tag value) -> bool
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
	} // namespace meshes
} // namespace dive

/*

		void IMesh::SetValue(const Vector& u)
		{
			for (auto& dofIndex : dofIndices_)
			{
				auto dof = dofIndex.dof;
				auto value = u(dofIndex.index);

				std::static_pointer_cast<nodes::IMesh>(dofIndex.ptr)->SetDofValue(dof, value);
			}

		}

*/