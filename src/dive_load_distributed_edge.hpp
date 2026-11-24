#ifndef DIVE_LOAD_DISTRIBUTED_EDGE_HPP_
#define DIVE_LOAD_DISTRIBUTED_EDGE_HPP_

#include "dive_load.hpp"

namespace dive
{
	namespace load
	{
		LoadDistributedEdgePtr CreateLoadDistributedEdge(IElementPtr element, EdgeIndex edgeIndex, IVectorCoordinatesPtr value, IWeakFormLoadPtr weakForm);

		class LoadDistributedEdge : public ILoadDistributedEdge, virtual public std::enable_shared_from_this<LoadDistributedEdge>
		{
		public:
			virtual ~LoadDistributedEdge() = default;

			static LoadDistributedEdgePtr Create();
			LoadDistributedEdgePtr GetPtr();

			Type GetType() const override;
			IElementMappedPtr GetElement() const override;
			Vector GetValue(const Vector& point) const;
			EdgeIndex GetEdgeIndex() const;
			IWeakFormLoadPtr GetWeakForm() const override;

			void SetElement(IElementMappedPtr element) override;
			void SetEdgeIndex(EdgeIndex edgeIndex);
			void SetValue(IVectorCoordinatesPtr value);
			void SetWeakForm(IWeakFormLoadPtr weakForm) override;

		protected:
			LoadDistributedEdge() = default;

			const Type type_{ load_distributed_edge };

			EdgeIndex edgeIndex_{ 0 };
			IVectorCoordinatesPtr value_{ nullptr };
			IElementMappedPtr element_{ nullptr };
			IWeakFormLoadPtr weakForm_{ nullptr };
		};

	} //namespace load
} //namespace dive

#endif /* DIVE_LOAD_DISTRIBUTED_EDGE_HPP_ */