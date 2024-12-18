#ifndef DIVE_WEAKFORMS_THERMAL_STABILIZATION_HPP_
#define DIVE_WEAKFORMS_THERMAL_STABILIZATION_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		StabilizationThermalPtr CreateWeakFormStabilizationThermal();

		class StabilizationThermal : public WeakFormElement, virtual public std::enable_shared_from_this<StabilizationThermal>
		{
		public:
			virtual ~StabilizationThermal() = default;

			static StabilizationThermalPtr Create();
			StabilizationThermalPtr GetPtr();
			ConstStabilizationThermalPtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);
			void SetVelocity(IMatrix3DPtr velocity);

		protected:
			StabilizationThermal() = default;

			Scalar FormDensity(IElementPtr element, const Vector& local) const;
			Scalar FormSpecificHeat(IElementPtr element, const Vector& local) const;
			Matrix FormVelocity(IElementPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };
			IMatrix3DPtr velocity_{ nullptr };

			using std::enable_shared_from_this<StabilizationThermal>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_THERMAL_STABILIZATION_HPP_ */