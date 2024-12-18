#ifndef DIVE_LOADS_DISTRIBUTED_EDGE_HPP_
#define DIVE_LOADS_DISTRIBUTED_EDGE_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedEdgePtr CreateLoadDistributedEdge(IElementPtr element, EdgeIndex edgeIndex, DofIndex dofIndex, Scalar(*function)(Scalar, Scalar, Scalar));

		class LoadDistributedEdge : public ILoad, virtual public std::enable_shared_from_this<LoadDistributedEdge>
		{
		public:
			virtual ~LoadDistributedEdge() = default;

			static LoadDistributedEdgePtr Create();
			LoadDistributedEdgePtr GetPtr();
			ConstLoadDistributedEdgePtr GetPtr() const;

			Type GetType() const override;
			IElementPtr GetElement() const override;
			DofIndex GetDofIndex() const override;
			Scalar GetValue(Scalar x, Scalar y, Scalar z) const override;
			Scalar GetValue(const Vector& point) const override;
			EdgeIndex GetEdgeIndex() const;

			void SetElement(IElementPtr element) override;
			void SetDofIndex(DofIndex dofIndex) override;
			void SetEdgeIndex(EdgeIndex edgeIndex);
			void SetValue(IScalar3DPtr value) override;

		protected:
			LoadDistributedEdge() = default;

			const Type type_{ load_distributedEdge };

			DofIndex dofIndex_{ 0 };
			EdgeIndex edgeIndex_{ 0 };
			IScalar3DPtr value_{ nullptr };
			IElementPtr element_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_DISTRIBUTED_EDGE_HPP_ */