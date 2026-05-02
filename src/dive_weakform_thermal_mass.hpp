#ifndef DIVE_WEAKFORM_THERMAL_MASS_HPP_
#define DIVE_WEAKFORM_THERMAL_MASS_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		MassThermalPtr CreateWeakFormMassThermal();

		class MassThermal : public IWeakFormElement, virtual public std::enable_shared_from_this<MassThermal>
		{
		public:
			virtual ~MassThermal() = default;

			static MassThermalPtr Create();
			MassThermalPtr GetPtr();
			ConstMassThermalPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalarCoordinatesPtr temperature);
			void SetPressure(IScalarCoordinatesPtr pressure);

		protected:
			MassThermal() = default;

			Scalar FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormSpecificHeat(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };

			using std::enable_shared_from_this<MassThermal>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_THERMAL_MASS_HPP_ */