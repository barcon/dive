#include "dive_elements_hexa.hpp"
#include "dive_timers_stationary.hpp"

namespace dive
{
	namespace elements
	{

		const Dimension		ElementHexa::dimension_{ 3 };
		const NumberFaces	ElementHexa::numberFaces_{ 6 };
		const NumberEdges	ElementHexa::numberEdges_{ 12 };

		const Scalar		ElementHexa::localCoordinates_[20][3] = {	{ -1.0, -1.0, -1.0 },
																		{  1.0, -1.0, -1.0 },
																		{  1.0,  1.0, -1.0 },
																		{ -1.0,  1.0, -1.0 },
																		{ -1.0, -1.0,  1.0 },
																		{  1.0, -1.0,  1.0 },
																		{  1.0,  1.0,  1.0 },
																		{ -1.0,  1.0,  1.0 },
																		{  0.0, -1.0, -1.0 },
																		{  1.0,  0.0, -1.0 },
																		{  0.0,  1.0, -1.0 },
																		{ -1.0,  0.0, -1.0 },
																		{  0.0, -1.0,  1.0 },
																		{  1.0,  0.0,  1.0 },
																		{  0.0,  1.0,  1.0 },
																		{ -1.0,  0.0,  1.0 },
																		{ -1.0, -1.0,  0.0 },
																		{  1.0, -1.0,  0.0 },
																		{  1.0,  1.0,  0.0 },
																		{ -1.0,  1.0,  0.0 } };

		const Index			ElementHexa::lookUpTable1_[6] = { 1, 2, 0, 1, 2, 0 };
		const Index			ElementHexa::lookUpTable2_[6] = { 2, 0, 1, 2, 0, 1 };
		const Index			ElementHexa::lookUpTable3_[6] = { 0, 1, 2, 0, 1, 2 };
		const Scalar		ElementHexa::lookUpTable4_[6] = { +1.0, +1.0, +1.0, -1.0, -1.0, -1.0 };

		const Index			ElementHexa::lookUpTable5_[12] = { 0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2 };
		const Index			ElementHexa::lookUpTable6_[12] = { 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0 };
		const Index			ElementHexa::lookUpTable7_[12] = { 2, 0, 2, 0, 2, 0, 2, 0, 1, 1, 1, 1 };
		const Scalar 		ElementHexa::lookUpTable8_[12] = {-1.0, -1.0, +1.0, -1.0,	-1.0, +1.0, +1.0, +1.0,	 -1.0, +1.0, +1.0, -1.0 };		
		const Scalar		ElementHexa::lookUpTable9_[12] = {-1.0, +1.0, -1.0, -1.0,	+1.0, +1.0, +1.0, -1.0,	 -1.0, -1.0, +1.0, +1.0 };

