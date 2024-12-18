#include "dive_loads_distributed_edge.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedEdgePtr CreateLoadDistributedEdge(IElementPtr element, EdgeIndex edgeIndex, DofIndex dofIndex, Scalar(*function)(Scalar, Scalar, Scalar))
		{
			auto res = LoadDistributedEdge::Create();

			res->SetElement(element);
			res->SetEdgeIndex(edgeIndex);
			res->SetDofIndex(dofIndex);
			res->SetValue(values::CreateValueScalar3DFunction(function));

			return res;
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
		IElementPtr LoadDistributedEdge::GetElement() const
		{
			return element_;
		}
		DofIndex LoadDistributedEdge::GetDofIndex() const
		{
			return dofIndex_;
		}
		Scalar LoadDistributedEdge::GetValue(Scalar x, Scalar y, Scalar z) const
		{
			return value_->GetValue(x, y, z);
		}
		Scalar LoadDistributedEdge::GetValue(const Vector& point) const
		{
			return value_->GetValue(point);
		}
		EdgeIndex LoadDistributedEdge::GetEdgeIndex() const
		{
			return edgeIndex_;
		}
		void LoadDistributedEdge::SetElement(IElementPtr element)
		{
			element_ = element;
		}
		void LoadDistributedEdge::SetDofIndex(DofIndex dofIndex)
		{
			dofIndex_ = dofIndex;
		}
		void LoadDistributedEdge::SetValue(IScalar3DPtr value)
		{
			value_ = value;
		}
		void LoadDistributedEdge::SetEdgeIndex(EdgeIndex edgeIndex)
		{
			edgeIndex_ = edgeIndex;
		}
	} //namespace loads
} //namespace dive