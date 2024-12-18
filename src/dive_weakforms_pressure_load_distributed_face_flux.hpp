#ifndef DIVE_WEAKFORMS_PRESSURE_LOAD_DISTRIBUTED_FACE_FLUX_HPP_
#define DIVE_WEAKFORMS_PRESSURE_LOAD_DISTRIBUTED_FACE_FLUX_HPP_

#include "dive_weakforms_basic.hpp"
#include "dive_loads_distributed_face.hpp"
#include "dive_problems.hpp"

namespace dive
{
    namespace weakforms
    {
		using ProblemFluidPtr = problems::ProblemFluidPtr;

		LoadDistributedFaceFluxPressurePtr CreateWeakFormLoadDistributedFaceFluxPressure();

		class LoadDistributedFaceFluxPressure : public WeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedFaceFluxPressure>
		{
		public:
			virtual ~LoadDistributedFaceFluxPressure() = default;

			static LoadDistributedFaceFluxPressurePtr Create();
			LoadDistributedFaceFluxPressurePtr GetPtr();
			ConstLoadDistributedFaceFluxPressurePtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;
			
			void SetMomentum(const Vector& momentum);
			void SetProblemVelocity(IProblemPtr problemVelocity);

		protected:
			LoadDistributedFaceFluxPressure() = default;
			
			Matrix FormMatrix_N(IElementPtr element, FaceIndex faceIndex, const Vector& point) const;
			Vector FormVector_n(IElementPtr element, FaceIndex faceIndex, const Vector& point) const;
			Matrix FormMatrix_q(IElementPtr element, const Vector& momentum, const Vector& point) const;

			Vector momentum_;
			IProblemPtr problemVelocity_{ nullptr };

			using std::enable_shared_from_this<LoadDistributedFaceFluxPressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_LOAD_DISTRIBUTED_FACE_FLUX_HPP_ */