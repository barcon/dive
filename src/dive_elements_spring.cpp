#include "dive_elements_spring.hpp"
#include "dive_weakforms.hpp"

namespace dive
{
	namespace elements
	{
		const Scalar		ElementSpring::localCoordinates_[2][3] = {	{ -1.0,  0.0,  0.0 },
																		{  1.0,  0.0,  0.0 } };

		const Index			ElementSpring::lookUpTable1_[6] = { 1, 2, 0, 1, 2, 0 };
		const Index			ElementSpring::lookUpTable2_[6] = { 2, 0, 1, 2, 0, 1 };
		const Index			ElementSpring::lookUpTable3_[6] = { 0, 1, 2, 0, 1, 2 };
		const Scalar		ElementSpring::lookUpTable4_[6] = { +1.0, +1.0, +1.0, -1.0, -1.0, -1.0 };

		const Index			ElementSpring::lookUpTable5_[12] = { 0, 1, 0, 1, 0, 1, 0, 1, 2, 2, 2, 2 };
		const Index			ElementSpring::lookUpTable6_[12] = { 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0 };
		const Index			ElementSpring::lookUpTable7_[12] = { 2, 0, 2, 0, 2, 0, 2, 0, 1, 1, 1, 1 };
		const Scalar 		ElementSpring::lookUpTable8_[12] = {-1.0, -1.0, +1.0, -1.0,	-1.0, +1.0, +1.0, +1.0,	 -1.0, +1.0, +1.0, -1.0 };		
		const Scalar		ElementSpring::lookUpTable9_[12] = {-1.0, +1.0, -1.0, -1.0,	+1.0, +1.0, +1.0, -1.0,	 -1.0, -1.0, +1.0, +1.0 };

