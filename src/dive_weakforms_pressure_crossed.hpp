#ifndef DIVE_WEAKFORMS_PRESSURE_CROSSED_HPP_
#define DIVE_WEAKFORMS_PRESSURE_CROSSED_HPP_

#include "dive_weakforms_basic.hpp"
#include "dive_problems.hpp"

namespace dive
{
    namespace weakforms
    {
		using ProblemFluidPtr = problems::ProblemFluidPtr;

		CrossedPressurePtr CreateWeakFormCrossedPressure();

		class CrossedPressure : public WeakFormElement, virtual public std::enable_shared_from_this<CrossedPressure>
		{
		public:
			virtual ~CrossedPressure() = default;

			static CrossedPressurePtr Create();
			CrossedPressurePtr GetPtr();
			ConstCrossedPressurePtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetProblemVelocity(IProblemPtr problemVelocity);

		protected:
			CrossedPressure() = default;

			Matrix FormMatrix_N(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			IProblemPtr problemVelocity_{ nullptr };

			using std::enable_shared_from_this<CrossedPressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_CROSSED_HPP_ */