#ifndef DIVE_ELEMENTS_COMBINED_HPP_
#define DIVE_ELEMENTS_COMBINED_HPP_

#include "dive_elements.hpp"

namespace dive
{
	namespace elements
	{
		ElementCombinedPtr CreateElementCombined();
		ElementCombinedPtr CreateElementCombined(Tag elementTag);
		ElementCombinedPtr CreateElementCombined(Tag elementTag, IScalarPtr stiffness, IScalarPtr damping);
		ElementCombinedPtr CastToElementCombined(IElementPtr element);

		class ElementCombined : public IElement, virtual public std::enable_shared_from_this<ElementCombined>
		{
		public:
			virtual ~ElementCombined() = default;

			static ElementCombinedPtr Create();
			ElementCombinedPtr GetPtr();
			ConstElementCombinedPtr GetPtr() const;

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
			bool GetNonlinear() const override;

			void SetTag(Tag elementTag) override;
			void SetNumberDof(NumberDof numberDof) override;
			void SetNode(const NodeIndex& nodeNumber, INodePtr node) override;
			void SetElementIndex(ElementIndex index) override;
			void SetMaterial(IMaterialPtr material) override;
			void SetProperty(IValuePtr value) override;
			void SetNonlinear(bool nonlinear) override;
			
			Vector LocalCoordinates(INodePtr node) const override;
			Vector LocalCoordinates(const NodeIndex& nodeIndex) const override;

			Vector GlobalCoordinates(INodePtr node) const override;
			Vector GlobalCoordinates(const NodeIndex& nodeIndex) const override;
			Vector GlobalCoordinates(const Vector& local) const override;

			Scalar SizeMinimum() const override;
			Scalar SizeMaximum() const override;

			bool IsUsed(INodePtr node) const override;
			bool IsMapped() const override;
			bool IsCacheable() const override;

			void InitializeCache() override;

			IScalarPtr GetStiffness() const;
			IScalarPtr GetDamping() const;

			void SetStiffness(IScalarPtr stiffness);
			void SetDamping(IScalarPtr damping);

			void Stiffness(Matrix& output) const;
			void Damping(Matrix& output) const;

		protected:
			ElementCombined();

			Vector GetGlobalVector0() const;
			Vector GetGlobalVector1() const;
			Vector GetGlobalVector2() const;

			Vector GetLocalVector0() const;
			Vector GetLocalVector1() const;
			Vector GetLocalVector2() const;

			Matrix FormMatrix_Canonical() const;

			Tag		tag_{ 0 };
			Type	type_{ element_combined };
			Nodes	nodes_;

			ElementIndex elementIndex_{ 0 };
			Properties properties_;

			NumberDof numberDof_{ 1 };
			const NumberNodes numberNodes_{ 2 };
			const NumberFaces numberFaces_{ 0 };
			const NumberEdges numberEdges_{ 1 };
			const NumberNodes numberNodesFace_{ 0 };
			const NumberNodes numberNodesEdge_{ 2 };
			const NumberDimensions numberDimensions_{ dimension_beam };
			const NumberCoordinates numberCoordinates_{ coordinate_three };
			bool nonlinear_{ false };
			
			static const Scalar localCoordinates_[2][3];
			static const NodeIndex lookUpTable11_[1 * 2];

			IScalarPtr stiffness_{ nullptr };
			IScalarPtr damping_{ nullptr };

			Matrix I_;
		};

	} //namespace elements
} //namespace dive

#endif /* DIVE_ELEMENTS_COMBINED_HPP_*/
