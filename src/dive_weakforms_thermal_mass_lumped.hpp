#ifndef DIVE_WEAKFORMS_THERMAL_MASS_LUMPED_HPP_
#define DIVE_WEAKFORMS_THERMAL_MASS_LUMPED_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		MassLumpedThermalPtr CreateWeakFormMassLumpedThermal();

		class MassLumpedThermal : public IWeakFormElement, virtual public std::enable_shared_from_this<MassLumpedThermal>
		{
		public:
			virtual ~MassLumpedThermal() = default;

			static MassLumpedThermalPtr Create();
			MassLumpedThermalPtr GetPtr();
			ConstMassLumpedThermalPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);

		protected:
			MassLumpedThermal() = default;

			Scalar FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormSpecificHeat(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };

			using std::enable_shared_from_this<MassLumpedThermal>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_THERMAL_MASS_LUMPED_HPP_ */