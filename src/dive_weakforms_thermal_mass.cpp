#include "dive_weakforms_thermal_mass.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace weakforms {
		MassThermalPtr CreateWeakFormMassThermal()
		{
			auto res = MassThermal::Create();

			return res;
		}
		MassThermalPtr MassThermal::Create()
		{
			class MakeSharedEnabler : public MassThermal
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassThermalPtr MassThermal::GetPtr()
		{
			return std::dynamic_pointer_cast<MassThermal>(shared_from_this());
		}
		ConstMassThermalPtr MassThermal::GetPtr() const
		{
			return const_cast<MassThermal*>(this)->GetPtr();
		}
		void MassThermal::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto rho = FormDensity(element, local);
			auto cp = FormSpecificHeat(element, local);

			output = N.Transpose() * (rho * cp) * N;
		}
		void MassThermal::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassThermal::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		Scalar MassThermal::FormDensity(IElementPtr element, const Vector& local) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetDensity(temperature, pressure);
		}
		Scalar MassThermal::FormSpecificHeat(IElementPtr element, const Vector& local) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetSpecificHeat(temperature, pressure);
		}
		Matrix MassThermal::FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->N(local, cacheIndex);
		}
	} // namespace problems
} // namespace dive