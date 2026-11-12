#ifndef DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_FACE_FLOW_HPP_
#define DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_FACE_FLOW_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		LoadDistributedFaceFlowPressurePtr CreateWeakFormLoadDistributedFaceFlowPressure();

		class LoadDistributedFaceFlowPressure : public IWeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedFaceFlowPressure>
		{
		public:
			virtual ~LoadDistributedFaceFlowPressure() = default;

			static LoadDistributedFaceFlowPressurePtr Create();
			LoadDistributedFaceFlowPressurePtr GetPtr();

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			LoadDistributedFaceFlowPressure() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& point) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<LoadDistributedFaceFlowPressure>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_FACE_FLOW_HPP_ */