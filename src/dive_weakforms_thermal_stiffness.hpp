#ifndef DIVE_WEAKFORMS_THERMAL_STIFFNESS_HPP_
#define DIVE_WEAKFORMS_THERMAL_STIFFNESS_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		StiffnessThermalPtr CreateWeakFormStiffnessThermal();

		class StiffnessThermal : public IWeakFormElement, virtual public std::enable_shared_from_this<StiffnessThermal>
		{
		public:
			virtual ~StiffnessThermal() = default;

			static StiffnessThermalPtr Create();
			StiffnessThermalPtr GetPtr();
			ConstStiffnessThermalPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);

		protected:
			StiffnessThermal() = default;

			Matrix FormMatrix_K(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };

			using std::enable_shared_from_this<StiffnessThermal>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_THERMAL_STIFFNESS_HPP_ */