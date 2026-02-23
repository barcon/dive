#ifndef DIVE_MESH_BASIC_HPP_
#define DIVE_MESH_BASIC_HPP_

#include "dive_mesh.hpp"

namespace dive
{
	namespace mesh
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

			NumberDof	GetTotalDof() override;
			NumberDof	GetPivot() override;
			Tag			GetTag() const override;

			void SetTag(Tag tag) override;

			void SortNodesByTag() override;
			void SortElementsByTag() override;

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

		Scalar GetSizeMinimum(const Elements& elements);
		Scalar GetSizeMaximum(const Elements& elements);

		void ApplyMaterial(IMeshPtr mesh, IMaterialPtr material);
		void ApplyValue(IMeshPtr mesh, IVector3DPtr value);
		void DeformByInterpolation(IMeshPtr mesh, IInterpolationPtr interpolation);

		void GmshInitialize();	
		void GmshOpenFile(const String& fileName);
		void GmshFinalize();

		using PhysicalGroup = std::pair<int, int>;
		using PhysicalGroups = std::vector<PhysicalGroup>;

		PhysicalGroups GmshGetPhysicalGroups();
		PhysicalGroup GmshGetPhysicalGroupByName(const String& groupName);
		String GmshGetPhysicalName(int dim, int tag);
		Nodes GmshGetNodesForPhysicalGroup(IMeshPtr mesh, const String& groupName);
		EdgePairs GmshGetEdgesForPhysicalGroup(IMeshPtr mesh, const String& groupName);
		FacePairs GmshGetFacesForPhysicalGroup(IMeshPtr mesh, const String& groupName);
		Elements GmshGetElementsForPhysicalGroup(IMeshPtr mesh, const String& groupName);
		IMeshPtr GmshGetMeshForPhysicalGroup(Tag meshTag, NumberDof numberDof, const String& groupName);

	} //namespace mesh
} //namespace dive

#endif /* DIVE_MESH_BASIC_HPP_ */