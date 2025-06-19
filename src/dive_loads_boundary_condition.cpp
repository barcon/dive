#include "dive_loads_boundary_condition.hpp"

namespace dive
{
	namespace loads
	{
		BoundaryConditionPtr CreateBoundaryCondition(INodePtr node, DofIndex dofIndex, IScalar3DPtr value)
		{
			auto res = BoundaryCondition::Create();

			res->SetNode(node);
			res->SetDofIndex(dofIndex);
			res->SetValue(value);

			return res;
		}
		BoundaryConditionPtr BoundaryCondition::Create()
		{
			class MakeSharedEnabler : public BoundaryCondition
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		BoundaryConditionPtr BoundaryCondition::GetPtr()
		{
			return std::dynamic_pointer_cast<BoundaryCondition>(shared_from_this());
		}
		ConstBoundaryConditionPtr BoundaryCondition::GetPtr() const
		{
			return const_cast<BoundaryCondition*>(this)->GetPtr();
		}
		INodePtr BoundaryCondition::GetNode() const
		{
			return node_;
		}
		DofIndex BoundaryCondition::GetDofIndex() const
		{
			return dofIndex_;
		}
		Scalar BoundaryCondition::GetValue() const
		{
			const auto& point = node_->GetPoint();

			return value_->GetValue(point(0), point(1), point(2));
		}
		Type BoundaryCondition::GetType() const
		{
			return type_;
		}
		void BoundaryCondition::SetNode(INodePtr node)
		{
			node_ = node;
		}
		void BoundaryCondition::SetDofIndex(DofIndex dofIndex)
		{
			dofIndex_ = dofIndex;
		}
		void BoundaryCondition::SetValue(IScalar3DPtr value)
		{
			value_ = value;
		}
	} //namespace loads
} //namespace dive