#include "dive_weakforms_structural_mass_lumped.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace weakforms {
		MassLumpedStructuralPtr CreateWeakFormMassLumpedStructural()
		{
			auto res = MassLumpedStructural::Create();

			return res;
		}
		MassLumpedStructuralPtr MassLumpedStructural::Create()
		{
			class MakeSharedEnabler : public MassLumpedStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassLumpedStructuralPtr MassLumpedStructural::GetPtr()
		{
			return std::dynamic_pointer_cast<MassLumpedStructural>(shared_from_this());
		}
		ConstMassLumpedStructuralPtr MassLumpedStructural::GetPtr() const
		{
			return const_cast<MassLumpedStructural*>(this)->GetPtr();
		}
		void MassLumpedStructural::WeakFormulation(IElementMappedPtr element, const Vector& point, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto NN = FormMatrix_NN(element, point, cacheIndex);
			auto rho = FormDensity(element, point, cacheIndex);
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

			output = rho * (total / trace) * NN.Diagonal();
		}
		void MassLumpedStructural::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassLumpedStructural::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		Matrix MassLumpedStructural::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
		Scalar MassLumpedStructural::FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto material = std::static_pointer_cast<material::IMaterialSolid>(element->GetMaterial());
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return material->GetDensity(temperature, pressure);
		}
	} // namespace problems
} // namespace dive