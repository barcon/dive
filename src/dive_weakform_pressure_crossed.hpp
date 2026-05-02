#ifndef DIVE_WEAKFORM_PRESSURE_CROSSED_HPP_
#define DIVE_WEAKFORM_PRESSURE_CROSSED_HPP_

#include "dive_weakform.hpp"
#include "dive_problem.hpp"

namespace dive
{
    namespace weakform
    {
		CrossedPressurePtr CreateWeakFormCrossedPressure();

		class CrossedPressure : public IWeakFormElement, virtual public std::enable_shared_from_this<CrossedPressure>
		{
		public:
			virtual ~CrossedPressure() = default;

			static CrossedPressurePtr Create();
			CrossedPressurePtr GetPtr();
			ConstCrossedPressurePtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			CrossedPressure() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<CrossedPressure>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_PRESSURE_CROSSED_HPP_ */