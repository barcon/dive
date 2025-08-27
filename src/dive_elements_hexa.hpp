#ifndef DIVE_ELEMENTS_HEXA_HPP_
#define DIVE_ELEMENTS_HEXA_HPP_

#include "dive_elements.hpp"

namespace dive
{
	namespace elements
	{
		ElementHexaPtr CreateElementHexa();
		ElementHexaPtr CreateElementHexa(Tag elementTag);
		ElementHexaPtr CastToElementHexa(IElementPtr element);

		class ElementHexa : public IElementSolid, virtual public std::enable_shared_from_this<ElementHexa>
		{
		public:
			virtual ~ElementHexa() = default;

			static ElementHexaPtr Create();
			ElementHexaPtr GetPtr();
			ConstElementHexaPtr GetPtr() const;

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
			Scalar SizeMinimum() const override;
			Scalar SizeMaximum() const override;

			Vector LocalCoordinates(INodePtr node) const override;
			Vector LocalCoordinates(const NodeIndex& nodeIndex) const override;
			
			Vector GlobalCoordinates(INodePtr node) const override;
			Vector GlobalCoordinates(const NodeIndex& nodeIndex) const override;
			Vector GlobalCoordinates(const Vector& local) const override;

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
			NumberDimensions GetNumberDimensions() const override;
			NumberCoordinates GetNumberCoordinates() const override;
			Vector GetCenter() const override;
			Tag GetTag() const override;
			IMaterialPtr GetMaterial() const override;
			ElementIndex GetElementIndex() const override;
			NumberDof GetNumberDof() const override;
			IValuePtr GetProperty(String key) const override;

			void SetNumberDof(NumberDof numberDof) override;
			void SetNode(const NodeIndex& nodeNumber, INodePtr node) override;
			void SetOrder(const Order& order) override;
			void SetParametric(const Parametric& parametric) override;
			void SetIntegral(const Integral& integral) override;
			void SetTag(Tag elementTag) override;
			void SetMaterial(IMaterialPtr material) override;
			void SetElementIndex(ElementIndex index) override;
			void SetProperty(IValuePtr value) override;

			bool IsUsed(INodePtr node) const override;
			bool IsMapped() const override;
			bool IsCacheable() const override;

			void InitializeCache() override;
			void IntegralWeakFormElement(IWeakFormElementPtr weakForm, Matrix& output) const override;
			void IntegralWeakFormLoad(IWeakFormLoadPtr weakForm, ILoadPtr load, Vector& output) const override;
		protected:
			ElementHexa();

			Vector GlobalDerivatives(const Vector& local, const Dimension& dim) const;
			
			IntegralAreaHelper GetIntegralAreaHelper(FaceIndex faceIndex) const;
			IntegralEdgeHelper GetIntegralEdgeHelper(EdgeIndex edgeIndex) const;

			Tag		tag_{ 0 };
			Type	type_{ element_hexa8 };
			Nodes	nodes_;

			ElementIndex elementIndex_{ 0 };
			Properties properties_;

			NumberDof	numberDof_{ 1 };
			NumberNodes numberNodes_;
			NumberNodes numberNodesFace_;
			NumberNodes numberNodesEdge_;
			NumberNodes numberNodesParametric_;
			NumberNodes numberNodesFaceParametric_;

			const NumberFaces numberFaces_{ 6 };
			const NumberEdges numberEdges_{12};
			const NumberDimensions numberDimensions_{ dimension_solid };
			const NumberCoordinates numberCoordinates_{ coordinate_three };
			
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

			Order	order_{ order_linear };
			ShapePtr* shape_{ &linearFunctions_.Shape[0] };
			ShapePtr* shapeD_{ &linearFunctions_.ShapeD[0] };	

			Parametric	parametric_{ parametric_linear };
			ShapePtr*	param_ = { &linearFunctions_.Shape[0] };
			ShapePtr*	paramD_ = { &linearFunctions_.ShapeD[0] };

			IMaterialPtr material_{ nullptr };

			IGaussPtr gaussHexa_{ nullptr };
			IGaussPtr gaussRect_{ nullptr };
			IGaussPtr gaussLine_{ nullptr };

			Matrices cacheJ_;
			Matrices cacheInvJ_;
			Matrices cacheN_;
			Matrices cachedN_;
			Scalars	 cacheDetJ_;

			struct LinearFunctions {
				static Scalar N0(const Vector& arg);
				static Scalar N0d1(const Vector& arg);
				static Scalar N0d2(const Vector& arg);
				static Scalar N0d3(const Vector& arg);

				static Scalar N1(const Vector& arg);
				static Scalar N1d1(const Vector& arg);
				static Scalar N1d2(const Vector& arg);
				static Scalar N1d3(const Vector& arg);

				static Scalar N2(const Vector& arg);
				static Scalar N2d1(const Vector& arg);
				static Scalar N2d2(const Vector& arg);
				static Scalar N2d3(const Vector& arg);

				static Scalar N3(const Vector& arg);
				static Scalar N3d1(const Vector& arg);
				static Scalar N3d2(const Vector& arg);
				static Scalar N3d3(const Vector& arg);

