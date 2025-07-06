#include "dive_loads_distributed_edge.hpp"
#include "dive_values_vector_congruent.hpp"
#include "dive_status.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedEdgePtr CreateLoadDistributedEdge(IElementPtr element, EdgeIndex edgeIndex, IVector3DPtr value)
		{
			if (!element->IsMapped())
			{
				logger::Error(headerDive, "Element is not mapped: " + dive::messages.at(dive::DIVE_INVALID_TYPE));
				return nullptr;
			}

			auto res = LoadDistributedEdge::Create();
			res->SetElement(std::dynamic_pointer_cast<elements::IElementMapped>(element));
			res->SetEdgeIndex(edgeIndex);
			res->SetValue(value);

			return res;
		}
		LoadDistributedEdgePtr CastToLoadDistributedEdge(ILoadPtr load)
		{
			return std::dynamic_pointer_cast<LoadDistributedEdge>(load);
		}
		LoadDistributedEdgePtr LoadDistributedEdge::Create()
		{
			class MakeSharedEnabler : public LoadDistributedEdge
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedEdgePtr LoadDistributedEdge::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedEdge>(shared_from_this());
		}
		ConstLoadDistributedEdgePtr LoadDistributedEdge::GetPtr() const
		{
			return const_cast<LoadDistributedEdge*>(this)->GetPtr();
		}
		Type LoadDistributedEdge::GetType() const
		{
			return type_;
		}
		IElementMappedPtr LoadDistributedEdge::GetElement() const
		{
			return element_;
		}
		Vector LoadDistributedEdge::GetValue(const Vector& point) const
		{
			return values::GetValueVector3D(value_, point, element_);
		}
		EdgeIndex LoadDistributedEdge::GetEdgeIndex() const
		{
			return edgeIndex_;
		}
		void LoadDistributedEdge::SetElement(IElementMappedPtr element)
		{
			element_ = element;
		}
		void LoadDistributedEdge::SetEdgeIndex(EdgeIndex edgeIndex)
		{
			edgeIndex_ = edgeIndex;
		}
		void LoadDistributedEdge::SetValue(IVector3DPtr value)
		{
			value_ = value;
		}
	} //namespace loads
} //namespace dive