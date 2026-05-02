#ifndef DIVE_WEAKFORM_THERMAL_STIFFNESS_HPP_
#define DIVE_WEAKFORM_THERMAL_STIFFNESS_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
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

			void SetTemperature(IScalarCoordinatesPtr temperature);
			void SetPressure(IScalarCoordinatesPtr pressure);

		protected:
			StiffnessThermal() = default;

			Matrix FormMatrix_K(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };

			using std::enable_shared_from_this<StiffnessThermal>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_THERMAL_STIFFNESS_HPP_ */