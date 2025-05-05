#ifndef DIVE_WEAKFORMS_THERMAL_STABILIZATION_HPP_
#define DIVE_WEAKFORMS_THERMAL_STABILIZATION_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		StabilizationThermalPtr CreateWeakFormStabilizationThermal();

		class StabilizationThermal : public IWeakFormElement, virtual public std::enable_shared_from_this<StabilizationThermal>
		{
		public:
			virtual ~StabilizationThermal() = default;

			static StabilizationThermalPtr Create();
			StabilizationThermalPtr GetPtr();
			ConstStabilizationThermalPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);
			
			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			StabilizationThermal() = default;

			Scalar FormDensity(IElementMappedPtr element, const Vector& local) const;
			Scalar FormSpecificHeat(IElementMappedPtr element, const Vector& local) const;
			Matrix FormVelocity(IElementMappedPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };
			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<StabilizationThermal>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_THERMAL_STABILIZATION_HPP_ */