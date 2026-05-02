#ifndef DIVE_WEAKFORM_THERMAL_STABILIZATION_HPP_
#define DIVE_WEAKFORM_THERMAL_STABILIZATION_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		StabilizationThermalPtr CreateWeakFormStabilizationThermal();

		class StabilizationThermal : public IWeakFormElement, virtual public std::enable_shared_from_this<StabilizationThermal>
		{
		public:
			virtual ~StabilizationThermal() = default;

			static StabilizationThermalPtr Create();
			StabilizationThermalPtr GetPtr();
			ConstStabilizationThermalPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalarCoordinatesPtr temperature);
			void SetPressure(IScalarCoordinatesPtr pressure);
			
			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			StabilizationThermal() = default;

			Scalar FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormSpecificHeat(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };
			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<StabilizationThermal>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_THERMAL_STABILIZATION_HPP_ */