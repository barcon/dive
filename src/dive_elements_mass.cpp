#include "dive_elements_mass.hpp"
#include "dive_status.hpp"
#include "dive_weakforms.hpp"

namespace dive
{
	namespace elements
	{
		const Scalar ElementMass::localCoordinates_[1][3] = { {0.0,  0.0, 0.0 }};

		ElementMassPtr CreateElementMass()
		{
			auto res = ElementMass::Create();

			return res;
		}
		ElementMassPtr CreateElementMass(Tag elementTag)
		{
			auto res = ElementMass::Create();

			res->SetTag(elementTag);

			return res;
		}
		ElementMassPtr CreateElementMass(Tag elementTag, IScalarPtr mass)
		{
			auto res = ElementMass::Create();

			res->SetTag(elementTag);
			res->SetMass(mass);

			return res;
		}
		ElementMassPtr CastToElementMass(IElementPtr element)
		{
			return std::dynamic_pointer_cast<ElementMass>(element);
		}

		ElementMass::ElementMass()
		{
			nodes_.resize(numberNodes_);
		}
		ElementMassPtr ElementMass::Create()
		{
			class MakeSharedEnabler : public ElementMass
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ElementMassPtr ElementMass::GetPtr()
		{
			return std::dynamic_pointer_cast<ElementMass>(shared_from_this());
		}
		ConstElementMassPtr ElementMass::GetPtr() const
		{
			return const_cast<ElementMass*>(this)->GetPtr();
		}
		
		Tag ElementMass::GetTag() const
		{
			return tag_;
		}
		Type ElementMass::GetType() const
		{
			return type_;
		}
		Vector ElementMass::GetCenter() const
		{
			Vector center(3, 0.0);

			return 	GlobalCoordinates(center);
		}
		IMaterialPtr ElementMass::GetMaterial() const
		{
			return nullptr;
		}

		NodeIndex ElementMass::GetNodeIndex(const INodePtr node) const
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
		ElementIndex ElementMass::GetElementIndex() const
		{
			return elementIndex_;
		}

		NumberDof ElementMass::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberNodes ElementMass::GetNumberNodes() const
		{
			return numberNodes_;
		}
		NumberFaces ElementMass::GetNumberFaces() const
		{
			return numberFaces_;
		}
		NumberEdges ElementMass::GetNumberEdges() const
		{
			return numberEdges_;
		}
		NumberNodes ElementMass::GetNumberNodesFace(const FaceIndex& faceIndex) const
		{
			return numberNodesFace_;
		}
		NumberNodes ElementMass::GetNumberNodesEdge(const EdgeIndex& edgeNode) const
		{
			return numberNodesEdge_;
		}
		NumberDimensions ElementMass::GetNumberDimensions() const
		{
			return numberDimensions_;
		}
		NumberCoordinates ElementMass::GetNumberCoordinates() const
		{
			return numberCoordinates_;
		}

		INodePtr ElementMass::GetNode(const NodeIndex& nodeIndex) const
		{
			if (nodeIndex >= numberNodes_)
			{
				logger::Error(headerDive, "Invalide node index: " + dive::messages.at(dive::DIVE_OUT_OF_RANGE));
				return nullptr;
			}

			return nodes_[nodeIndex];
		}
		INodePtr ElementMass::GetNodeFace(const FaceIndex& faceIndex, const NodeIndex& nodeIndex) const
		{
			return nullptr;
		}
		INodePtr ElementMass::GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const
		{
			return nullptr;
		}
		const Nodes& ElementMass::GetNodes() const
		{
			return nodes_;
		}
		IValuePtr ElementMass::GetProperty(String key) const
		{
			auto property = properties_.find(key);

			if (property == properties_.end())
			{
				return nullptr;
			}

			return property->second;
		}
		bool ElementMass::GetNonlinear() const
		{
			return nonlinear_;
		}

		void ElementMass::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ElementMass::SetNumberDof(NumberDof numberDof)
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

			I_ = Matrix(numberDof * numberNodes_, numberDof * numberNodes_, eilig::matrix_diagonal);
		}
		void ElementMass::SetNode(const NodeIndex& nodeIndex, INodePtr node)
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
		void ElementMass::SetElementIndex(ElementIndex index)
		{
			elementIndex_ = index;
		}
		void ElementMass::SetMaterial(IMaterialPtr material)
		{
		}
		void ElementMass::SetProperty(IValuePtr value)
		{
			properties_.insert({ value->GetKey(), value });
		}
		void ElementMass::SetNonlinear(bool nonlinear)
		{
			nonlinear_ = nonlinear;
		}

		Vector ElementMass::LocalCoordinates(INodePtr node) const
		{
			auto nodeIndex = GetNodeIndex(node);

			if (nodeIndex == nodeIndexInvalid)
			{
				logger::Error(headerDive, "Invalid node: " + dive::messages.at(dive::DIVE_NOT_FOUND));
				return Vector();
			}

			return LocalCoordinates(nodeIndex);
		}
		Vector ElementMass::LocalCoordinates(const NodeIndex& nodeIndex) const
		{
			Vector local(3);

			local(0) = localCoordinates_[nodeIndex][0];
			local(1) = localCoordinates_[nodeIndex][1];
			local(2) = localCoordinates_[nodeIndex][2];

			return local;
		}
		Vector ElementMass::GlobalCoordinates(INodePtr node) const
		{
			return GlobalCoordinates(GetNodeIndex(node));
		}
		Vector ElementMass::GlobalCoordinates(const NodeIndex& nodeIndex) const
		{
			return nodes_[nodeIndex]->GetPoint();
		}
		Vector ElementMass::GlobalCoordinates(const Vector& local) const
		{
			return nodes_[0]->GetPoint();
		}

		Scalar ElementMass::SizeMinimum() const
		{
			return 0.0;
		}
		Scalar ElementMass::SizeMaximum() const
		{
			return 0.0;
		}

		bool ElementMass::IsUsed(INodePtr node) const
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
		bool ElementMass::IsMapped() const
		{
			return false;
		}
		bool ElementMass::IsCacheable() const
		{
			return false;
		}

		void ElementMass::InitializeCache()
		{
		}

		IScalarPtr ElementMass::GetMass() const
		{
			return mass_;
		}
		void ElementMass::SetMass(IScalarPtr mass)
		{
			mass_ = mass;
		}
		void ElementMass::Mass(Matrix& output) const
		{
			output = mass_->GetValue() * I_;
		}
	} //namespace elements
} //namespace dive