#ifndef DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME_CORRECTION_HPP_
#define DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME_CORRECTION_HPP_

#include "dive_weakforms_basic.hpp"
#include "dive_problems.hpp"

namespace dive
{
    namespace weakforms
    {
		LoadDistributedVolumeCorrectionFluidPtr CreateWeakFormLoadDistributedVolumeCorrectionFluid();

		class LoadDistributedVolumeCorrectionFluid : public WeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedVolumeCorrectionFluid>
		{
		public:
			virtual ~LoadDistributedVolumeCorrectionFluid() = default;

			static LoadDistributedVolumeCorrectionFluidPtr Create();
			LoadDistributedVolumeCorrectionFluidPtr GetPtr();
			ConstLoadDistributedVolumeCorrectionFluidPtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

			void SetProblemPressure(IProblemPtr problemPressure);

		protected:
			LoadDistributedVolumeCorrectionFluid() = default;

			Scalar FormPressure(IElementPtr element, const Vector& point) const;
			Vector FormVector_dN(IElementPtr element, const Vector& point) const;

			IProblemPtr problemPressure_{ nullptr };

			using std::enable_shared_from_this<LoadDistributedVolumeCorrectionFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_LOAD_DISTRIBUTED_VOLUME_CORRECTION_HPP_ */