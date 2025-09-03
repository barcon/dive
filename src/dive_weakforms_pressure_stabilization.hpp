#ifndef DIVE_WEAKFORMS_PRESSURE_STABILIZATION_HPP_
#define DIVE_WEAKFORMS_PRESSURE_STABILIZATION_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		StabilizationPressurePtr CreateWeakFormStabilizationPressure();

		class StabilizationPressure : public IWeakFormElement, virtual public std::enable_shared_from_this<StabilizationPressure>
		{
		public:
			virtual ~StabilizationPressure() = default;

			static StabilizationPressurePtr Create();
			StabilizationPressurePtr GetPtr();
			ConstStabilizationPressurePtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			StabilizationPressure() = default;

			Matrix FormMomentum(IElementMappedPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local) const;
			Matrix FormMatrix_udN(IElementMappedPtr element, const Vector& local) const;
			Matrix FormMatrix_dNp(IElementMappedPtr element, const Vector& local) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<StabilizationPressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_STABILIZATION_HPP_ */