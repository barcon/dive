#include "dive_weakforms_thermal_stiffness.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace weakforms {
		StiffnessThermalPtr CreateWeakFormStiffnessThermal()
		{
			auto res = StiffnessThermal::Create();

			return res;
		}
		StiffnessThermalPtr StiffnessThermal::Create()
		{
			class MakeSharedEnabler : public StiffnessThermal
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		StiffnessThermalPtr StiffnessThermal::GetPtr()
		{
			return std::dynamic_pointer_cast<StiffnessThermal>(shared_from_this());
		}
		ConstStiffnessThermalPtr StiffnessThermal::GetPtr() const
		{
			return const_cast<StiffnessThermal*>(this)->GetPtr();
		}
		void StiffnessThermal::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			auto K = FormMatrix_K(element, local, cacheIndex);

			output = dN.Transpose() * K * dN;
		}
		void StiffnessThermal::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void StiffnessThermal::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		Matrix StiffnessThermal::FormMatrix_K(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->K(temperature, pressure);
		}
		Matrix StiffnessThermal::FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->InvJ(cacheIndex) * element->dN(cacheIndex);
		}
	} // namespace problems
} // namespace dive