#ifndef DIVE_WEAKFORMS_THERMAL_CONVECTION_HPP_
#define DIVE_WEAKFORMS_THERMAL_CONVECTION_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		ConvectionThermalPtr CreateWeakFormConvectionThermal();

		class ConvectionThermal : public IWeakFormElement, virtual public std::enable_shared_from_this<ConvectionThermal>
		{
		public:
			virtual ~ConvectionThermal() = default;

			static ConvectionThermalPtr Create();
			ConvectionThermalPtr GetPtr();
			ConstConvectionThermalPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);
			
			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			ConvectionThermal() = default;

			Scalar FormDensity(IElementMappedPtr element, const Vector& local) const;
			Scalar FormSpecificHeat(IElementMappedPtr element, const Vector& local) const;
			Matrix FormVelocity(IElementMappedPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local) const;
			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };
			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<ConvectionThermal>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_THERMAL_CONVECTION_HPP_ */