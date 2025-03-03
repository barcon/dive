#ifndef DIVE_MESHES_BASIC_HPP_
#define DIVE_MESHES_BASIC_HPP_

#include "dive_meshes.hpp"

namespace dive
{
	namespace meshes
	{
		MeshPtr CreateMesh();
		MeshPtr CreateMesh(Tag meshTag);

		class Mesh : public IMesh, virtual public std::enable_shared_from_this<Mesh>
		{
		public:
			virtual ~Mesh() = default;

			static MeshPtr Create();
			MeshPtr GetPtr();
			ConstMeshPtr GetPtr() const;

			void AddNode(INodePtr node, Status& status, bool IndexCheck = false) override;
			void AddElement(IElementPtr element, Status& status, bool IndexCheck = false) override;

			void RemoveNode(INodePtr node, Status& status) override;
			void RemoveNode(Tag tag, Status& status) override;
			void RemoveElement(IElementPtr element, Status& status) override;
			void RemoveElement(Tag tag, Status& status) override;

			const Nodes& GetNodes() const override;
			const Elements& GetElements() const override;

			INodePtr	GetNode(Tag tag, Status& status) const override;
			INodePtr	GetNodeIndex(NodeIndex index, Status& status) const override;
			INodePtr	GetNodeSorted(Tag tag, Status& status) const override;
			IElementPtr	GetElement(Tag tag, Status& status) const override;
			IElementPtr	GetElementIndex(ElementIndex index, Status& status) const override;
			IElementPtr	GetElementSorted(Tag tag, Status& status) const override;
			Scalar		GetElementSizeMinium() const override;

			NumberDof	GetTotalDof() override;
			NumberDof	GetPivot() override;
			Tag			GetTag() const override;

			void SetTag(Tag tag) override;

			void SortNodesTag() override;
			void SortElementsTag() override;

			Index GetIndexNode(Tag tag, Status& status) const override;
			Index GetIndexNodeSorted(Tag tag, Status& status) const override;
			Index GetIndexElement(Tag tag, Status& status) const override;
			Index GetIndexElementSorted(Tag tag, Status& status) const override;

		protected:
			Mesh() = default;

			NumberDof totalDof_{ 0 };
			Index pivot_{ 0 };

			Tag			tag_{ 0 };
			Nodes 		nodes_;
			Elements	elements_;
		};

		IMeshPtr LoadGmsh(Tag meshTag, String fileName, NumberDof numberDof, Status& status);

		void DeformByInterpolation(IMeshPtr mesh, IInterpolationPtr interpolation);

	} //namespace meshes
} //namespace dive

#endif /* DIVE_MESHES_BASIC_HPP_ */