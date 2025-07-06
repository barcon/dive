#include "dive_loads_node.hpp"

namespace dive
{
	namespace loads
	{
		LoadNodePtr CreateLoadNode(INodePtr node, IVector3DPtr value)
		{
			auto res = LoadNode::Create();

			res->SetNode(node);
			res->SetValue(value);

			return res;
		}
		LoadNodePtr CastToLoadNode(ILoadPtr load)
		{
			return std::dynamic_pointer_cast<LoadNode>(load);
		}
		LoadNodePtr LoadNode::Create()
		{
			class MakeSharedEnabler : public LoadNode
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadNodePtr LoadNode::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadNode>(shared_from_this());;
		}
		ConstLoadNodePtr LoadNode::GetPtr() const
		{
			return const_cast<LoadNode*>(this)->GetPtr();
		}
		Type LoadNode::GetType() const
		{
			return type_;
		}
		INodePtr LoadNode::GetNode() const
		{
			return node_;
		}
		void LoadNode::SetNode(INodePtr node)
		{
			node_ = node;
		}
		Vector LoadNode::GetValue() const
		{
			return value_->GetValue(node_->GetPoint());
		}
		void LoadNode::SetValue(IVector3DPtr value)
		{
			value_ = value;
		}
	} //namespace loads
} //namespace dive