		ElementSpringPtr CreateElementSpring()
		{
			auto res = ElementSpring::Create();

			return res;
		}
		ElementSpringPtr CreateElementSpring(Tag elementTag)
		{
			auto res = ElementSpring::Create();

			res->SetTag(elementTag);

			return res;
		}
		ElementSpringPtr ElementSpring::Create()
		{
			class MakeSharedEnabler : public ElementSpring
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ElementSpringPtr ElementSpring::GetPtr()
		{
			return std::dynamic_pointer_cast<ElementSpring>(shared_from_this());
		}
		ConstElementSpringPtr ElementSpring::GetPtr() const
		{
			return const_cast<ElementSpring*>(this)->GetPtr();
		}
		ElementSpring::ElementSpring()
		{
			gaussLine_ = quadrature::CreateGaussLine();

			SetOrder(order_linear);
			SetParametric(parametric_linear);
		}
		Matrix ElementSpring::u() const
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
		Matrix ElementSpring::u(const Vector& local) const
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
		Matrix ElementSpring::du() const
		{
			Matrix res(numberDimensions_, numberDof_ * numberNodes_, 0.0);

			for (Dimension k = 0; k < numberDimensions_; ++k)
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
		Matrix ElementSpring::du(const Vector& local) const
		{
			Matrix res(numberDimensions_, numberDof_, 0.0);

			for (Dimension k = 0; k < numberDimensions_; ++k)
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
		Matrix ElementSpring::du(const Vector& local, const Dimension& dimension) const
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
		Matrix ElementSpring::J(const Vector& local) const
		{
			Matrix output(numberDimensions_, numberDimensions_, 0.0);

			for (Dimension i = 0; i < numberDimensions_; ++i)
			{
				for (Dimension j = 0; j < numberDimensions_; ++j)
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
		Matrix ElementSpring::InvJ(const Vector& local) const
		{
			return eilig::Inverse(J(local));
		}
		Matrix ElementSpring::N(const Vector& local) const
		{
			Matrix res(1, numberNodes_, 0.0);

			for (NumberNodes i = 0; i < numberNodes_; ++i)
			{
				res(0, i) = shape_[i](local);
			}

			return res;
		}
		Matrix ElementSpring::dN(const Vector& local) const
		{
			Matrix res(numberDimensions_, numberNodes_, 0.0);

			for (Dimension i = 0; i < numberDimensions_; ++i)
			{
				for (NumberNodes j = 0; j < numberNodes_; ++j)
				{
					res(i, j) = shapeD_[i * numberNodes_ + j](local);
				}
			}

			return res;
		}
		const Matrix& ElementSpring::J(const Vector& local, CacheIndex cacheIndex) const
		{
			return cacheJ_[cacheIndex];
		}
		const Matrix& ElementSpring::InvJ(const Vector& local, CacheIndex cacheIndex) const
		{
			return cacheInvJ_[cacheIndex];
		}
		const Matrix& ElementSpring::N(const Vector& local, CacheIndex cacheIndex) const
		{
			return cacheN_[cacheIndex];
		}
		const Matrix& ElementSpring::dN(const Vector& local, CacheIndex cacheIndex) const
		{
			return cachedN_[cacheIndex];
		}
		Scalar ElementSpring::DetJ(const Vector& local) const
		{
			return eilig::Determinant(J(local));
		}
		Scalar ElementSpring::DetJ(const Vector& local, CacheIndex cacheIndex) const
		{
			return cacheDetJ_[cacheIndex];
		}
		Scalar ElementSpring::DelA(const Vector& local, const Dimension& dim1, const Dimension& dim2) const
		{
			Vector du = GlobalDerivatives(local, dim1);
			Vector dv = GlobalDerivatives(local, dim2);
			Vector dw = eilig::Cross(du, dv);

			return eilig::NormP2(dw);
		}
		Scalar ElementSpring::DelL(const Vector& local, const Dimension& dim1) const
		{
			Vector dw;

			dw = GlobalDerivatives(local, dim1);

			return eilig::NormP2(dw);
		}
		Scalar ElementSpring::Size() const
		{
			Vector local1(3, 0.0);
			Vector local2(3, 0.0);

			Vector global1(3, 0.0);
			Vector global2(3, 0.0);

			Scalar h1{ 0.0 };
			Scalar h2{ 0.0 };
			Scalar h3{ 0.0 };

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

			return std::min({ h1, h2, h3 });
		}
		Vector ElementSpring::LocalCoordinates(INodePtr node) const
		{
			return LocalCoordinates(GetNodeIndex(node));
		}
		Vector ElementSpring::LocalCoordinates(const NodeIndex& nodeIndex) const
		{
			Vector local(3);

			local(0) = localCoordinates_[nodeIndex][0];
			local(1) = localCoordinates_[nodeIndex][1];
			local(2) = localCoordinates_[nodeIndex][2];

			return local;
		}
		Vector ElementSpring::GlobalCoordinates(INodePtr node) const
		{
			return GlobalCoordinates(GetNodeIndex(node));
		}
		Vector ElementSpring::GlobalCoordinates(const NodeIndex& nodeIndex) const
		{
			return nodes_[nodeIndex]->GetPoint();
		}
		Vector ElementSpring::GlobalCoordinates(const Vector& local) const
		{
			Vector output(numberDimensions_);

			for (Dimension i = 0; i < numberDimensions_; ++i)
			{
				for (NodeIndex j = 0; j < numberNodesParametric_; ++j)
				{
					output(i) += param_[j](local) * nodes_[j]->GetPoint()(i);
				}
			}

			return output;
		}
		Vector ElementSpring::GlobalDerivatives(const Vector& local, const Dimension& dim) const
		{
			Vector output(numberDimensions_);

			for (Dimension i = 0; i < numberDimensions_; ++i)
			{
				for (NodeIndex j = 0; j < numberNodesParametric_; ++j)
				{

					output(i) += paramD_[dim * numberNodes_ + j](local) * nodes_[j]->GetPoint()(i);
				}
			}

			return output;
		}
		INodePtr ElementSpring::GetNodeFace(const FaceIndex& faceIndex, const NodeIndex& nodeIndex) const
		{
			return GetNode(lookUpTable10_[numberNodesFace_ * faceIndex + nodeIndex]);
		}
		INodePtr ElementSpring::GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const
		{
			return GetNode(lookUpTable11_[numberNodesEdge_ * edgeIndex + nodeIndex]);
		}
		INodePtr ElementSpring::GetNode(const NodeIndex& nodeIndex) const
		{
			return nodes_[nodeIndex];
		}
		const Nodes& ElementSpring::GetNodes() const
		{
			return nodes_;
		}
		Order ElementSpring::GetOrder() const
		{
			return order_;
		}
		Parametric ElementSpring::GetParametric() const
		{
			return parametric_;
		}
		Type ElementSpring::GetType() const
		{
			return type_;
		}
		NodeIndex ElementSpring::GetNodeIndex(const INodePtr node) const
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
		NumberNodes ElementSpring::GetNumberNodesFace(const FaceIndex& faceIndex) const
		{
			return numberNodesFace_;
		}
		NumberNodes ElementSpring::GetNumberNodesEdge(const EdgeIndex& edgeNode) const
		{
			return numberNodesEdge_;
		}
		NumberNodes ElementSpring::GetNumberNodes() const
		{
			return numberNodes_;
		}
		NumberFaces ElementSpring::GetNumberFaces() const
		{
			return numberFaces_;
		}
		NumberEdges ElementSpring::GetNumberEdges() const
		{
			return numberEdges_;
		}
		NumberDimensions ElementSpring::GetNumberDimensions() const
		{
			return numberDimensions_;
		}
		Vector ElementSpring::GetCenter() const
		{
			Vector center(3, 0.0);

			return 	GlobalCoordinates(center);;
		}
		Tag ElementSpring::GetTag() const
		{
			return tag_;
		}
		IMaterialPtr ElementSpring::GetMaterial() const
		{
			return material_;
		}
		ElementIndex ElementSpring::GetElementIndex() const
		{
			return elementIndex_;
		}
		NumberDof ElementSpring::GetNumberDof() const
		{
			return numberDof_;
		}
		IValuePtr ElementSpring::GetProperty(String key) const
		{
			auto property = properties_.find(key);

			if (property == properties_.end())
			{
				return nullptr;
			}

			return property->second;
		}
		IntegralAreaHelper ElementSpring::GetIntegralAreaHelper(FaceIndex faceIndex) const
		{
			IntegralAreaHelper res;

			res.dim1 = lookUpTable1_[faceIndex];
			res.dim2 = lookUpTable2_[faceIndex];
			res.dim3 = lookUpTable3_[faceIndex];
			res.coord3 = lookUpTable4_[faceIndex];

			return res;
		}
		IntegralEdgeHelper ElementSpring::GetIntegralEdgeHelper(EdgeIndex edgeIndex) const
		{
			IntegralEdgeHelper res;

			res.dim1 = lookUpTable5_[edgeIndex];
			res.dim2 = lookUpTable6_[edgeIndex];
			res.dim3 = lookUpTable7_[edgeIndex];
			res.coord2 = lookUpTable8_[edgeIndex];
			res.coord3 = lookUpTable9_[edgeIndex];

			return res;
		}
		void ElementSpring::SetNumberDof(NumberDof numberDof)
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
		void ElementSpring::SetNode(const NodeIndex& nodeIndex, INodePtr node)
		{
			if (node == nullptr)
			{
				auto& elements = nodes_[nodeIndex]->GetConnectivity().elements;
				auto it = std::find(elements.begin(), elements.end(), this->GetPtr());

				if (it != elements.end())
				{
					elements.erase(it);
				}

				nodes_[nodeIndex] = nullptr;

				return;
			}

			if (nodes_[nodeIndex] != nullptr)
			{
				auto& elements = nodes_[nodeIndex]->GetConnectivity().elements;
				auto it = std::find(elements.begin(), elements.end(), this->GetPtr());

				if (it != elements.end())
				{
					elements.erase(it);
				}
			}

			nodes_[nodeIndex] = node;
			nodes_[nodeIndex]->SetNumberDof(numberDof_);

			auto& elements = nodes_[nodeIndex]->GetConnectivity().elements;
			auto it = std::find(elements.begin(), elements.end(), this->GetPtr());

			if (it == elements.end())
			{
				elements.push_back(this->GetPtr());
			}
		}
		void ElementSpring::SetOrder(const Order& order)
		{
			if (order == order_linear)
			{
				order_ = order_linear;

				shape_ = &linearFunctions_.Shape[0];
				shapeD_ = &linearFunctions_.ShapeD[0];
				lookUpTable10_ = &linearFunctions_.NodeIndexFace[0];
				lookUpTable11_ = &linearFunctions_.NodeIndexEdge[0];

				numberNodes_ = 8;
				numberNodesFace_ = 4;
				numberNodesEdge_ = 2;
				nodes_.resize(numberNodes_);
				type_ = element_hexa8;

				gaussSpring_->SetQuadrature(2);
				gaussRect_->SetQuadrature(2);
				gaussLine_->SetQuadrature(2);
			}
			else if (order == order_quadratic)
			{
				order_ = order_quadratic;

				shape_ = &quadraticFunctions_.Shape[0];
				shapeD_ = &quadraticFunctions_.ShapeD[0];
				lookUpTable10_ = &quadraticFunctions_.NodeIndexFace[0];
				lookUpTable11_ = &quadraticFunctions_.NodeIndexEdge[0];

				numberNodes_ = 20;
				numberNodesFace_ = 8;
				numberNodesEdge_ = 3;
				nodes_.resize(numberNodes_);
				type_ = element_hexa20;

				gaussSpring_->SetQuadrature(3);
				gaussRect_->SetQuadrature(3);
				gaussLine_->SetQuadrature(3);
			}
		}
		void ElementSpring::SetParametric(const Parametric& parametric)
		{
			if (parametric == parametric_linear)
			{
				parametric_ = parametric_linear;

				param_ = &linearFunctions_.Shape[0];
				paramD_ = &linearFunctions_.ShapeD[0];

				numberNodesParametric_ = 8;
				numberNodesFaceParametric_ = 4;
			}
			else if (parametric == parametric_quadratic)
			{
				parametric_ = parametric_quadratic;

				param_ = &quadraticFunctions_.Shape[0];
				paramD_ = &quadraticFunctions_.ShapeD[0];

				numberNodesParametric_ = 20;
				numberNodesFaceParametric_ = 8;
			}
		}
		void ElementSpring::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ElementSpring::SetMaterial(IMaterialPtr material)
		{
			material_ = material;
		}
		void ElementSpring::SetElementIndex(ElementIndex index)
		{
			elementIndex_ = index;
		}
		void ElementSpring::SetProperty(IValuePtr value)
		{
			properties_.insert({ value->GetKey(), value });
		}
		bool ElementSpring::IsUsed(INodePtr node) const
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
		bool ElementSpring::IsMapped() const
		{
			return true;
		}
		bool ElementSpring::IsCacheable() const
		{
			return false;
		}
		bool ElementSpring::IsIntegrable() const
		{
			return true;
		}
		void ElementSpring::IntegralWeakFormElement(IWeakFormElementPtr weakForm, Matrix& output) const
		{
			Matrix local;

			const auto& points = gaussSpring_->GetPoints();
			const auto& weights = gaussSpring_->GetWeights();
			const auto& counter = gaussSpring_->GetCounter();

			auto element = std::make_shared<ElementSpring>(*this);

			weakForm->WeakFormulation(element, 0, points[0], local);
			output = weights[0] * DetJ(points[0], 0) * local;

			for (quadrature::Counter i = 1; i < counter; ++i)
			{
				weakForm->WeakFormulation(element, i, points[i], local);

				output = output + weights[i] * DetJ(points[i], i) * local;
			}

		}
		void ElementSpring::IntegralWeakFormLoad(IWeakFormLoadPtr weakForm, ILoadPtr load, Vector& output) const
		{
			Vector local;
			Vector point(numberDimensions_);

			if (load->GetType() == loads::load_distributedVolume)
			{
				const auto& force = std::static_pointer_cast<loads::LoadDistributedVolume>(load);

				const auto& points = gaussSpring_->GetPoints();
				const auto& weights = gaussSpring_->GetWeights();
				const auto& counter = gaussSpring_->GetCounter();

				weakForm->WeakFormulation(load, points[0], local);
				output = weights[0] * DetJ(points[0], 0) * local;

				for (quadrature::Counter i = 1; i < counter; ++i)
				{
					weakForm->WeakFormulation(load, points[i], local);

					output = output + weights[i] * DetJ(points[i], i) * local;
				}
			}
			else if(load->GetType() == loads::load_distributedFace)
			{ 
				auto force = std::static_pointer_cast<loads::LoadDistributedFace>(load);

				const auto& points = gaussRect_->GetPoints();
				const auto& weights = gaussRect_->GetWeights();
				const auto& counter = gaussRect_->GetCounter();

				auto faceIndex = force->GetFaceIndex();
				auto helper = GetIntegralAreaHelper(faceIndex);

				point(helper.dim1) = points[0](0);
				point(helper.dim2) = points[0](1);
				point(helper.dim3) = helper.coord3;

				weakForm->WeakFormulation(load, point, local);
				output = output + weights[0] * DelA(point, helper.dim1, helper.dim2) * local;

				for (quadrature::Counter i = 1; i < counter; ++i)
				{
					point(helper.dim1) = points[i](0);
					point(helper.dim2) = points[i](1);
					point(helper.dim3) = helper.coord3;

					weakForm->WeakFormulation(load, point, local);

					output = output + weights[i] * DelA(point, helper.dim1, helper.dim2) * local;
				}
			}
			else if (load->GetType() == loads::load_distributedEdge)
			{
				auto force = std::static_pointer_cast<loads::LoadDistributedEdge>(load);

				const auto& points = gaussLine_->GetPoints();
				const auto& weights = gaussLine_->GetWeights();
				const auto& counter = gaussLine_->GetCounter();

				auto edgeIndex = force->GetEdgeIndex();
				auto helper = GetIntegralEdgeHelper(edgeIndex);

				point(helper.dim1) = points[0](0);
				point(helper.dim2) = helper.coord2;
				point(helper.dim3) = helper.coord3;

				weakForm->WeakFormulation(load, point, local);
				output = output + weights[0] * DelL(point, helper.dim1) * local;

				for (quadrature::Counter i = 1; i < counter; ++i)
				{
					point(helper.dim1) = points[counter](0);
					point(helper.dim2) = helper.coord2;
					point(helper.dim3) = helper.coord3;

					weakForm->WeakFormulation(load, point, local);

					output = output + weights[counter] * DelL(point, helper.dim1) * local;
				}
			}
		}
		void ElementSpring::InitializeCache()
		{
		}
		Scalar ElementSpring::LinearFunctions::N0(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N0d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N0d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N0d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementSpring::LinearFunctions::N1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N1d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N1d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N1d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementSpring::LinearFunctions::N2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N2d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N2d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N2d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementSpring::LinearFunctions::N3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N3d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(1)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N3d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N3d3(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementSpring::LinearFunctions::N4(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N4d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N4d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N4d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementSpring::LinearFunctions::N5(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N5d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N5d2(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N5d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 - arg(1));
		}
		Scalar ElementSpring::LinearFunctions::N6(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N6d1(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N6d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N6d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 + arg(0)) * (1.0 + arg(1));
		}
		Scalar ElementSpring::LinearFunctions::N7(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N7d1(const Vector& arg)
		{
			return -1.0 / 8.0 * (1.0 + arg(1)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N7d2(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(2));
		}
		Scalar ElementSpring::LinearFunctions::N7d3(const Vector& arg)
		{
			return 1.0 / 8.0 * (1.0 - arg(0)) * (1.0 + arg(1));
		}
	} //namespace elements
} //namespace dive