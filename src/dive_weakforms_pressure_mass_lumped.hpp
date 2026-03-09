#ifndef DIVE_WEAKFORMS_PRESSURE_MASS_LUMPED_HPP_
#define DIVE_WEAKFORMS_PRESSURE_MASS_LUMPED_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		MassLumpedPressurePtr CreateWeakFormMassLumpedPressure();

		class MassLumpedPressure : public IWeakFormElement, virtual public std::enable_shared_from_this<MassLumpedPressure>
		{
		public:
			virtual ~MassLumpedPressure() = default;

			static MassLumpedPressurePtr Create();
			MassLumpedPressurePtr GetPtr();
			ConstMassLumpedPressurePtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);
			void SetVelocity(IMatrix3DPtr velocity);

		protected:
			MassLumpedPressure() = default;

			Matrix FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormKineticViscosity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };
			IMatrix3DPtr velocity_{ nullptr };

			using std::enable_shared_from_this<MassLumpedPressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_MASS_LUMPED_HPP_ */