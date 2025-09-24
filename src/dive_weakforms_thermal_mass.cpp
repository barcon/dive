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
		void MassThermal::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto NN = FormMatrix_NN(element, local, cacheIndex);
			auto rho = FormDensity(element, local, cacheIndex);
			auto cp = FormSpecificHeat(element, local, cacheIndex);

			output = (rho * cp) * NN;
		}
		void MassThermal::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassThermal::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		Scalar MassThermal::FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetDensity(temperature, pressure);
		}
		Scalar MassThermal::FormSpecificHeat(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetSpecificHeat(temperature, pressure);
		}
		Matrix MassThermal::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
	} // namespace problems
} // namespace dive