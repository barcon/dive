#ifndef DIVE_WEAKFORMS_THERMAL_STIFFNESS_HPP_
#define DIVE_WEAKFORMS_THERMAL_STIFFNESS_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		StiffnessThermalPtr CreateWeakFormStiffnessThermal();

		class StiffnessThermal : public WeakFormElement, virtual public std::enable_shared_from_this<StiffnessThermal>
		{
		public:
			virtual ~StiffnessThermal() = default;

			static StiffnessThermalPtr Create();
			StiffnessThermalPtr GetPtr();
			ConstStiffnessThermalPtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);

		protected:
			StiffnessThermal() = default;

			Matrix FormMatrix_K(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };

			using std::enable_shared_from_this<StiffnessThermal>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_THERMAL_STIFFNESS_HPP_ */