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
			IElementMappedPtr GetElement() const override;
			Vector GetValue(const Vector& point) const;
			EdgeIndex GetEdgeIndex() const;

			void SetElement(IElementMappedPtr element) override;
			void SetEdgeIndex(EdgeIndex edgeIndex);
			void SetValue(IVector3DPtr value);

		protected:
			LoadDistributedEdge() = default;

			const Type type_{ load_distributed_edge };

			EdgeIndex edgeIndex_{ 0 };
			IVector3DPtr value_{ nullptr };
			IElementMappedPtr element_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_DISTRIBUTED_EDGE_HPP_ */