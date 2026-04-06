#ifndef DIVE_WEAKFORMS_PRESSURE_MASS_HPP_
#define DIVE_WEAKFORMS_PRESSURE_MASS_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		MassPressurePtr CreateWeakFormMassPressure();

		class MassPressure : public IWeakFormElement, virtual public std::enable_shared_from_this<MassPressure>
		{
		public:
			virtual ~MassPressure() = default;

			static MassPressurePtr Create();
			MassPressurePtr GetPtr();
			ConstMassPressurePtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalarCoordinatesPtr temperature);
			void SetPressure(IScalarCoordinatesPtr pressure);
			void SetVelocity(IMatrixCoordinatesPtr velocity);

		protected:
			MassPressure() = default;

			Matrix FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormKineticViscosity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };
			IMatrixCoordinatesPtr velocity_{ nullptr };

			using std::enable_shared_from_this<MassPressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_MASS_HPP_ */