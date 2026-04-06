#include "dive_load_dirichlet.hpp"

namespace dive
{
	namespace load
	{
		LoadDirichletPtr CreateLoadDirichlet(INodePtr node, DofIndex dofIndex, IScalarCoordinatesPtr value)
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
			return value_->GetValue(node_->GetPoint());
		}
		Type LoadDirichlet::GetType() const
		{
			return type_;
		}
		void LoadDirichlet::SetNode(INodePtr node)
		{
			if (node == nullptr)
			{
				throw std::invalid_argument("Node is nullptr");
			}

			node_ = node;
		}
		void LoadDirichlet::SetDofIndex(DofIndex dofIndex)
		{
			if (dofIndex >= node_->GetNumberDof())
			{
				throw std::invalid_argument("Dof index is out of range");
			}

			dofIndex_ = dofIndex;
		}
		void LoadDirichlet::SetValue(IScalarCoordinatesPtr value)
		{
			if (value == nullptr)
			{
				throw std::invalid_argument("Value is nullptr");
			}

			if (value->GetNumberCoordinates() != node_->GetNumberCoordinates())
			{
				throw std::invalid_argument("Number coordinates of value does not match number coordinates of element");
			}

			value_ = value;
		}
	} //namespace load
} //namespace dive