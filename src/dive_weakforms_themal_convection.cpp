#include "dive_weakforms_thermal_convection.hpp"
#include "dive_values_scalar_congruent.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		ConvectionThermalPtr CreateWeakFormConvectionThermal()
		{
			auto res = ConvectionThermal::Create();

			return res;
		}
		ConvectionThermalPtr ConvectionThermal::Create()
		{
			class MakeSharedEnabler : public ConvectionThermal
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ConvectionThermalPtr ConvectionThermal::GetPtr()
		{
			return std::dynamic_pointer_cast<ConvectionThermal>(shared_from_this());
		}
		ConstConvectionThermalPtr ConvectionThermal::GetPtr() const
		{
			return const_cast<ConvectionThermal*>(this)->GetPtr();
		}
		void ConvectionThermal::WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			
			auto rho = FormDensity(element, local);
			auto cp = FormSpecificHeat(element, local);
			auto u = FormVelocity(element, local);
			auto divergence = FormDivergence(element, local);

			output = N.Transpose() * (rho * cp) * (divergence * N + u.Transpose() * dN);
		}
		void ConvectionThermal::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void ConvectionThermal::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		void ConvectionThermal::SetVelocity(IMatrix3DPtr velocity)
		{
			velocity_ = velocity;
		}
		Scalar ConvectionThermal::FormDensity(IElementMappedPtr element, const Vector& local) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetDensity(temperature, pressure);
		}
		Scalar ConvectionThermal::FormSpecificHeat(IElementMappedPtr element, const Vector& local) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetSpecificHeat(temperature, pressure);
		}
		Matrix ConvectionThermal::FormVelocity(IElementMappedPtr element, const Vector& local) const
		{
			return values::GetValueMatrix3D(velocity_, local, element);
		}
		Scalar ConvectionThermal::FormDivergence(IElementMappedPtr element, const Vector& local) const
		{
			auto& elementVelocity = std::static_pointer_cast<values::ValueMatrix3DCongruent>(velocity_)->GetMesh()->GetElements()[element->GetElementIndex()];
			auto du = eilig::Inverse(elementVelocity->J(local)) * elementVelocity->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix ConvectionThermal::FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const
		{	
			return element->N(local, cacheIndex);
		}
		Matrix ConvectionThermal::FormMatrix_dN(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const
		{		
			return element->InvJ(local, cacheIndex) * element->dN(local, cacheIndex);
		}
	} // namespace problems
} // namespace dive