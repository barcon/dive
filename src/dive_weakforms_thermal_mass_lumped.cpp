#include "dive_weakforms_thermal_mass_lumped.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace weakforms {
		MassLumpedThermalPtr CreateWeakFormMassLumpedThermal()
		{
			auto res = MassLumpedThermal::Create();

			return res;
		}
		MassLumpedThermalPtr MassLumpedThermal::Create()
		{
			class MakeSharedEnabler : public MassLumpedThermal
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassLumpedThermalPtr MassLumpedThermal::GetPtr()
		{
			return std::dynamic_pointer_cast<MassLumpedThermal>(shared_from_this());
		}
		ConstMassLumpedThermalPtr MassLumpedThermal::GetPtr() const
		{
			return const_cast<MassLumpedThermal*>(this)->GetPtr();
		}
		void MassLumpedThermal::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto NN = FormMatrix_NN(element, local, cacheIndex);
			auto rho = FormDensity(element, local, cacheIndex);
			auto cp = FormSpecificHeat(element, local, cacheIndex);
			auto trace = NN.Trace();
			auto total{ 0.0 };

			switch (element->GetNumberDimensions())
			{
			case elements::dimension_beam:
				total = std::dynamic_pointer_cast<elements::IElementBeam>(element)->Length();
				break;
			case elements::dimension_shell:
				total = std::dynamic_pointer_cast<elements::IElementShell>(element)->Area();
				break;
			case elements::dimension_solid:
				total = std::dynamic_pointer_cast<elements::IElementSolid>(element)->Volume();
				break;
			default:
				break;
			}

			output = (rho * cp) * (total / trace) * NN.Diagonal();
		}
		void MassLumpedThermal::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassLumpedThermal::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		Scalar MassLumpedThermal::FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetDensity(temperature, pressure);
		}
		Scalar MassLumpedThermal::FormSpecificHeat(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetSpecificHeat(temperature, pressure);
		}
		Matrix MassLumpedThermal::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
	} // namespace problems
} // namespace dive