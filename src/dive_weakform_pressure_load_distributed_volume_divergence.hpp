#ifndef DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_
#define DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_

#include "dive_weakform.hpp"
#include "dive_problem.hpp"

namespace dive
{
    namespace weakform
    {
		LoadDistributedVolumeDivergencePressurePtr CreateLoadDistributedVolumeDivergencePressure();

		class LoadDistributedVolumeDivergencePressure : public IWeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedVolumeDivergencePressure>
		{
		public:
			virtual ~LoadDistributedVolumeDivergencePressure() = default;

			static LoadDistributedVolumeDivergencePressurePtr Create();
			LoadDistributedVolumeDivergencePressurePtr GetPtr();

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			LoadDistributedVolumeDivergencePressure() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<LoadDistributedVolumeDivergencePressure>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_VOLUME_DIVERGENCE_HPP_ */