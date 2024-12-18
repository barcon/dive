#ifndef DIVE_WEAKFORMS_THERMAL_MASS_HPP_
#define DIVE_WEAKFORMS_THERMAL_MASS_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		MassThermalPtr CreateWeakFormMassThermal();

		class MassThermal : public WeakFormElement, virtual public std::enable_shared_from_this<MassThermal>
		{
		public:
			virtual ~MassThermal() = default;

			static MassThermalPtr Create();
			MassThermalPtr GetPtr();
			ConstMassThermalPtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);

		protected:
			MassThermal() = default;

			Scalar FormDensity(IElementPtr element, const Vector& local) const;
			Scalar FormSpecificHeat(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };

			using std::enable_shared_from_this<MassThermal>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_THERMAL_MASS_HPP_ */