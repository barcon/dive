#ifndef DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME__STABILIZATION_HPP_
#define DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME__STABILIZATION_HPP_

#include "dive_weakforms_basic.hpp"
#include "dive_timers.hpp"

namespace dive
{
    namespace weakforms
    {
		LoadDistributedVolumeStabilizationFluidPtr CreateWeakFormLoadDistributedVolumeStabilizationFluid();

		class LoadDistributedVolumeStabilizationFluid : public WeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedVolumeStabilizationFluid>
		{
		public:
			virtual ~LoadDistributedVolumeStabilizationFluid() = default;

			static LoadDistributedVolumeStabilizationFluidPtr Create();
			LoadDistributedVolumeStabilizationFluidPtr GetPtr();
			ConstLoadDistributedVolumeStabilizationFluidPtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

			void SetVelocity(IMatrix3DPtr velocity);

		protected:
			LoadDistributedVolumeStabilizationFluid() = default;

			Matrix FormVelocity(IElementPtr element, const Vector& point) const;
			Scalar FormDivergence(IElementPtr element, const Vector& point) const;
			Matrix FormMatrix_N(IElementPtr element, const Vector& point) const;
			Matrix FormMatrix_udN(IElementPtr element, const Vector& point) const;


			IMatrix3DPtr velocity_{ nullptr };

			using std::enable_shared_from_this<LoadDistributedVolumeStabilizationFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME__STABILIZATION_HPP_ */