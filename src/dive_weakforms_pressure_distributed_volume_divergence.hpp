#ifndef DIVE_WEAKFORMS_PRESSURE_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_
#define DIVE_WEAKFORMS_PRESSURE_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_

#include "dive_weakforms_basic.hpp"
#include "dive_problems.hpp"

namespace dive
{
    namespace weakforms
    {
		using ProblemFluidPtr = problems::ProblemFluidPtr;

		DistributedVolumeDivergencePressurePtr CreateWeakFormDistributedVolumeDivergencePressure();

		class DistributedVolumeDivergencePressure : public WeakFormElement, virtual public std::enable_shared_from_this<DistributedVolumeDivergencePressure>
		{
		public:
			virtual ~DistributedVolumeDivergencePressure() = default;

			static DistributedVolumeDivergencePressurePtr Create();
			DistributedVolumeDivergencePressurePtr GetPtr();
			ConstDistributedVolumeDivergencePressurePtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			DistributedVolumeDivergencePressure() = default;

			Matrix FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_Div(IElementPtr element, const Vector& local) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<DistributedVolumeDivergencePressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_ */