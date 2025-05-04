#ifndef DIVE_WEAKFORMS_FLUID_STABILIZATION_HPP_
#define DIVE_WEAKFORMS_FLUID_STABILIZATION_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		StabilizationFluidPtr CreateWeakFormStabilizationFluid();

		class StabilizationFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<StabilizationFluid>
		{
		public:
			virtual ~StabilizationFluid() = default;

			static StabilizationFluidPtr Create();
			StabilizationFluidPtr GetPtr();
			ConstStabilizationFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

		protected:
			StabilizationFluid() = default;

			Matrix FormMomentum(IElementMappedPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_udN(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;

			using std::enable_shared_from_this<StabilizationFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_STABILIZATION_HPP_ */