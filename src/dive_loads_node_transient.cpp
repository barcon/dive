#include "dive_loads_node_transient.hpp"

namespace dive
{
	namespace loads
	{
		LoadNodeTransientPtr CreateLoadNodeTransient(INodePtr node, IVector3DTimePtr value)
		{
			auto res = LoadNodeTransient::Create();

			res->SetNode(node);
			res->SetValue(value);

			return res;
		}
		LoadNodeTransientPtr CastToLoadNodeTransient(ILoadPtr load)
		{
			return std::dynamic_pointer_cast<LoadNodeTransient>(load);
		}
		LoadNodeTransientPtr LoadNodeTransient::Create()
		{
			class MakeSharedEnabler : public LoadNodeTransient
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadNodeTransientPtr LoadNodeTransient::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadNodeTransient>(shared_from_this());;
		}
		ConstLoadNodeTransientPtr LoadNodeTransient::GetPtr() const
		{
			return const_cast<LoadNodeTransient*>(this)->GetPtr();
		}
		Type LoadNodeTransient::GetType() const
		{
			return type_;
		}
		INodePtr LoadNodeTransient::GetNode() const
		{
			return node_;
		}
		void LoadNodeTransient::SetNode(INodePtr node)
		{
			node_ = node;
		}
		Vector LoadNodeTransient::GetValue(Scalar time) const
		{
			return value_->GetValue(time, node_->GetPoint());
		}
		void LoadNodeTransient::SetValue(IVector3DTimePtr value)
		{
			value_ = value;
		}
	} //namespace loads
} //namespace dive