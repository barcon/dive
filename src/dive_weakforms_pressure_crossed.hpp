#ifndef DIVE_WEAKFORMS_PRESSURE_CROSSED_HPP_
#define DIVE_WEAKFORMS_PRESSURE_CROSSED_HPP_

#include "dive_weakforms.hpp"
#include "dive_problems.hpp"

namespace dive
{
    namespace weakforms
    {
		CrossedPressurePtr CreateWeakFormCrossedPressure();

		class CrossedPressure : public IWeakFormElement, virtual public std::enable_shared_from_this<CrossedPressure>
		{
		public:
			virtual ~CrossedPressure() = default;

			static CrossedPressurePtr Create();
			CrossedPressurePtr GetPtr();
			ConstCrossedPressurePtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			CrossedPressure() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local) const;
			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<CrossedPressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_CROSSED_HPP_ */