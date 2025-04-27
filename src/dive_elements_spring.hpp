#ifndef DIVE_ELEMENTS_SPRING_HPP_
#define DIVE_ELEMENTS_SPRING_HPP_

#include "dive_elements.hpp"

namespace dive
{
	namespace elements
	{

		ElementSpringPtr CreateElementSpring();
		ElementSpringPtr CreateElementSpring(Tag elementTag);

		class ElementSpring : public IElement, virtual public std::enable_shared_from_this<ElementSpring>
		{
		public:
			virtual ~ElementSpring() = default;

			static ElementSpringPtr Create();
			ElementSpringPtr GetPtr();
			ConstElementSpringPtr GetPtr() const;

			Matrix u() const override;
			Matrix u(const Vector& local) const override;

			Matrix du() const override;
			Matrix du(const Vector& local) const override;
			Matrix du(const Vector& local, const Dimension& dimension) const override;

			Matrix J(const Vector& local) const override;
			Matrix InvJ(const Vector& local) const override;
			Matrix N(const Vector& local) const override;
			Matrix dN(const Vector& local) const override;

			const Matrix& J(const Vector& local, CacheIndex cacheIndex) const override;
			const Matrix& InvJ(const Vector& local, CacheIndex cacheIndex) const override;
			const Matrix& N(const Vector& local, CacheIndex cacheIndex) const override;
			const Matrix& dN(const Vector& local, CacheIndex cacheIndex) const override;

			Scalar DetJ(const Vector& local) const override;
			Scalar DelA(const Vector& local, const Dimension& dim1, const Dimension& dim2) const override;
			Scalar DelL(const Vector& local, const Dimension& dim1) const override;

			Scalar DetJ(const Vector& local, CacheIndex cacheIndex) const override;

			Scalar Volume() const override;
			Scalar Area(const FaceIndex& face) const override;
			Scalar Length(const EdgeIndex& edge) const override;
			Scalar Size() const override;

			Vector LocalCoordinates(INodePtr node) const override;
			Vector LocalCoordinates(const NodeIndex& nodeIndex) const override;
			
			Vector GlobalCoordinates(INodePtr node) const override;
			Vector GlobalCoordinates(const NodeIndex& nodeIndex) const override;
			Vector GlobalCoordinates(const Vector& local) const override;
			Vector GlobalDerivatives(const Vector& local, const Dimension& dim) const override;

			INodePtr GetNodeFace(const FaceIndex& faceNumber, const NodeIndex& nodeNumber) const override;
			INodePtr GetNodeEdge(const EdgeIndex& edgeNumber, const NodeIndex& nodeNumber) const override;
			INodePtr GetNode(const NodeIndex& nodeNumber) const override;
			const Nodes& GetNodes() const override;
			Order GetOrder() const override;
			Parametric GetParametric() const override;
			Type GetType() const override;
			NodeIndex GetNodeIndex(INodePtr node) const override;
			NumberNodes GetNumberNodesFace(const FaceIndex& faceNumber) const override;
			NumberNodes GetNumberNodesEdge(const EdgeIndex& edgeNode) const override;
			NumberNodes GetNumberNodes() const override;
			NumberFaces GetNumberFaces() const override;
			NumberEdges GetNumberEdges() const override;
			Dimension GetDimension() const override;
			Vector GetCenter() const override;
			Tag GetTag() const override;
			IMaterialPtr GetMaterial() const override;
			ElementIndex GetElementIndex() const override;
			NumberDof GetNumberDof() const override;

			IGaussPtr IntegralVolume() const override;
			IGaussPtr IntegralArea() const override;
			IGaussPtr IntegralEdge() const override;

			IntegralAreaHelper GetIntegralAreaHelper(FaceIndex faceIndex) const override;
			IntegralEdgeHelper GetIntegralEdgeHelper(EdgeIndex edgeIndex) const override;

			void SetNumberDof(NumberDof numberDof) override;
			void SetNode(const NodeIndex& nodeNumber, INodePtr node) override;
			void SetOrder(const Order& order) override;
			void SetParametric(const Parametric& order) override;
			void SetTag(Tag elementTag) override;
			void SetMaterial(IMaterialPtr material) override;
			void SetElementIndex(ElementIndex index) override;

			bool IsUsed(INodePtr node) const override;
			void InitializeCache() override;

		protected:
			ElementSpring();

			Tag		tag_{ 0 };
			Type	type_{ element_hexa8 };
			Nodes	nodes_;
			Nodes	nodesLocal_;

			ElementIndex	elementIndex_;

			NumberDof	numberDof_{ 3 };
			NumberNodes numberNodes_;
			NumberNodes numberNodesFace_;
			NumberNodes numberNodesEdge_;
			NumberNodes numberNodesParametric_;
			NumberNodes numberNodesFaceParametric_;

			static const Dimension dimension_;
			static const NumberFaces numberFaces_;
			static const NumberEdges numberEdges_;
			static const Scalar localCoordinates_[20][3];

			static const Index lookUpTable1_[6];
			static const Index lookUpTable2_[6];
			static const Index lookUpTable3_[6];
			static const Scalar lookUpTable4_[6];
			static const Index lookUpTable5_[12];
			static const Index lookUpTable6_[12];
			static const Index lookUpTable7_[12];
			static const Scalar lookUpTable8_[12];
			static const Scalar lookUpTable9_[12];

			NodeIndex* lookUpTable10_{ &linearFunctions_.NodeIndexFace[0] };
			NodeIndex* lookUpTable11_{ &linearFunctions_.NodeIndexFace[0] };

			Order	order_{ linear };
			ShapePtr* shape_{ &linearFunctions_.Shape[0] };
			ShapePtr* shapeD_{ &linearFunctions_.ShapeD[0] };	

			Parametric	parametric_{ linear };
			ShapePtr*	param_ = { &linearFunctions_.Shape[0] };
			ShapePtr*	paramD_ = { &linearFunctions_.ShapeD[0] };

			IMaterialPtr material_{ nullptr };

			IGaussPtr gaussSpring_{ nullptr };
			IGaussPtr gaussRect_{ nullptr };
			IGaussPtr gaussLine_{ nullptr };

			Matrices cacheJ_;
			Matrices cacheInvJ_;
			Matrices cacheN_;
			Matrices cachedN_;
			Scalars	 cacheDetJ_;
		};

	} //namespace elements
} //namespace dive

#endif /* DIVE_ELEMENTS_SPRING_HPP_*/
