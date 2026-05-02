#ifndef DIVE_WEAKFORM_FLUID_STABILIZATION_HPP_
#define DIVE_WEAKFORM_FLUID_STABILIZATION_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		StabilizationFluidPtr CreateWeakFormStabilizationFluid();

		class StabilizationFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<StabilizationFluid>
		{
		public:
			virtual ~StabilizationFluid() = default;

			static StabilizationFluidPtr Create();
			StabilizationFluidPtr GetPtr();
			ConstStabilizationFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

		protected:
			StabilizationFluid() = default;

			Matrix FormMomentum(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_udN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			using std::enable_shared_from_this<StabilizationFluid>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORMFLUID_STABILIZATION_HPP_ */