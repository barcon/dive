#ifndef DIVE_ELEMENTS_SPRING_HPP_
#define DIVE_ELEMENTS_SPRING_HPP_

#include "dive_elements.hpp"

namespace dive
{
	namespace elements
	{
		ElementSpringPtr CreateElementSpring();
		ElementSpringPtr CreateElementSpring(Tag elementTag, IScalar1DPtr stiffness);

		class ElementSpring : public IElement, virtual public std::enable_shared_from_this<ElementSpring>
		{
		public:
			virtual ~ElementSpring() = default;

			static ElementSpringPtr Create();
			ElementSpringPtr GetPtr();
			ConstElementSpringPtr GetPtr() const;

			Tag GetTag() const override;
			Type GetType() const override;
			Vector GetCenter() const override;
			IMaterialPtr GetMaterial() const override;
			
			NodeIndex GetNodeIndex(INodePtr node) const override;
			ElementIndex GetElementIndex() const override;

			NumberDof GetNumberDof() const override;
			NumberNodes GetNumberNodes() const override;
			NumberFaces GetNumberFaces() const override;
			NumberEdges GetNumberEdges() const override;
			NumberNodes GetNumberNodesFace(const FaceIndex& faceNumber) const override;
			NumberNodes GetNumberNodesEdge(const EdgeIndex& edgeNode) const override;
			NumberDimensions GetNumberDimensions() const override;
			NumberCoordinates GetNumberCoordinates() const override;

			INodePtr GetNode(const NodeIndex& nodeNumber) const override;
			INodePtr GetNodeFace(const FaceIndex& faceNumber, const NodeIndex& nodeNumber) const override;
			INodePtr GetNodeEdge(const EdgeIndex& edgeNumber, const NodeIndex& nodeNumber) const override;
			const Nodes& GetNodes() const override;
			IValuePtr GetProperty(String key) const override;

			void SetTag(Tag elementTag) override;
			void SetNumberDof(NumberDof numberDof) override;
			void SetNode(const NodeIndex& nodeNumber, INodePtr node) override;
			void SetElementIndex(ElementIndex index) override;
			void SetMaterial(IMaterialPtr material) override;
			void SetProperty(IValuePtr value) override;
			
			Vector LocalCoordinates(INodePtr node) const override;
			Vector LocalCoordinates(const NodeIndex& nodeIndex) const override;

			Vector GlobalCoordinates(INodePtr node) const override;
			Vector GlobalCoordinates(const NodeIndex& nodeIndex) const override;
			Vector GlobalCoordinates(const Vector& local) const override;

			Scalar Size() const override;

			bool IsUsed(INodePtr node) const override;
			bool IsMapped() const override;
			bool IsCacheable() const override;
			bool IsIntegrable() const override;

			void InitializeCache() override;
			void IntegralWeakFormElement(IWeakFormElementPtr weakForm, Matrix& output) const override;
			void IntegralWeakFormLoad(IWeakFormLoadPtr weakForm, ILoadPtr load, Vector& output) const override;

			IScalar1DPtr GetStiffness() const;
			void SetStiffness(IScalar1DPtr stiffness);

			Matrix	K() const;

		protected:
			ElementSpring() = default;

			Tag		tag_{ 0 };
			Type	type_{ element_spring };
			Nodes	nodes_;
			Nodes	nodesLocal_;

			ElementIndex elementIndex_{ 0 };
			Properties properties_;

			NumberDof	numberDof_{ 1 };
			const NumberNodes numberNodes_{ 2 };
			const NumberFaces numberFaces_{ 0 };
			const NumberEdges numberEdges_{ 1 };
			const NumberNodes numberNodesFace_{ 0 };
			const NumberNodes numberNodesEdge_{ 2 };
			const NumberDimensions numberDimensions_{ dimension_beam };
			const NumberCoordinates numberCoordinates_{ 3 };
			
			static const Scalar localCoordinates_[2][3];
			static const NodeIndex lookUpTable11_[1 * 2];

			IScalar1DPtr stiffness_{ nullptr };

			Matrix K;
		};

	} //namespace elements
} //namespace dive

#endif /* DIVE_ELEMENTS_SPRING_HPP_*/