		ElementHexaPtr CreateElementHexa()
		{
			auto res = ElementHexa::Create();

			return res;
		}
		ElementHexaPtr CreateElementHexa(Tag elementTag)
		{
			auto res = ElementHexa::Create();

			res->SetTag(elementTag);

			return res;
		}
		ElementHexaPtr ElementHexa::Create()
		{
			class MakeSharedEnabler : public ElementHexa
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ElementHexaPtr ElementHexa::GetPtr()
		{
			return std::dynamic_pointer_cast<ElementHexa>(shared_from_this());
		}
		ConstElementHexaPtr ElementHexa::GetPtr() const
		{
			return const_cast<ElementHexa*>(this)->GetPtr();
		}
		ElementHexa::ElementHexa()
		{
			gaussElementHexa_ = quadrature::CreateGaussHexa();
			gaussRect_ = quadrature::CreateGaussRect();
			gaussLine_ = quadrature::CreateGaussLine();

			SetOrder(linear);
			SetParametric(linear);
		}
		Scalar ElementHexa::CalculateHeight()
		{
			Vector local1(3, 0.0);
			Vector local2(3, 0.0);

			Vector global1(3, 0.0);
			Vector global2(3, 0.0);

			Scalar h1{ 0.0 };
			Scalar h2{ 0.0 };
			Scalar h3{ 0.0 };

			height_ = 0.0;

			local1(0) = 1.0;
			local1(1) = 0.0;
			local1(2) = 0.0;

			local2(0) = -1.0;
			local2(1) = 0.0;
			local2(2) = 0.0;

			global1 = GlobalCoordinates(local1);
			global2 = GlobalCoordinates(local2);

			h1 = eilig::NormP2(global1 - global2);

			local1(0) = 0.0;
			local1(1) = 1.0;
			local1(2) = 0.0;

			local2(0) = 0.0;
			local2(1) = -1.0;
			local2(2) = 0.0;

			global1 = GlobalCoordinates(local1);
			global2 = GlobalCoordinates(local2);

			h2 = eilig::NormP2(global1 - global2);

			local1(0) = 0.0;
			local1(1) = 0.0;
			local1(2) = 1.0;

			local2(0) = 0.0;
			local2(1) = 0.0;
			local2(2) = -1.0;

			global1 = GlobalCoordinates(local1);
			global2 = GlobalCoordinates(local2);

			h3 = eilig::NormP2(global1 - global2);

			height_ = std::min({ h1, h2, h3 });

			return height_;
		}
		Matrix ElementHexa::u() const
		{
			Matrix res(numberDof_ * numberNodes_, 1, 0.0);

			for (NodeIndex i = 0; i < numberNodes_; ++i)
			{
				for (DofIndex j = 0; j < numberDof_; ++j)
				{
					res(i * numberDof_ + j, 0) = nodes_[i]->GetValue(j);
				}
			}

			return res;
		}
		Matrix ElementHexa::u(const Vector& local) const
		{
			Matrix res(numberDof_, 1, 0.0);

			for (NodeIndex i = 0; i < numberNodes_; ++i)
			{
				auto shape = shape_[i](local);

				for (DofIndex j = 0; j < numberDof_; ++j)
				{
					res(j, 0) += shape * nodes_[i]->GetValue(j);
				}
			}

			return res;
		}
		Matrix ElementHexa::du() const
		{
			Matrix res(dimension_, numberDof_ * numberNodes_, 0.0);

			for (Dimension k = 0; k < dimension_; ++k)
			{
				for (NodeIndex i = 0; i < numberNodes_; ++i)
				{
					auto shapeD = shapeD_[k * numberNodes_ + i](LocalCoordinates(i));

					for (NumberDof j = 0; j < numberDof_; ++j)
					{
						res(k, i * numberDof_ + j) = shapeD * nodes_[i]->GetValue(j);
					}

				}

			}

			return res;
		}
		Matrix ElementHexa::du(const Vector& local) const
		{
			Matrix res(dimension_, numberDof_, 0.0);

			for (Dimension k = 0; k < dimension_; ++k)
			{
				for (NodeIndex i = 0; i < numberNodes_; ++i)
				{
					auto shapeD = shapeD_[k * numberNodes_ + i](local);

					for (NumberDof j = 0; j < numberDof_; ++j)
					{
						res(k, j) += shapeD * nodes_[i]->GetValue(j);
					}

				}

			}

			return res;
		}
		Matrix ElementHexa::du(const Vector& local, const Dimension& dimension) const
		{
			Matrix res(1, numberDof_, 0.0);

			for (NodeIndex i = 0; i < numberNodes_; ++i)
			{
				auto shapeD = shapeD_[dimension * numberNodes_ + i](local);

				for (NumberDof j = 0; j < numberDof_; ++j)
				{
					res(0, j) += shapeD * nodes_[i]->GetValue(j);
				}

			}

			return res;
		}
		Matrix ElementHexa::J(const Vector& local) const
		{
			Matrix output(dimension_, dimension_, 0.0);

			for (Dimension i = 0; i < dimension_; ++i)
			{
				for (Dimension j = 0; j < dimension_; ++j)
				{
					for (NodeIndex k = 0; k < numberNodesParametric_; ++k)
					{
						const auto& nodePoint = nodes_[k]->GetPoint();

						output(i, j) += paramD_[i * numberNodes_ + k](local) * nodePoint(j);
					}
				}
			}

			return output;
		}
		Matrix ElementHexa::InvJ(const Vector& local) const
		{
			return eilig::Inverse(J(local));
		}
		Matrix ElementHexa::N(const Vector& local) const
		{
			Matrix res(1, numberNodes_, 0.0);

			for (NumberNodes i = 0; i < numberNodes_; ++i)
			{
				res(0, i) = shape_[i](local);
			}

			return res;
		}
		Matrix ElementHexa::dN(const Vector& local) const
		{
			Matrix res(dimension_, numberNodes_, 0.0);

			for (Dimension i = 0; i < dimension_; ++i)
			{
				for (NumberNodes j = 0; j < numberNodes_; ++j)
				{
					res(i, j) = shapeD_[i * numberNodes_ + j](local);
				}
			}

			return res;
		}
		const Matrix& ElementHexa::J(const Vector& local, CacheIndex cacheIndex) const
		{
			return cacheJ_[cacheIndex];
		}
		const Matrix& ElementHexa::InvJ(const Vector& local, CacheIndex cacheIndex) const
		{
			return cacheInvJ_[cacheIndex];
		}
		const Matrix& ElementHexa::N(const Vector& local, CacheIndex cacheIndex) const
		{
			return cacheN_[cacheIndex];
		}
		const Matrix& ElementHexa::dN(const Vector& local, CacheIndex cacheIndex) const
		{
			return cachedN_[cacheIndex];
		}
		Scalar ElementHexa::DetJ(const Vector& local) const
		{
			return eilig::Determinant(J(local));
		}
		Scalar ElementHexa::DelA(const Vector& local, const Dimension& dim1, const Dimension& dim2) const
		{
			Vector du = GlobalDerivatives(local, dim1);
			Vector dv = GlobalDerivatives(local, dim2);
			Vector dw = eilig::Cross(du, dv);

			return eilig::NormP2(dw);
		}
		Scalar ElementHexa::DelL(const Vector& local, const Dimension& dim1) const
		{
			Vector dw;

			dw = GlobalDerivatives(local, dim1);

			return eilig::NormP2(dw);
		}
		Scalar ElementHexa::DetJ(const Vector& local, CacheIndex cacheIndex) const
		{
			return cacheDetJ_[cacheIndex];
		}
		Scalar ElementHexa::Volume() const
		{
			Scalar res{ 0. };

			std::size_t counter{ 0 };

			const auto& points = gaussElementHexa_->GetPoints();
			const auto& weights = gaussElementHexa_->GetWeights();
			const auto& quadrature = gaussElementHexa_->GetQuadrature();

			for (quadrature::Quadrature i = 0; i < quadrature; ++i)
			{
				for (quadrature::Quadrature j = 0; j < quadrature; ++j)
				{
					for (quadrature::Quadrature k = 0; k < quadrature; ++k)
					{
						res += weights[counter] * DetJ(points[counter]);

						++counter;
					}
				}
			}

			return res;
		}
		Scalar ElementHexa::Area(const FaceIndex& face) const
		{
			Scalar res{ 0. };

			std::size_t counter{ 0 };
			Vector point(dimension_);

			const auto& points = gaussRect_->GetPoints();
			const auto& weights = gaussRect_->GetWeights();
			const auto& quadrature = gaussRect_->GetQuadrature();

			auto dim1 = lookUpTable1_[face];
			auto dim2 = lookUpTable2_[face];
			auto dim3 = lookUpTable3_[face];

			point(dim3) = lookUpTable4_[face];

			for (quadrature::Quadrature i = 0; i < quadrature; ++i)
			{
				for (quadrature::Quadrature j = 0; j < quadrature; ++j)
				{
					point(dim1) = points[counter](0);
					point(dim2) = points[counter](1);
					res += weights[counter] * DelA(point, dim1, dim2);

					++counter;
				}
			}

			return res;
		}
		Scalar ElementHexa::Length(const EdgeIndex& edge) const
		{
			Scalar res{ 0. };

			std::size_t counter = 0;

			Dimension dim1;
			Dimension dim2;
			Dimension dim3;
			Vector point(dimension_);

			const auto& points = gaussLine_->GetPoints();
			const auto& weights = gaussLine_->GetWeights();
			const auto& quadrature = gaussLine_->GetQuadrature();

			dim1 = lookUpTable5_[edge];
			dim2 = lookUpTable6_[edge];
			dim3 = lookUpTable7_[edge];

			point(dim2) = lookUpTable8_[edge];
			point(dim3) = lookUpTable9_[edge];

			for (quadrature::Quadrature i = 0; i < quadrature; ++i)
			{
				point(dim1) = points[counter](0);
				res += weights[counter] * DelL(point, dim1);

				++counter;
			}

			return res;
		}
		Vector ElementHexa::LocalCoordinates(INodePtr node) const
		{
			return LocalCoordinates(GetNodeIndex(node));
		}
		Vector ElementHexa::LocalCoordinates(const NodeIndex& nodeIndex) const
		{
			Vector local(3, 0.0);

			local(0) = localCoordinates_[nodeIndex][0];
			local(1) = localCoordinates_[nodeIndex][1];
			local(2) = localCoordinates_[nodeIndex][2];

			return local;
		}
		Vector ElementHexa::GlobalCoordinates(INodePtr node) const
		{
			return GlobalCoordinates(GetNodeIndex(node));
		}
		Vector ElementHexa::GlobalCoordinates(const NodeIndex& nodeIndex) const
		{
			return nodes_[nodeIndex]->GetPoint();
		}
		Vector ElementHexa::GlobalCoordinates(const Vector& local) const
		{
			Vector output(dimension_);

			for (Dimension i = 0; i < dimension_; ++i)
			{
				for (NodeIndex j = 0; j < numberNodesParametric_; ++j)
				{
					output(i) += param_[j](local) * nodes_[j]->GetPoint()(i);
				}
			}

			return output;
		}
		Vector ElementHexa::GlobalDerivatives(const Vector& local, const Dimension& dim) const
		{
			Vector output(dimension_);

			for (Dimension i = 0; i < dimension_; ++i)
			{
				for (NodeIndex j = 0; j < numberNodesParametric_; ++j)
				{

					output(i) += paramD_[dim * numberNodes_ + j](local) * nodes_[j]->GetPoint()(i);
				}
			}

			return output;
		}
		INodePtr ElementHexa::GetNodeFace(const FaceIndex& faceIndex, const NodeIndex& nodeIndex) const
		{
			return GetNode(lookUpTable10_[numberNodesFace_ * faceIndex + nodeIndex]);
		}
		INodePtr ElementHexa::GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const
		{
			return GetNode(lookUpTable11_[numberNodesEdge_ * edgeIndex + nodeIndex]);
		}
		INodePtr ElementHexa::GetNode(const NodeIndex& nodeIndex) const
		{
			return nodes_[nodeIndex];
		}
		const Nodes& ElementHexa::GetNodes() const
		{
			return nodes_;
		}
		Order ElementHexa::GetOrder() const
		{
			return order_;
		}
		Parametric ElementHexa::GetParametric() const
		{
			return parametric_;
		}
		Type ElementHexa::GetType() const
		{
			return type_;
		}
		NodeIndex ElementHexa::GetNodeIndex(const INodePtr node) const
		{
			for (NodeIndex i = 0; i < numberNodes_; ++i)
			{
				if (nodes_[i] == node)
				{
					return i;
				}
			}

			return nodeIndexInvalid;
		}
		NumberNodes ElementHexa::GetNumberNodesFace(const FaceIndex& faceIndex) const
		{
			return numberNodesFace_;
		}
		NumberNodes ElementHexa::GetNumberNodesEdge(const EdgeIndex& edgeNode) const
		{
			return numberNodesEdge_;
		}
		NumberNodes ElementHexa::GetNumberNodes() const
		{
			return numberNodes_;
		}
		NumberFaces ElementHexa::GetNumberFaces() const
		{
			return numberFaces_;
		}
		NumberEdges ElementHexa::GetNumberEdges() const
		{
			return numberEdges_;
		}
		Dimension ElementHexa::GetDimension() const
		{
			return dimension_;
		}
		Vector ElementHexa::GetCenter() const
		{
			Vector center(3, 0.0);

			return 	GlobalCoordinates(center);;
		}
		Tag ElementHexa::GetTag() const
		{
			return tag_;
		}
		IMaterialPtr ElementHexa::GetMaterial() const
		{
			return material_;
		}
		ElementIndex ElementHexa::GetElementIndex() const
		{
			return elementIndex_;
		}
		Scalar ElementHexa::GetHeight() const
		{
			return height_;
		}
		NumberDof ElementHexa::GetNumberDof() const
		{
			return numberDof_;
		}
		IGaussPtr ElementHexa::IntegralVolume() const
		{
			return gaussElementHexa_;
		}
		IGaussPtr ElementHexa::IntegralArea() const
		{
			return gaussRect_;
		}
		IGaussPtr ElementHexa::IntegralEdge() const
		{
			return gaussLine_;
		}
		IntegralAreaHelper ElementHexa::GetIntegralAreaHelper(FaceIndex faceIndex) const
		{
			IntegralAreaHelper res;

			res.dim1 = lookUpTable1_[faceIndex];
			res.dim2 = lookUpTable2_[faceIndex];
			res.dim3 = lookUpTable3_[faceIndex];
			res.coord3 = lookUpTable4_[faceIndex];

			return res;
		}
		IntegralEdgeHelper ElementHexa::GetIntegralEdgeHelper(EdgeIndex edgeIndex) const
		{
			IntegralEdgeHelper res;

			res.dim1 = lookUpTable5_[edgeIndex];
			res.dim2 = lookUpTable6_[edgeIndex];
			res.dim3 = lookUpTable7_[edgeIndex];
			res.coord2 = lookUpTable8_[edgeIndex];
			res.coord3 = lookUpTable9_[edgeIndex];

			return res;
		}
		void ElementHexa::SetNumberDof(NumberDof numberDof)
		{
			numberDof_ = numberDof;

			for (NodeIndex i = 0; i < numberNodes_; ++i)
			{
				if (nodes_[i] != nullptr)
				{
					nodes_[i]->SetNumberDof(numberDof_);
				}
			}
		}
		void ElementHexa::SetNode(const NodeIndex& nodeIndex, INodePtr node)
		{
			if (node == nullptr)
			{
				nodes_[nodeIndex]->RemoveElement(this->GetPtr());
				nodes_[nodeIndex] = nullptr;

				return;
			}

			if (nodes_[nodeIndex] != nullptr)
			{
				nodes_[nodeIndex]->RemoveElement(this->GetPtr());
			}

			nodes_[nodeIndex] = node;
			nodes_[nodeIndex]->SetNumberDof(numberDof_);
			nodes_[nodeIndex]->AddElement(this->GetPtr());
		}
		void ElementHexa::SetOrder(const Order& order)
		{
			if (order == linear)
			{
				order_ = linear;

				shape_ = &linearFunctions_.Shape[0];
				shapeD_ = &linearFunctions_.ShapeD[0];
				lookUpTable10_ = &linearFunctions_.NodeIndexFace[0];
				lookUpTable11_ = &linearFunctions_.NodeIndexEdge[0];

				numberNodes_ = 8;
				numberNodesFace_ = 4;
				numberNodesEdge_ = 2;
				nodes_.resize(numberNodes_);
				type_ = element_hexa8;

				gaussElementHexa_->SetQuadrature(2);
				gaussRect_->SetQuadrature(2);
				gaussLine_->SetQuadrature(2);
			}
			else if (order == quadratic)
			{
				order_ = quadratic;

				shape_ = &quadraticFunctions_.Shape[0];
				shapeD_ = &quadraticFunctions_.ShapeD[0];
				lookUpTable10_ = &quadraticFunctions_.NodeIndexFace[0];
				lookUpTable11_ = &quadraticFunctions_.NodeIndexEdge[0];

				numberNodes_ = 20;
				numberNodesFace_ = 8;
				numberNodesEdge_ = 3;
				nodes_.resize(numberNodes_);
				type_ = element_hexa20;

				gaussElementHexa_->SetQuadrature(3);
				gaussRect_->SetQuadrature(3);
				gaussLine_->SetQuadrature(3);
			}
		}
		void ElementHexa::SetParametric(const Parametric& parametric)
		{
			if (parametric == linear)
			{
				parametric_ = linear;

				param_ = &linearFunctions_.Shape[0];
				paramD_ = &linearFunctions_.ShapeD[0];

				numberNodesParametric_ = 8;
				numberNodesFaceParametric_ = 4;
			}
			else if (parametric == quadratic)
			{
				parametric_ = quadratic;

				param_ = &quadraticFunctions_.Shape[0];
				paramD_ = &quadraticFunctions_.ShapeD[0];

				numberNodesParametric_ = 20;
				numberNodesFaceParametric_ = 8;
			}
		}
		void ElementHexa::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ElementHexa::SetMaterial(IMaterialPtr material)
		{
			material_ = material;
		}
		void ElementHexa::SetElementIndex(ElementIndex index)
		{
			elementIndex_ = index;
		}
		bool ElementHexa::IsUsed(INodePtr node) const
		{
			auto it = std::find_if(nodes_.begin(), nodes_.end(), [&](INodePtr ptr) -> bool
				{
					return ptr == node;
				});

			if (it == nodes_.end())
			{
				return true;
			}

			return false;
		}
		void ElementHexa::InitializeCache()
		{
			const auto& gauss = IntegralVolume();
			const auto& points = gauss->GetPoints();
			const auto& counter = gauss->GetCounter();

			cacheJ_.resize(counter);
			cacheN_.resize(counter);
			cachedN_.resize(counter);
			cacheInvJ_.resize(counter);
			cacheDetJ_.resize(counter);

			for (quadrature::Counter i = 0; i < counter; ++i)
			{
				cacheJ_[i] = J(points[i]);
				cacheN_[i] = N(points[i]);
				cachedN_[i] = dN(points[i]);
				cacheInvJ_[i] = InvJ(points[i]);
				cacheDetJ_[i] = DetJ(points[i]);
			}
		}
		Scalar ElementHexa::LinearFunctions::N0(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N0d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N0d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N0d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementHexa::LinearFunctions::N1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N1d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N1d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N1d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementHexa::LinearFunctions::N2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N2d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N2d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N2d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementHexa::LinearFunctions::N3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N3d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N3d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N3d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementHexa::LinearFunctions::N4(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N4d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N4d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N4d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementHexa::LinearFunctions::N5(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N5d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N5d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N5d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementHexa::LinearFunctions::N6(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N6d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N6d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N6d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementHexa::LinearFunctions::N7(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N7d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N7d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::LinearFunctions::N7d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N0(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2)) * (-arg(0) - arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N0d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2)) - 1.0 / 8.0 * (1.0 - arg(1)) * (1.0 - arg(2)) * (-arg(0) - arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N0d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2)) - 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(2)) * (-arg(0) - arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N0d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2)) - 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (-arg(0) - arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2)) * (arg(0) - arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N1d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2)) + 1.0 / 8.0 * (1.0 - arg(1)) * (1.0 - arg(2)) * (arg(0) - arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N1d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2)) - 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(2)) * (arg(0) - arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N1d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2)) - 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (arg(0) - arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2)) * (arg(0) + arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N2d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2)) + 1.0 / 8.0 * (1.0 + arg(1)) * (1.0 - arg(2)) * (arg(0) + arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N2d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2)) + 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(2)) * (arg(0) + arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N2d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2)) - 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (arg(0) + arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2)) * (-arg(0) + arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N3d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2)) - 1.0 / 8.0 * (1.0 + arg(1)) * (1.0 - arg(2)) * (-arg(0) + arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N3d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2)) + 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(2)) * (-arg(0) + arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N3d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2)) - 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (-arg(0) + arg(1) - arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N4(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2)) * (-arg(0) - arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N4d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2)) - 1.0 / 8.0 * (1.0 - arg(1)) * (1.0 + arg(2)) * (-arg(0) - arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N4d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2)) - 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(2)) * (-arg(0) - arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N4d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2)) + 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (-arg(0) - arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N5(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2)) * (arg(0) - arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N5d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2)) + 1.0 / 8.0 * (1.0 - arg(1)) * (1.0 + arg(2)) * (arg(0) - arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N5d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2)) - 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(2)) * (arg(0) - arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N5d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2)) + 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (arg(0) - arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N6(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2)) * (arg(0) + arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N6d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2)) + 1.0 / 8.0 * (1.0 + arg(1)) * (1.0 + arg(2)) * (arg(0) + arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N6d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2)) + 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(2)) * (arg(0) + arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N6d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2)) + 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (arg(0) + arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N7(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2)) * (-arg(0) + arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N7d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2)) - 1.0 / 8.0 * (1.0 + arg(1)) * (1.0 + arg(2)) * (-arg(0) + arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N7d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2)) + 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(2)) * (-arg(0) + arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N7d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2)) + 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (-arg(0) + arg(1) + arg(2) - 2.0);
		}
		Scalar ElementHexa::QuadraticFunctions::N8(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N8d1(const Vector& arg)
		{
			return 1.0 / 4.0 * (-2.0 * arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N8d2(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N8d3(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N9(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (1.0 - arg(1) * arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N9d1(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(1) * arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N9d2(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (-2.0 * arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N9d3(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 + arg(0)) * (1.0 - arg(1) * arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N10(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N10d1(const Vector& arg)
		{
			return 1.0 / 4.0 * (-2.0 * arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N10d2(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N10d3(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N11(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (1.0 - arg(1) * arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N11d1(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(1) * arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N11d2(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (-2.0 * arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N11d3(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(0)) * (1.0 - arg(1) * arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N12(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N12d1(const Vector& arg)
		{
			return 1.0 / 4.0 * (-2.0 * arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N12d2(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N12d3(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N13(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (1.0 - arg(1) * arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N13d1(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(1) * arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N13d2(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (-2.0 * arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N13d3(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (1.0 - arg(1) * arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N14(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N14d1(const Vector& arg)
		{
			return 1.0 / 4.0 * (-2.0 * arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N14d2(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N14d3(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0) * arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N15(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (1.0 - arg(1) * arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N15d1(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(1) * arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N15d2(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (-2.0 * arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N15d3(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (1.0 - arg(1) * arg(1));
		}
		Scalar ElementHexa::QuadraticFunctions::N16(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N16d1(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(1)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N16d2(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 - arg(0)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N16d3(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (-2.0 * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N17(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N17d1(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(1)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N17d2(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 + arg(0)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N17d3(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (-2.0 * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N18(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N18d1(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(1)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N18d2(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N18d3(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (-2.0 * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N19(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N19d1(const Vector& arg)
		{
			return -1.0 / 4.0 * (1.0 + arg(1)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N19d2(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (1.0 - arg(2) * arg(2));
		}
		Scalar ElementHexa::QuadraticFunctions::N19d3(const Vector& arg)
		{
			return 1.0 / 4.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (-2.0 * arg(2));
		}

	} //namespace elements
} //namespace dive