#ifndef DIVE_LOADS_DISTRIBUTED_EDGE_HPP_
#define DIVE_LOADS_DISTRIBUTED_EDGE_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedEdgePtr CreateLoadDistributedEdge(IElementPtr element, EdgeIndex edgeIndex, IVector3DPtr value);

		class LoadDistributedEdge : public ILoadDistributedEdge, virtual public std::enable_shared_from_this<LoadDistributedEdge>
		{
		public:
			virtual ~LoadDistributedEdge() = default;

			static LoadDistributedEdgePtr Create();
			LoadDistributedEdgePtr GetPtr();
			ConstLoadDistributedEdgePtr GetPtr() const;

			Type GetType() const override;
			IElementPtr GetElement() const override;
			Vector GetValue(const Vector& point) const;
			EdgeIndex GetEdgeIndex() const;

			void SetElement(IElementPtr element) override;
			void SetEdgeIndex(EdgeIndex edgeIndex);
			void SetValue(IVector3DPtr value);

		protected:
			LoadDistributedEdge() = default;

			const Type type_{ load_distributedEdge };

			EdgeIndex edgeIndex_{ 0 };
			IVector3DPtr value_{ nullptr };
			IElementPtr element_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_DISTRIBUTED_EDGE_HPP_ */