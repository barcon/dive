#include "dive_elements_spring.hpp"
#include "dive_status.hpp"
#include "dive_weakforms.hpp"

namespace dive
{
	namespace elements
	{
		const Scalar	ElementSpring::localCoordinates_[2][3] = { {-1.0,  0.0, 0.0 }, { 1.0,  0.0, 0.0 } };
		const NodeIndex	ElementSpring::lookUpTable11_[1 * 2] = { NodeIndex(0), NodeIndex(1) };

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
		ElementSpringPtr CreateElementSpring(Tag elementTag, IScalarPtr stiffness)
		{
			auto res = ElementSpring::Create();

			res->SetTag(elementTag);
			res->SetStiffness(stiffness);

			return res;
		}
		ElementSpringPtr CastToElementSpring(IElementPtr element)
		{
			return std::dynamic_pointer_cast<ElementSpring>(element);
		}

		ElementSpring::ElementSpring()
		{
			nodes_.resize(numberNodes_);

			SetNumberDof(numberDof_);
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
		
		Tag ElementSpring::GetTag() const
		{
			return tag_;
		}
		Type ElementSpring::GetType() const
		{
			return type_;
		}
		Vector ElementSpring::GetCenter() const
		{
			Vector center(3, 0.0);

			return 	GlobalCoordinates(center);;
		}
		IMaterialPtr ElementSpring::GetMaterial() const
		{
			return nullptr;
		}

		NodeIndex ElementSpring::GetNodeIndex(const INodePtr node) const
		{
			if (node == nullptr)
			{
				return nodeIndexInvalid;
			}

			for (NodeIndex i = 0; i < numberNodes_; ++i)
			{
				if (nodes_[i] != nullptr)
				{
					if (nodes_[i]->GetTag() == node->GetTag())
					{
						return i;
					}
				}
			}

			return nodeIndexInvalid;
		}
		ElementIndex ElementSpring::GetElementIndex() const
		{
			return elementIndex_;
		}

		NumberDof ElementSpring::GetNumberDof() const
		{
			return numberDof_;
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
		NumberNodes ElementSpring::GetNumberNodesFace(const FaceIndex& faceIndex) const
		{
			return numberNodesFace_;
		}
		NumberNodes ElementSpring::GetNumberNodesEdge(const EdgeIndex& edgeNode) const
		{
			return numberNodesEdge_;
		}
		NumberDimensions ElementSpring::GetNumberDimensions() const
		{
			return numberDimensions_;
		}
		NumberCoordinates ElementSpring::GetNumberCoordinates() const
		{
			return numberCoordinates_;
		}

		INodePtr ElementSpring::GetNode(const NodeIndex& nodeIndex) const
		{
			if (nodeIndex >= numberNodes_)
			{
				logger::Error(headerDive, "Invalide node index: " + dive::messages.at(dive::DIVE_OUT_OF_RANGE));
				return nullptr;
			}

			return nodes_[nodeIndex];
		}
		INodePtr ElementSpring::GetNodeFace(const FaceIndex& faceIndex, const NodeIndex& nodeIndex) const
		{
			return nullptr;
		}
		INodePtr ElementSpring::GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const
		{
			return GetNode(lookUpTable11_[numberNodesEdge_ * edgeIndex + nodeIndex]);
		}
		const Nodes& ElementSpring::GetNodes() const
		{
			return nodes_;
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

		void ElementSpring::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ElementSpring::SetNumberDof(NumberDof numberDof)
		{
			numberDof_ = numberDof;

			for (NodeIndex i = 0; i < numberNodes_; ++i)
			{
				if (nodes_[i] != nullptr)
				{
					if (nodes_[i]->GetNumberDof() != numberDof_)
					{
						nodes_[i]->SetNumberDof(numberDof_);
						nodes_[i]->GetConnectivity().globalDofIndices.resize(numberDof_);
					}
				}
			}

			I_ = Matrix(numberDof * numberNodes_, numberDof * numberNodes_);
			I_(0, 0) =  1.0;
			I_(0, (numberNodes_ - 1) * numberDof_) =  -1.0;

			I_((numberNodes_ - 1) * numberDof_, 0) = -1.0;
			I_((numberNodes_ - 1) * numberDof_, (numberNodes_ - 1) * numberDof_) = 1.0;
		}
		void ElementSpring::SetNode(const NodeIndex& nodeIndex, INodePtr node)
		{
			if (nodeIndex >= numberNodes_)
			{
				logger::Error(headerDive, "Invalid node index: " + dive::messages.at(dive::DIVE_OUT_OF_RANGE));
				return;
			}

			if (nodes_[nodeIndex] == nullptr)
			{
				if (node != nullptr)
				{
					nodes_[nodeIndex] = node;
					nodes_[nodeIndex]->SetNumberDof(numberDof_);
					nodes_[nodeIndex]->GetConnectivity().globalDofIndices.resize(numberDof_);
					nodes_[nodeIndex]->GetConnectivity().elements.push_back(this->GetPtr());
				}
				
				return;
			}
			else
			{
				auto& elements = nodes_[nodeIndex]->GetConnectivity().elements;
				auto it = std::find_if(elements.begin(), elements.end(), [&](IElementPtr element) -> bool
					{
						return element->GetTag() == tag_;
					});

				if (it != elements.end())
				{
					elements.erase(it);
				}

				nodes_[nodeIndex] = node;

				if (node != nullptr)
				{
					nodes_[nodeIndex]->SetNumberDof(numberDof_);
					nodes_[nodeIndex]->GetConnectivity().globalDofIndices.resize(numberDof_);
					nodes_[nodeIndex]->GetConnectivity().elements.push_back(this->GetPtr());
				}
				
				return;
			}
		}
		void ElementSpring::SetElementIndex(ElementIndex index)
		{
			elementIndex_ = index;
		}
		void ElementSpring::SetMaterial(IMaterialPtr material)
		{
		}
		void ElementSpring::SetProperty(IValuePtr value)
		{
			properties_.insert({ value->GetKey(), value });
		}

		Vector ElementSpring::LocalCoordinates(INodePtr node) const
		{
			auto nodeIndex = GetNodeIndex(node);

			if (nodeIndex == nodeIndexInvalid)
			{
				logger::Error(headerDive, "Invalid node: " + dive::messages.at(dive::DIVE_NOT_FOUND));
				return Vector();
			}

			return LocalCoordinates(nodeIndex);
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
			Scalar s = local(0);

			return 0.5 * (nodes_[0]->GetPoint() * (1.0 - s) + nodes_[1]->GetPoint() * (1.0 + s));
		}

		Scalar ElementSpring::SizeMinimum() const
		{
			const auto& point0 = nodes_[0]->GetPoint();
			const auto& point1 = nodes_[1]->GetPoint();

			return eilig::NormP2(point1 - point0);
		}
		Scalar ElementSpring::SizeMaximum() const
		{
			const auto& point0 = nodes_[0]->GetPoint();
			const auto& point1 = nodes_[1]->GetPoint();

			return eilig::NormP2(point1 - point0);
		}

		bool ElementSpring::IsUsed(INodePtr node) const
		{
			if (node == nullptr)
			{
				return false;
			}

			auto it = std::find_if(nodes_.begin(), nodes_.end(), [&](INodePtr ptr) -> bool
				{
					return ptr->GetTag() == node->GetTag();
				});

			if (it != nodes_.end())
			{
				return true;
			}

			return false;
		}
		bool ElementSpring::IsMapped() const
		{
			return false;
		}
		bool ElementSpring::IsCacheable() const
		{
			return false;
		}

		void ElementSpring::InitializeCache()
		{
		}

		IScalarPtr ElementSpring::GetStiffness() const
		{
			return stiffness_;
		}
		void ElementSpring::SetStiffness(IScalarPtr stiffness)
		{
			stiffness_ = stiffness;
		}
		void ElementSpring::Stiffness(Matrix& output) const
		{
			auto T = FormMatrix_Transform();

			//output = stiffness_->GetValue() * I_ * T;
			output = stiffness_->GetValue() * I_;
		}

		Vector ElementSpring::GetGlobalVector0() const
		{
			Vector e0(3);
			
			e0(0) = 1.0;

			return e0;
		}
		Vector ElementSpring::GetGlobalVector1() const
		{
			Vector e1(3);

			e1(1) = 1.0;

			return e1;
		}
		Vector ElementSpring::GetGlobalVector2() const
		{
			Vector e2(3);

			e2(2) = 1.0;

			return e2;
		}
		Vector ElementSpring::GetLocalVector0() const
		{
			const auto& p1 = nodes_[1]->GetPoint();
			const auto& p0 = nodes_[0]->GetPoint();
			
			auto l0 = eilig::NormP2(p1 - p0);			
			auto v0 = (1.0 / l0) * (p1 - p0);

			return v0;
		}
		Vector ElementSpring::GetLocalVector1() const
		{			 
			Vector v0 = GetLocalVector0();
			Vector v1;
			Scalars dots;
			
			auto e0 = GetGlobalVector0();
			auto e1 = GetGlobalVector1();
			auto e2 = GetGlobalVector2();

			dots.push_back(std::abs(eilig::Dot(v0, e0)));
			dots.push_back(std::abs(eilig::Dot(v0, e1)));
			dots.push_back(std::abs(eilig::Dot(v0, e2)));

			auto minimum = std::min_element(dots.begin(), dots.end());
			auto index = std::distance(dots.begin(), minimum);

			switch (index)
			{
			case 0:
				v1 = eilig::Cross(v0, e0);
				break;
			case 1:
				v1 = eilig::Cross(v0, e1);
				break;
			case 2:
				v1 = eilig::Cross(v0, e2);
				break;
			}

			auto l1 = eilig::NormP2(v1);

			return (1.0 / l1) * v1;
		}
		Vector ElementSpring::GetLocalVector2() const
		{
			Vector v0 = GetLocalVector0();
			Vector v1 = GetLocalVector1();

			return eilig::Cross(v0, v1);
		}

		Matrix ElementSpring::FormMatrix_Transform() const
		{
			Matrix res(numberDof_ * numberNodes_, numberDof_ * numberNodes_);
			
			Vectors local;
			Vectors global;

			local.push_back(GetLocalVector0());
			local.push_back(GetLocalVector1());
			local.push_back(GetLocalVector2());
			
			global.push_back(GetGlobalVector0());
			global.push_back(GetGlobalVector1());
			global.push_back(GetGlobalVector2());

			for (Index i = 0; i < numberNodes_; i++)
			{
				for (Index j = 0; j < numberDof_; j++)
				{
					res(i, j) = eilig::Dot(local[j], global[i]);
				}
			}

			return res;
		}
	} //namespace elements
} //namespace dive