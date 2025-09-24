#include "dive_weakforms_structural_mass.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace weakforms {
		MassStructuralPtr CreateWeakFormMassStructural()
		{
			auto res = MassStructural::Create();

			return res;
		}
		MassStructuralPtr MassStructural::Create()
		{
			class MakeSharedEnabler : public MassStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassStructuralPtr MassStructural::GetPtr()
		{
			return std::dynamic_pointer_cast<MassStructural>(shared_from_this());
		}
		ConstMassStructuralPtr MassStructural::GetPtr() const
		{
			return const_cast<MassStructural*>(this)->GetPtr();
		}
		void MassStructural::WeakFormulation(IElementMappedPtr element, const Vector& point, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto NN = FormMatrix_NN(element, point, cacheIndex);
			auto rho = FormDensity(element, point, cacheIndex);

			output = rho * NN;
		}
		void MassStructural::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassStructural::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		Matrix MassStructural::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
		Scalar MassStructural::FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto material = std::static_pointer_cast<material::IMaterialSolid>(element->GetMaterial());
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return material->GetDensity(temperature, pressure);
		}
	} // namespace problems
} // namespace dive