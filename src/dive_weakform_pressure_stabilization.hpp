#ifndef DIVE_WEAKFORM_PRESSURE_STABILIZATION_HPP_
#define DIVE_WEAKFORM_PRESSURE_STABILIZATION_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		StabilizationPressurePtr CreateWeakFormStabilizationPressure();

		class StabilizationPressure : public IWeakFormElement, virtual public std::enable_shared_from_this<StabilizationPressure>
		{
		public:
			virtual ~StabilizationPressure() = default;

			static StabilizationPressurePtr Create();
			StabilizationPressurePtr GetPtr();
			ConstStabilizationPressurePtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			StabilizationPressure() = default;

			Matrix FormMomentum(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_udN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_dNp(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<StabilizationPressure>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_PRESSURE_STABILIZATION_HPP_ */