#include "dive_elements_spring.hpp"
#include "dive_weakforms.hpp"

namespace dive
{
	namespace elements
	{
		const Scalar		ElementSpring::localCoordinates_[2][3] = { {-1.0,  0.0, 0.0 },
																	 {	1.0,  0.0, 0.0 } };

		const NodeIndex		ElementSpring::lookUpTable11_[1 * 2] = { NodeIndex(0), NodeIndex(1) };

		ElementSpringPtr CreateElementSpring()
		{
			auto res = ElementSpring::Create();

			return res;
		}
		ElementSpringPtr CreateElementSpring(Tag elementTag, IScalar1DPtr stiffness)
		{
			auto res = ElementSpring::Create();

			res->SetTag(elementTag);
			res->SetProperty(stiffness);

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
			for (NodeIndex i = 0; i < numberNodes_; ++i)
			{
				if (nodes_[i] == node)
				{
					return i;
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

		INodePtr ElementSpring::GetNode(const NodeIndex& nodeIndex) const
		{
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

		Scalar ElementSpring::Size() const
		{
			const auto& point0 = nodes_[0]->GetPoint();
			const auto& point1 = nodes_[1]->GetPoint();

			return eilig::NormP2(point1 - point0);
		}

		bool ElementSpring::IsUsed(INodePtr node) const
		{
			auto it = std::find_if(nodes_.begin(), nodes_.end(), [&](INodePtr ptr) -> bool
				{
					return ptr == node;
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
		bool ElementSpring::IsIntegrable() const
		{
			return false;
		}

		void ElementSpring::IntegralWeakFormElement(IWeakFormElementPtr weakForm, Matrix& output) const
		{
		}
		void ElementSpring::IntegralWeakFormLoad(IWeakFormLoadPtr weakForm, ILoadPtr load, Vector& output) const
		{
		}
		void ElementSpring::InitializeCache()
		{
		}
	} //namespace elements
} //namespace dive