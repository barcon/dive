#ifndef DIVE_WEAKFORMS_FLUID_STIFFNESS_HPP_
#define DIVE_WEAKFORMS_FLUID_STIFFNESS_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		StiffnessFluidPtr CreateWeakFormStiffnessFluid();

		class StiffnessFluid : public WeakFormElement, virtual public std::enable_shared_from_this<StiffnessFluid>
		{
		public:
			virtual ~StiffnessFluid() = default;

			static StiffnessFluidPtr Create();
			StiffnessFluidPtr GetPtr();
			ConstStiffnessFluidPtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);

		protected:
			StiffnessFluid() = default;

			Matrix FormMatrix_Id(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_B(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Vector FormVector_Z(IElementPtr element, const Vector& local) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };

			using std::enable_shared_from_this<StiffnessFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_STIFFNESS_HPP_ */