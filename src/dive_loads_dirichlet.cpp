#include "dive_loads_dirichlet.hpp"

namespace dive
{
	namespace loads
	{
		DirichletPtr CreateDirichletByValue(INodePtr node, DofIndex dofIndex, Scalar value)
		{
			auto res = Dirichlet::Create();

			res->SetNode(node);
			res->SetDofIndex(dofIndex);
			res->SetValue(values::CreateValueScalar3D(value));

			return res;
		}
		DirichletPtr CreateDirichletByFunction(INodePtr node, DofIndex dofIndex, Scalar(*function)(Scalar, Scalar, Scalar))
		{
			auto res = Dirichlet::Create();

			res->SetNode(node);
			res->SetDofIndex(dofIndex);
			res->SetValue(values::CreateValueScalar3DFunction(function));

			return res;
		}
		DirichletPtr Dirichlet::Create()
		{
			class MakeSharedEnabler : public Dirichlet
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		DirichletPtr Dirichlet::GetPtr()
		{
			return std::dynamic_pointer_cast<Dirichlet>(shared_from_this());
		}
		ConstDirichletPtr Dirichlet::GetPtr() const
		{
			return const_cast<Dirichlet*>(this)->GetPtr();
		}
		INodePtr Dirichlet::GetNode() const
		{
			return node_;
		}
		DofIndex Dirichlet::GetDofIndex() const
		{
			return dofIndex_;
		}
		Scalar Dirichlet::GetValue() const
		{
			const auto& point = node_->GetPoint();

			return value_->GetValue(point(0), point(1), point(2));
		}
		Type Dirichlet::GetType() const
		{
			return type_;
		}
		void Dirichlet::SetNode(INodePtr node)
		{
			node_ = node;
		}
		void Dirichlet::SetDofIndex(DofIndex dofIndex)
		{
			dofIndex_ = dofIndex;
		}
		void Dirichlet::SetValue(IScalar3DPtr value)
		{
			value_ = value;
		}
	} //namespace loads
} //namespace dive