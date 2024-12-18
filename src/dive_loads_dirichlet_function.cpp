#include "dive_loads_dirichlet.hpp"

namespace dive
{
	namespace loads
	{
		DirichletPtr CreateDirichlet()
		{
			auto res = Dirichlet::Create();

			return res;
		}
		DirichletPtr CreateDirichlet(IProblemPtr problem, INodePtr node, DofIndex dofIndex, Scalar value)
		{
			auto res = Dirichlet::Create();

			res->SetNode(node);
			res->SetValue(dofIndex, value);

			problem->AddDirichlet(res);

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
			return value_;
		}
		Type Dirichlet::GetType() const
		{
			return type_;
		}
		void Dirichlet::SetNode(INodePtr node)
		{
			node_ = node;
		}
		void Dirichlet::SetValue(DofIndex dofIndex, Scalar value)
		{
			dofIndex_ = dofIndex;
			value_ = value;
		}
	} //namespace loads
} //namespace dive