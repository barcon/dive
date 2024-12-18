#ifndef DIVE_LOADS_DIRICHLET_HPP_
#define DIVE_LOADS_DIRICHLET_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		DirichletPtr CreateDirichletByValue(INodePtr node, DofIndex dofIndex, Scalar value);
		DirichletPtr CreateDirichletByFunction(INodePtr node, DofIndex dofIndex, Scalar(*function)(Scalar, Scalar, Scalar));

		class Dirichlet : public IDirichlet, virtual public std::enable_shared_from_this<Dirichlet>
		{
		public:
			virtual ~Dirichlet() = default;

			static DirichletPtr Create();
			DirichletPtr GetPtr();
			ConstDirichletPtr GetPtr() const;

			Type GetType() const override;
			INodePtr GetNode() const override;
			DofIndex GetDofIndex() const override;
			Scalar GetValue() const override;

			void SetNode(INodePtr node) override;
			void SetDofIndex(DofIndex dofIndex) override;
			void SetValue(IScalar3DPtr value) override;

		protected:
			Dirichlet() = default;

			const Type type_{ load_dirichlet };

			INodePtr node_{ nullptr };
			DofIndex dofIndex_{ 0 };
			IScalar3DPtr value_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_DIRICHLET_HPP_ */