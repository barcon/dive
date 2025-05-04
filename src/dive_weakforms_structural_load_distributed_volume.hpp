#ifndef DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_VOLUME_HPP_
#define DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_VOLUME_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		LoadDistributedVolumeStructuralPtr CreateWeakFormLoadDistributedVolumeStructural();

		class LoadDistributedVolumeStructural : public IWeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedVolumeStructural>
		{
		public:
			virtual ~LoadDistributedVolumeStructural() = default;

			static LoadDistributedVolumeStructuralPtr Create();
			LoadDistributedVolumeStructuralPtr GetPtr();
			ConstLoadDistributedVolumeStructuralPtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

		protected:
			LoadDistributedVolumeStructural() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& point) const;

			using std::enable_shared_from_this<LoadDistributedVolumeStructural>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_VOLUME_HPP_ */