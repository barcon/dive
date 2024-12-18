#ifndef DIVE_WEAKFORMS_PRESSURE_STABILIZATION_HPP_
#define DIVE_WEAKFORMS_PRESSURE_STABILIZATION_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		StabilizationPressurePtr CreateWeakFormStabilizationPressure();

		class StabilizationPressure : public WeakFormElement, virtual public std::enable_shared_from_this<StabilizationPressure>
		{
		public:
			virtual ~StabilizationPressure() = default;

			static StabilizationPressurePtr Create();
			StabilizationPressurePtr GetPtr();
			ConstStabilizationPressurePtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetProblemVelocity(IProblemPtr problemVelocity);

		protected:
			StabilizationPressure() = default;

			Matrix FormVelocity(IElementPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_N(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_udN(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_dNp(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			IProblemPtr problemVelocity_{ nullptr };

			using std::enable_shared_from_this<StabilizationPressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_STABILIZATION_HPP_ */