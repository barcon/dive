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
		void StiffnessThermal::SetTemperature(IScalarCoordinatesPtr temperature)
		{
			temperature_ = temperature;
		}
		void StiffnessThermal::SetPressure(IScalarCoordinatesPtr pressure)
		{
			pressure_ = pressure;
		}
		Matrix StiffnessThermal::FormMatrix_K(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto state = Vector(2);
			state(0) = values::GetValueScalarCoordinates(temperature_, local, element);
			state(1) = values::GetValueScalarCoordinates(pressure_, local, element);

			return element->GetMaterial()->K(state);
		}
		Matrix StiffnessThermal::FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->InvJ(cacheIndex) * element->dN(cacheIndex);
		}
	} // namespace problems
} // namespace dive