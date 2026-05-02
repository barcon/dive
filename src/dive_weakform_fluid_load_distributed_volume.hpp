#ifndef DIVE_WEAKFORM_FLUID_LOAD_DISTRIBUTED_VOLUME_HPP_
#define DIVE_WEAKFORM_FLUID_LOAD_DISTRIBUTED_VOLUME_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		LoadDistributedVolumeFluidPtr CreateWeakFormLoadDistributedVolumeFluid();

		class LoadDistributedVolumeFluid : public IWeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedVolumeFluid>
		{
		public:
			virtual ~LoadDistributedVolumeFluid() = default;

			static LoadDistributedVolumeFluidPtr Create();
			LoadDistributedVolumeFluidPtr GetPtr();
			ConstLoadDistributedVolumeFluidPtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

		protected:
			LoadDistributedVolumeFluid() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& point) const;

			using std::enable_shared_from_this<LoadDistributedVolumeFluid>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_FLUID_LOAD_DISTRIBUTED_VOLUME_HPP_ */