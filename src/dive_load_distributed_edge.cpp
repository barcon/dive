#include "dive_load_distributed_edge.hpp"
#include "dive_values_vector_congruent.hpp"
#include "dive_status.hpp"

namespace dive
{
	namespace load
	{
		LoadDistributedEdgePtr CreateLoadDistributedEdge(IElementPtr element, EdgeIndex edgeIndex, IVectorCoordinatesPtr value)
		{
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
			return values::GetValueVectorCoordinates(value_, point, element_);
		}
		EdgeIndex LoadDistributedEdge::GetEdgeIndex() const
		{
			return edgeIndex_;
		}
		void LoadDistributedEdge::SetElement(IElementMappedPtr element)
		{
			if (element == nullptr)
			{
				throw std::invalid_argument("Element is nullptr");
			}

			if (!element->IsMapped())
			{
				throw std::invalid_argument("Element is not mapped");
			}

			element_ = element;
		}
		void LoadDistributedEdge::SetEdgeIndex(EdgeIndex edgeIndex)
		{
			edgeIndex_ = edgeIndex;
		}
		void LoadDistributedEdge::SetValue(IVectorCoordinatesPtr value)
		{
			if (value == nullptr)
			{
				throw std::invalid_argument("Value is nullptr");
			}

			if (value->GetNumberCoordinates() != element_->GetNumberCoordinates())
			{
				throw std::invalid_argument("Number coordinates of value does not match number coordinates of element");
			}

			value_ = value;
		}
	} //namespace load
} //namespace dive