				static Scalar N4(const Vector& arg);
				static Scalar N4d1(const Vector& arg);
				static Scalar N4d2(const Vector& arg);
				static Scalar N4d3(const Vector& arg);

				static Scalar N5(const Vector& arg);
				static Scalar N5d1(const Vector& arg);
				static Scalar N5d2(const Vector& arg);
				static Scalar N5d3(const Vector& arg);

				static Scalar N6(const Vector& arg);
				static Scalar N6d1(const Vector& arg);
				static Scalar N6d2(const Vector& arg);
				static Scalar N6d3(const Vector& arg);

				static Scalar N7(const Vector& arg);
				static Scalar N7d1(const Vector& arg);
				static Scalar N7d2(const Vector& arg);
				static Scalar N7d3(const Vector& arg);

				ShapePtr Shape[8] = { &N0, &N1, &N2, &N3, &N4, &N5, &N6, &N7 };
				ShapePtr ShapeD[3 * 8] = { &N0d1, &N1d1, &N2d1, &N3d1, &N4d1, &N5d1, &N6d1, &N7d1,
											&N0d2, &N1d2, &N2d2, &N3d2, &N4d2, &N5d2, &N6d2, &N7d2,
											&N0d3, &N1d3, &N2d3, &N3d3, &N4d3, &N5d3, &N6d3, &N7d3 };

				NodeIndex NodeIndexFace[6 * 4] = { NodeIndex(6), NodeIndex(5), NodeIndex(1), NodeIndex(2),
													NodeIndex(6), NodeIndex(2), NodeIndex(3), NodeIndex(7),
													NodeIndex(6), NodeIndex(7), NodeIndex(4), NodeIndex(5),
													NodeIndex(0), NodeIndex(4), NodeIndex(7), NodeIndex(3),
													NodeIndex(0), NodeIndex(1), NodeIndex(5), NodeIndex(4),
													NodeIndex(0), NodeIndex(3), NodeIndex(2), NodeIndex(1) };

				NodeIndex NodeIndexEdge[12 * 2] = { NodeIndex(0), NodeIndex(1),
													NodeIndex(1), NodeIndex(2),
													NodeIndex(2), NodeIndex(3),
													NodeIndex(3), NodeIndex(0),

													NodeIndex(4), NodeIndex(5),
													NodeIndex(5), NodeIndex(6),
													NodeIndex(6), NodeIndex(7),
													NodeIndex(7), NodeIndex(4),

													NodeIndex(0), NodeIndex(4),
													NodeIndex(1), NodeIndex(5),
													NodeIndex(2), NodeIndex(6),
													NodeIndex(3), NodeIndex(7) };
			} linearFunctions_;
			struct QuadraticFunctions {
				static Scalar N0(const Vector& arg);
				static Scalar N0d1(const Vector& arg);
				static Scalar N0d2(const Vector& arg);
				static Scalar N0d3(const Vector& arg);

				static Scalar N1(const Vector& arg);
				static Scalar N1d1(const Vector& arg);
				static Scalar N1d2(const Vector& arg);
				static Scalar N1d3(const Vector& arg);

				static Scalar N2(const Vector& arg);
				static Scalar N2d1(const Vector& arg);
				static Scalar N2d2(const Vector& arg);
				static Scalar N2d3(const Vector& arg);

				static Scalar N3(const Vector& arg);
				static Scalar N3d1(const Vector& arg);
				static Scalar N3d2(const Vector& arg);
				static Scalar N3d3(const Vector& arg);

				static Scalar N4(const Vector& arg);
				static Scalar N4d1(const Vector& arg);
				static Scalar N4d2(const Vector& arg);
				static Scalar N4d3(const Vector& arg);

				static Scalar N5(const Vector& arg);
				static Scalar N5d1(const Vector& arg);
				static Scalar N5d2(const Vector& arg);
				static Scalar N5d3(const Vector& arg);

				static Scalar N6(const Vector& arg);
				static Scalar N6d1(const Vector& arg);
				static Scalar N6d2(const Vector& arg);
				static Scalar N6d3(const Vector& arg);

				static Scalar N7(const Vector& arg);
				static Scalar N7d1(const Vector& arg);
				static Scalar N7d2(const Vector& arg);
				static Scalar N7d3(const Vector& arg);

				static Scalar N8(const Vector& arg);
				static Scalar N8d1(const Vector& arg);
				static Scalar N8d2(const Vector& arg);
				static Scalar N8d3(const Vector& arg);

				static Scalar N9(const Vector& arg);
				static Scalar N9d1(const Vector& arg);
				static Scalar N9d2(const Vector& arg);
				static Scalar N9d3(const Vector& arg);

				static Scalar N10(const Vector& arg);
				static Scalar N10d1(const Vector& arg);
				static Scalar N10d2(const Vector& arg);
				static Scalar N10d3(const Vector& arg);

				static Scalar N11(const Vector& arg);
				static Scalar N11d1(const Vector& arg);
				static Scalar N11d2(const Vector& arg);
				static Scalar N11d3(const Vector& arg);

