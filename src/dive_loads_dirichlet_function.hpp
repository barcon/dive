#ifndef DIVE_LOADS_DIRICHLET_FUNCTION_HPP_
#define DIVE_LOADS_DIRICHLET_FUNCTION_HPP_

#include "dive_loads.hpp"
#include "dive_problems.hpp"

namespace dive
{
	namespace loads
	{
		DirichletFunctionPtr CreateDirichletFunction();
		DirichletFunctionPtr CreateDirichletFunction(IProblemPtr problem, INodePtr node, DofIndex dofIndex, Scalar value);

		class DirichletFunction : public IDirichlet, virtual public std::enable_shared_from_this<DirichletFunction>
		{
		public:
			virtual ~DirichletFunction() = default;

			static DirichletFunctionPtr Create();
			DirichletFunctionPtr GetPtr();
			ConstDirichletFunctionPtr GetPtr() const;

			Type GetType() const override;
			INodePtr GetNode() const override;
			DofIndex GetDofIndex() const override;
			Scalar GetValue(Scalar time) const override;

			void SetNode(INodePtr node) override;
			void SetDofIndex(DofIndex dofIndex) override;
			void SetValue(DofIndex dofIndex, Scalar value);

		protected:
			DirichletFunction() = default;

			const Type type_{ load_dirichlet };

			INodePtr node_{ nullptr };
			DofIndex dofIndex_{ 0 };
			Scalar value_{ 0.0 };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_DIRICHLET_FUNCTION_HPP_ */