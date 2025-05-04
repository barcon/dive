#ifndef DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_EDGE_HPP_
#define DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_EDGE_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		LoadDistributedEdgeStructuralPtr CreateWeakFormLoadDistributedEdgeStructural();

		class LoadDistributedEdgeStructural : public IWeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedEdgeStructural>
		{
		public:
			virtual ~LoadDistributedEdgeStructural() = default;

			static LoadDistributedEdgeStructuralPtr Create();
			LoadDistributedEdgeStructuralPtr GetPtr();
			ConstLoadDistributedEdgeStructuralPtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

		protected:
			LoadDistributedEdgeStructural() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& point) const;

			using std::enable_shared_from_this<LoadDistributedEdgeStructural>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_EDGE_HPP_ */