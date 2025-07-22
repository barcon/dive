#include "dive_elements_combined.hpp"
#include "dive_status.hpp"
#include "dive_weakforms.hpp"

namespace dive
{
	namespace elements
	{
		const Scalar	ElementCombined::localCoordinates_[2][3] = { {-1.0,  0.0, 0.0 }, { 1.0,  0.0, 0.0 } };
		const NodeIndex	ElementCombined::lookUpTable11_[1 * 2] = { NodeIndex(0), NodeIndex(1) };

		ElementCombinedPtr CreateElementCombined()
		{
			auto res = ElementCombined::Create();

			return res;
		}
		ElementCombinedPtr CreateElementCombined(Tag elementTag)
		{
			auto res = ElementCombined::Create();

			res->SetTag(elementTag);

			return res;
		}
		ElementCombinedPtr CreateElementCombined(Tag elementTag, IScalarPtr stiffness, IScalarPtr damping)
		{
			auto res = ElementCombined::Create();

			res->SetTag(elementTag);
			res->SetStiffness(stiffness);
			res->SetDamping(damping);

			return res;
		}
		ElementCombinedPtr CastToElementCombined(IElementPtr element)
		{
			return std::dynamic_pointer_cast<ElementCombined>(element);
		}

		ElementCombined::ElementCombined()
		{
			nodes_.resize(numberNodes_);

			SetStiffness(values::CreateValueScalar(0.0));
			SetDamping(values::CreateValueScalar(0.0));
			SetNumberDof(numberDof_);
		}
		ElementCombinedPtr ElementCombined::Create()
		{
			class MakeSharedEnabler : public ElementCombined
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ElementCombinedPtr ElementCombined::GetPtr()
		{
			return std::dynamic_pointer_cast<ElementCombined>(shared_from_this());
		}
		ConstElementCombinedPtr ElementCombined::GetPtr() const
		{
			return const_cast<ElementCombined*>(this)->GetPtr();
		}
		
		Tag ElementCombined::GetTag() const
		{
			return tag_;
		}
		Type ElementCombined::GetType() const
		{
			return type_;
		}
		Vector ElementCombined::GetCenter() const
		{
			Vector center(3, 0.0);

			return 	GlobalCoordinates(center);;
		}
		IMaterialPtr ElementCombined::GetMaterial() const
		{
			return nullptr;
		}

		NodeIndex ElementCombined::GetNodeIndex(const INodePtr node) const
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
		ElementIndex ElementCombined::GetElementIndex() const
		{
			return elementIndex_;
		}

		NumberDof ElementCombined::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberNodes ElementCombined::GetNumberNodes() const
		{
			return numberNodes_;
		}
		NumberFaces ElementCombined::GetNumberFaces() const
		{
			return numberFaces_;
		}
		NumberEdges ElementCombined::GetNumberEdges() const
		{
			return numberEdges_;
		}
		NumberNodes ElementCombined::GetNumberNodesFace(const FaceIndex& faceIndex) const
		{
			return numberNodesFace_;
		}
		NumberNodes ElementCombined::GetNumberNodesEdge(const EdgeIndex& edgeNode) const
		{
			return numberNodesEdge_;
		}
		NumberDimensions ElementCombined::GetNumberDimensions() const
		{
			return numberDimensions_;
		}
		NumberCoordinates ElementCombined::GetNumberCoordinates() const
		{
			return numberCoordinates_;
		}

		INodePtr ElementCombined::GetNode(const NodeIndex& nodeIndex) const
		{
			if (nodeIndex >= numberNodes_)
			{
				logger::Error(headerDive, "Invalide node index: " + dive::messages.at(dive::DIVE_OUT_OF_RANGE));
				return nullptr;
			}

			return nodes_[nodeIndex];
		}
		INodePtr ElementCombined::GetNodeFace(const FaceIndex& faceIndex, const NodeIndex& nodeIndex) const
		{
			return nullptr;
		}
		INodePtr ElementCombined::GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const
		{
			return GetNode(lookUpTable11_[numberNodesEdge_ * edgeIndex + nodeIndex]);
		}
		const Nodes& ElementCombined::GetNodes() const
		{
			return nodes_;
		}
		IValuePtr ElementCombined::GetProperty(String key) const
		{
			auto property = properties_.find(key);

			if (property == properties_.end())
			{
				return nullptr;
			}

			return property->second;
		}

		void ElementCombined::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ElementCombined::SetNumberDof(NumberDof numberDof)
		{
			if(numberDof < 1 || numberDof > 3)
			{
				logger::Error(headerDive, "Invalid number of degrees of freedom: " + dive::messages.at(dive::DIVE_OUT_OF_RANGE));
				return;
			}

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
		}
		void ElementCombined::SetNode(const NodeIndex& nodeIndex, INodePtr node)
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
		void ElementCombined::SetElementIndex(ElementIndex index)
		{
			elementIndex_ = index;
		}
		void ElementCombined::SetMaterial(IMaterialPtr material)
		{
		}
		void ElementCombined::SetProperty(IValuePtr value)
		{
			properties_.insert({ value->GetKey(), value });
		}

		Vector ElementCombined::LocalCoordinates(INodePtr node) const
		{
			auto nodeIndex = GetNodeIndex(node);

			if (nodeIndex == nodeIndexInvalid)
			{
				logger::Error(headerDive, "Invalid node: " + dive::messages.at(dive::DIVE_NOT_FOUND));
				return Vector();
			}

			return LocalCoordinates(nodeIndex);
		}
		Vector ElementCombined::LocalCoordinates(const NodeIndex& nodeIndex) const
		{
			Vector local(3);

			local(0) = localCoordinates_[nodeIndex][0];
			local(1) = localCoordinates_[nodeIndex][1];
			local(2) = localCoordinates_[nodeIndex][2];

			return local;
		}
		Vector ElementCombined::GlobalCoordinates(INodePtr node) const
		{
			return GlobalCoordinates(GetNodeIndex(node));
		}
		Vector ElementCombined::GlobalCoordinates(const NodeIndex& nodeIndex) const
		{
			return nodes_[nodeIndex]->GetPoint();
		}
		Vector ElementCombined::GlobalCoordinates(const Vector& local) const
		{
			Scalar s = local(0);

			return 0.5 * (nodes_[0]->GetPoint() * (1.0 - s) + nodes_[1]->GetPoint() * (1.0 + s));
		}

		Scalar ElementCombined::SizeMinimum() const
		{
			const auto& point0 = nodes_[0]->GetPoint();
			const auto& point1 = nodes_[1]->GetPoint();

			return eilig::NormP2(point1 - point0);
		}
		Scalar ElementCombined::SizeMaximum() const
		{
			const auto& point0 = nodes_[0]->GetPoint();
			const auto& point1 = nodes_[1]->GetPoint();

			return eilig::NormP2(point1 - point0);
		}

		bool ElementCombined::IsUsed(INodePtr node) const
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
		bool ElementCombined::IsMapped() const
		{
			return false;
		}
		bool ElementCombined::IsCacheable() const
		{
			return false;
		}

		void ElementCombined::InitializeCache()
		{
		}

		IScalarPtr ElementCombined::GetStiffness() const
		{
			return stiffness_;
		}
		IScalarPtr ElementCombined::GetDamping() const
		{
			return damping_;
		}

		void ElementCombined::SetCanonical(bool canonical)
		{
			canonical_ = canonical;
		}
		void ElementCombined::SetStiffness(IScalarPtr stiffness)
		{
			if (stiffness == nullptr)
			{
				logger::Error(headerDive, "Invalid stiffness: " + dive::messages.at(dive::DIVE_NULL_POINTER));
				return;
			}

			stiffness_ = stiffness;
		}
		void ElementCombined::SetDamping(IScalarPtr damping)
		{
			if (damping == nullptr)
			{
				logger::Error(headerDive, "Invalid damping: " + dive::messages.at(dive::DIVE_NULL_POINTER));
				return;
			}

			damping_ = damping;
		}
		
		void ElementCombined::Stiffness(Matrix& output) const
		{
			if (canonical_)
			{
				output = stiffness_->GetValue() * FormMatrix_Canonical();
			}
			else
			{
				output = stiffness_->GetValue() * FormMatrix_Decomposed();
			}
		}
		void ElementCombined::Damping(Matrix& output) const
		{
			if (canonical_)
			{
				output = damping_->GetValue() * FormMatrix_Canonical();
			}
			else
			{
				output = damping_->GetValue() * FormMatrix_Decomposed();
			}
		}

		Vector ElementCombined::GetGlobalVector0() const
		{
			Vector e0(3);
			
			e0(0) = 1.0;

			return e0;
		}
		Vector ElementCombined::GetGlobalVector1() const
		{
			Vector e1(3);

			e1(1) = 1.0;

			return e1;
		}
		Vector ElementCombined::GetGlobalVector2() const
		{
			Vector e2(3);

			e2(2) = 1.0;

			return e2;
		}
		Vector ElementCombined::GetLocalVector0() const
		{
			const auto& p0 = nodes_[0]->GetPoint();
			const auto& p1 = nodes_[1]->GetPoint();
			
			auto l0 = eilig::NormP2(p1 - p0);
			auto v0 = (1.0 / l0) * (p1 - p0);

			return v0;
		}

		Matrix ElementCombined::FormMatrix_Canonical() const
		{
			auto res = Matrix(numberNodes_ * numberDof_, numberNodes_ * numberDof_, eilig::matrix_zeros);

			switch (numberDof_)
			{
			case 1:
				res(0, 0) = +1.0;
				res(0, 1) = -1.0;
				res(1, 1) = +1.0;
				res(1, 0) = -1.0;

				break;
			case 2:
				res(0, 0) = +1.0;
				res(0, 2) = -1.0;
				res(2, 2) = +1.0;
				res(2, 0) = -1.0;

				res(1, 1) = +1.0;
				res(1, 3) = -1.0;
				res(3, 3) = +1.0;
				res(3, 1) = -1.0;

				break;
			case 3:
				res(0, 0) = +1.0;
				res(0, 3) = -1.0;
				res(3, 3) = +1.0;
				res(3, 0) = -1.0;

				res(1, 1) = +1.0;
				res(1, 4) = -1.0;
				res(4, 4) = +1.0;
				res(4, 1) = -1.0;

				res(2, 2) = +1.0;
				res(2, 5) = -1.0;
				res(5, 5) = +1.0;
				res(5, 2) = -1.0;

				break;
			}

			return res;
		}
		Matrix ElementCombined::FormMatrix_Decomposed() const
		{
			auto res = Matrix(numberNodes_ * numberDof_, numberNodes_ * numberDof_, eilig::matrix_zeros);

			auto local = GetLocalVector0();
			auto dot1 = std::abs(eilig::Dot(local, GetGlobalVector0()));
			auto dot2 = std::abs(eilig::Dot(local, GetGlobalVector1()));
			auto dot3 = std::abs(eilig::Dot(local, GetGlobalVector2()));

			switch (numberDof_)
			{
			case 1:
				res(0, 0) = +dot1;
				res(0, 1) = -dot1;
				res(1, 1) = +dot1;
				res(1, 0) = -dot1;

				break;
			case 2:
				res(0, 0) = +dot1;
				res(0, 2) = -dot1;
				res(2, 2) = +dot1;
				res(2, 0) = -dot1;

				res(1, 1) = +dot2;
				res(1, 3) = -dot2;
				res(3, 3) = +dot2;
				res(3, 1) = -dot2;

				break;
			case 3:
				res(0, 0) = +dot1;
				res(0, 3) = -dot1;
				res(3, 3) = +dot1;
				res(3, 0) = -dot1;

				res(1, 1) = +dot2;
				res(1, 4) = -dot2;
				res(4, 4) = +dot2;
				res(4, 1) = -dot2;

				res(2, 2) = +dot3;
				res(2, 5) = -dot3;
				res(5, 5) = +dot3;
				res(5, 2) = -dot3;

				break;
			}

			return res;
		}
	} //namespace elements
} //namespace dive

/*

		Vector ElementCombined::GetLocalVector0() const
		{
			const auto& p0 = nodes_[0]->GetPoint();
			const auto& p1 = nodes_[1]->GetPoint();

			auto l0 = eilig::NormP2(p1 - p0);
			auto v0 = (1.0 / l0) * (p1 - p0);

			return v0;
		}
		Vector ElementCombined::GetLocalVector1() const
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
		Vector ElementCombined::GetLocalVector2() const
		{
			Vector v0 = GetLocalVector0();
			Vector v1 = GetLocalVector1();

			return eilig::Cross(v0, v1);
		}

*/