				static Scalar N12(const Vector& arg);
				static Scalar N12d1(const Vector& arg);
				static Scalar N12d2(const Vector& arg);
				static Scalar N12d3(const Vector& arg);

				static Scalar N13(const Vector& arg);
				static Scalar N13d1(const Vector& arg);
				static Scalar N13d2(const Vector& arg);
				static Scalar N13d3(const Vector& arg);

				static Scalar N14(const Vector& arg);
				static Scalar N14d1(const Vector& arg);
				static Scalar N14d2(const Vector& arg);
				static Scalar N14d3(const Vector& arg);

				static Scalar N15(const Vector& arg);
				static Scalar N15d1(const Vector& arg);
				static Scalar N15d2(const Vector& arg);
				static Scalar N15d3(const Vector& arg);

				static Scalar N16(const Vector& arg);
				static Scalar N16d1(const Vector& arg);
				static Scalar N16d2(const Vector& arg);
				static Scalar N16d3(const Vector& arg);

				static Scalar N17(const Vector& arg);
				static Scalar N17d1(const Vector& arg);
				static Scalar N17d2(const Vector& arg);
				static Scalar N17d3(const Vector& arg);

				static Scalar N18(const Vector& arg);
				static Scalar N18d1(const Vector& arg);
				static Scalar N18d2(const Vector& arg);
				static Scalar N18d3(const Vector& arg);

				static Scalar N19(const Vector& arg);
				static Scalar N19d1(const Vector& arg);
				static Scalar N19d2(const Vector& arg);
				static Scalar N19d3(const Vector& arg);

				ShapePtr Shape[20] = { &N0, &N1, &N2, &N3, &N4, &N5, &N6, &N7, &N8, &N9, &N10, &N11, &N12, &N13, &N14, &N15, &N16, &N17, &N18, &N19 };
				ShapePtr ShapeD[3 * 20] = { &N0d1, &N1d1, &N2d1, &N3d1, &N4d1, &N5d1, &N6d1, &N7d1, &N8d1, &N9d1, &N10d1, &N11d1, &N12d1, &N13d1, &N14d1, &N15d1, &N16d1, &N17d1, &N18d1, &N19d1,
											&N0d2, &N1d2, &N2d2, &N3d2, &N4d2, &N5d2, &N6d2, &N7d2, &N8d2, &N9d2, &N10d2, &N11d2, &N12d2, &N13d2, &N14d2, &N15d2, &N16d2, &N17d2, &N18d2, &N19d2,
											&N0d3, &N1d3, &N2d3, &N3d3, &N4d3, &N5d3, &N6d3, &N7d3, &N8d3, &N9d3, &N10d3, &N11d3, &N12d3, &N13d3, &N14d3, &N15d3, &N16d3, &N17d3, &N18d3, &N19d3 };

				NodeIndex NodeIndexFace[6 * 8] = { NodeIndex(6), NodeIndex(13), NodeIndex(5), NodeIndex(17), NodeIndex(1), NodeIndex(9) , NodeIndex(2), NodeIndex(18),
													NodeIndex(6), NodeIndex(18), NodeIndex(2), NodeIndex(10), NodeIndex(3), NodeIndex(19), NodeIndex(7), NodeIndex(14),
													NodeIndex(6), NodeIndex(14), NodeIndex(7), NodeIndex(15), NodeIndex(4), NodeIndex(12), NodeIndex(5), NodeIndex(13),
													NodeIndex(0), NodeIndex(16), NodeIndex(4), NodeIndex(15), NodeIndex(7), NodeIndex(19), NodeIndex(3), NodeIndex(11),
													NodeIndex(0), NodeIndex(8) , NodeIndex(1), NodeIndex(17), NodeIndex(5), NodeIndex(12), NodeIndex(4), NodeIndex(16),
													NodeIndex(0), NodeIndex(11), NodeIndex(3), NodeIndex(10), NodeIndex(2), NodeIndex(9) , NodeIndex(1), NodeIndex(8) };

				NodeIndex NodeIndexEdge[12 * 3] = { NodeIndex(0), NodeIndex(8), NodeIndex(1),
													NodeIndex(1), NodeIndex(9), NodeIndex(2),
													NodeIndex(2), NodeIndex(10), NodeIndex(3),
													NodeIndex(3), NodeIndex(11), NodeIndex(0),

													NodeIndex(4), NodeIndex(12), NodeIndex(5),
													NodeIndex(5), NodeIndex(13), NodeIndex(6),
													NodeIndex(6), NodeIndex(14), NodeIndex(7),
													NodeIndex(7), NodeIndex(15), NodeIndex(4),

													NodeIndex(0), NodeIndex(16), NodeIndex(4),
													NodeIndex(1), NodeIndex(17), NodeIndex(5),
													NodeIndex(2), NodeIndex(18), NodeIndex(6),
													NodeIndex(3), NodeIndex(19), NodeIndex(7) };

			} quadraticFunctions_;
		};

	} //namespace elements
} //namespace dive

#endif /* DIVE_ELEMENTS_HEXA_HPP_*/
