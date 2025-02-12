#ifndef DIVE_WEAKFORMS_PRESSURE_LOAD_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_
#define DIVE_WEAKFORMS_PRESSURE_LOAD_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_

#include "dive_weakforms_basic.hpp"
#include "dive_problems.hpp"

namespace dive
{
    namespace weakforms
    {
		using ProblemFluidPtr = problems::ProblemFluidPtr;

		LoadDistributedVolumeDivergencePressurePtr CreateWeakFormLoadDistributedVolumeDivergencePressure();

		class LoadDistributedVolumeDivergencePressure : public WeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedVolumeDivergencePressure>
		{
		public:
			virtual ~LoadDistributedVolumeDivergencePressure() = default;

			static LoadDistributedVolumeDivergencePressurePtr Create();
			LoadDistributedVolumeDivergencePressurePtr GetPtr();
			ConstLoadDistributedVolumeDivergencePressurePtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			LoadDistributedVolumeDivergencePressure() = default;

			Matrix FormMatrix_N(IElementPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementPtr element, const Vector& local) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<LoadDistributedVolumeDivergencePressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_LOAD_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_ */