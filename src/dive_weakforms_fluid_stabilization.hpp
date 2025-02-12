#ifndef DIVE_WEAKFORMS_FLUID_STABILIZATION_HPP_
#define DIVE_WEAKFORMS_FLUID_STABILIZATION_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		StabilizationFluidPtr CreateWeakFormStabilizationFluid();

		class StabilizationFluid : public WeakFormElement, virtual public std::enable_shared_from_this<StabilizationFluid>
		{
		public:
			virtual ~StabilizationFluid() = default;

			static StabilizationFluidPtr Create();
			StabilizationFluidPtr GetPtr();
			ConstStabilizationFluidPtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

		protected:
			StabilizationFluid() = default;

			Matrix FormMomentum(IElementPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_udN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			using std::enable_shared_from_this<StabilizationFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_STABILIZATION_HPP_ */