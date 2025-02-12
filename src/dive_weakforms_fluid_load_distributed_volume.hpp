#ifndef DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME_HPP_
#define DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		LoadDistributedVolumeFluidPtr CreateWeakFormLoadDistributedVolumeFluid();

		class LoadDistributedVolumeFluid : public WeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedVolumeFluid>
		{
		public:
			virtual ~LoadDistributedVolumeFluid() = default;

			static LoadDistributedVolumeFluidPtr Create();
			LoadDistributedVolumeFluidPtr GetPtr();
			ConstLoadDistributedVolumeFluidPtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

		protected:
			LoadDistributedVolumeFluid() = default;

			Matrix FormMatrix_N(IElementPtr element, const Vector& point) const;

			using std::enable_shared_from_this<LoadDistributedVolumeFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME_HPP_ */