#ifndef DIVE_MESHES_HPP_
#define DIVE_MESHES_HPP_

#include "dive_types.hpp"
#include "dive_elements.hpp"
#include "dive_loads.hpp"

namespace dive
{
	struct DofMeshIndex
	{
		INodePtr node;
		NodeIndex nodeIndex;
		DofIndex dofIndex;
		DofIndex globalIndex;
		IDirichletPtr dirichlet;
		DirichletIndex dirichletIndex;
		bool dirichletReordered;
	};
	struct NodeMeshIndex
	{
		Index index;
		INodePtr node;
		Indices dofIndices;
	};
	struct DirichletMeshIndex
	{
		INodePtr node;
		DofIndex dofIndex;
	};

	using DofMeshIndices = std::vector<DofMeshIndex>;
	using NodeMeshIndices = std::vector<std::vector<NodeMeshIndex>>;
	using DirichletMeshIndices = std::vector<DirichletMeshIndex>;

	namespace meshes
	{
		class IMesh
		{
		public:
			virtual ~IMesh() = default;

			virtual void AddNode(INodePtr node, Status& status, bool IndexCheck = false) = 0;
			virtual void AddElement(IElementPtr element, Status& status, bool IndexCheck = false) = 0;

			virtual void RemoveNode(INodePtr node, Status& status) = 0;
			virtual void RemoveNode(Tag tag, Status& status) = 0;
			virtual void RemoveElement(IElementPtr element, Status& status) = 0;
			virtual void RemoveElement(Tag tag, Status& status) = 0;

			virtual const Nodes& GetNodes() const = 0;
			virtual const Elements& GetElements() const = 0;

			virtual INodePtr	GetNode(Tag tag, Status& status) const = 0;
			virtual INodePtr	GetNodeIndex(NodeIndex index, Status& status) const = 0;
			virtual INodePtr	GetNodeSorted(Tag tag, Status& status) const = 0;
			virtual IElementPtr	GetElement(Tag tag, Status& status) const = 0;
			virtual IElementPtr	GetElementIndex(ElementIndex index, Status& status) const = 0;
			virtual IElementPtr	GetElementSorted(Tag tag, Status& status) const = 0;

			virtual NumberDof	GetTotalDof() = 0;
			virtual NumberDof	GetPivot() = 0;
			virtual Tag			GetTag() const = 0;

			virtual void SetTag(Tag tag) = 0;

			virtual void SortNodesTag() = 0;
			virtual void SortElementsTag() = 0;

			virtual Index GetIndexNode(Tag tag, Status& status) const = 0;
			virtual Index GetIndexNodeSorted(Tag tag, Status& status) const = 0;
			virtual Index GetIndexElement(Tag tag, Status& status) const = 0;
			virtual Index GetIndexElementSorted(Tag tag, Status& status) const = 0;

		};
	} //namespace meshes
} //namespace dive

#endif /* DIVE_MESHES_HPP_ */