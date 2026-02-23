#include "dive_load_dirichlet.hpp"

namespace dive
{
	namespace load
	{
		LoadDirichletPtr CreateLoadDirichlet(INodePtr node, DofIndex dofIndex, IScalar3DPtr value)
		{
			auto res = LoadDirichlet::Create();

			res->SetNode(node);
			res->SetDofIndex(dofIndex);
			res->SetValue(value);

			return res;
		}
		LoadDirichletPtr LoadDirichlet::Create()
		{
			class MakeSharedEnabler : public LoadDirichlet
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDirichletPtr LoadDirichlet::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDirichlet>(shared_from_this());
		}
		ConstLoadDirichletPtr LoadDirichlet::GetPtr() const
		{
			return const_cast<LoadDirichlet*>(this)->GetPtr();
		}
		INodePtr LoadDirichlet::GetNode() const
		{
			return node_;
		}
		DofIndex LoadDirichlet::GetDofIndex() const
		{
			return dofIndex_;
		}
		Scalar LoadDirichlet::GetValue() const
		{
			const auto& point = node_->GetPoint();

			return value_->GetValue(point(0), point(1), point(2));
		}
		Type LoadDirichlet::GetType() const
		{
			return type_;
		}
		void LoadDirichlet::SetNode(INodePtr node)
		{
			node_ = node;
		}
		void LoadDirichlet::SetDofIndex(DofIndex dofIndex)
		{
			dofIndex_ = dofIndex;
		}
		void LoadDirichlet::SetValue(IScalar3DPtr value)
		{
			value_ = value;
		}
	} //namespace load
} //namespace dive