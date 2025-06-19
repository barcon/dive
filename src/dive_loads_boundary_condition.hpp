#ifndef DIVE_LOADS_BOUNDARY_CONDITION_HPP_
#define DIVE_LOADS_BOUNDARY_CONDITION_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		BoundaryConditionPtr CreateBoundaryCondition(INodePtr node, DofIndex dofIndex, IScalar3DPtr value);

		class BoundaryCondition : public IBoundaryCondition, virtual public std::enable_shared_from_this<BoundaryCondition>
		{
		public:
			virtual ~BoundaryCondition() = default;

			static BoundaryConditionPtr Create();
			BoundaryConditionPtr GetPtr();
			ConstBoundaryConditionPtr GetPtr() const;

			Type GetType() const override;
			INodePtr GetNode() const override;
			DofIndex GetDofIndex() const override;
			Scalar GetValue() const override;

			void SetNode(INodePtr node) override;
			void SetDofIndex(DofIndex dofIndex) override;
			void SetValue(IScalar3DPtr value) override;

		protected:
			BoundaryCondition() = default;

			const Type type_{ load_boundary_condition };

			INodePtr node_{ nullptr };
			DofIndex dofIndex_{ 0 };
			IScalar3DPtr value_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_BOUNDARY_CONDITION_HPP_ */