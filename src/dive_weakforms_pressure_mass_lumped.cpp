#include "dive_weakforms_pressure_mass_lumped.hpp"
#include "dive_values_scalar_congruent.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		MassLumpedPressurePtr CreateWeakFormMassLumpedPressure()
		{
			auto res = MassLumpedPressure::Create();

			return res;
		}
		MassLumpedPressurePtr MassLumpedPressure::Create()
		{
			class MakeSharedEnabler : public MassLumpedPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassLumpedPressurePtr MassLumpedPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<MassLumpedPressure>(shared_from_this());
		}
		ConstMassLumpedPressurePtr MassLumpedPressure::GetPtr() const
		{
			return const_cast<MassLumpedPressure*>(this)->GetPtr();
		}
		void MassLumpedPressure::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			Scalar uconv{ 0.0 };
			Scalar udiff{ 0.0 };
			Scalar beta{ 0.0 };

			auto NN = FormMatrix_NN(element, local, cacheIndex);
			auto nu = FormKineticViscosity(element, local, cacheIndex);
			auto u = FormVelocity(element, local, cacheIndex);
			auto h = element->SizeMinimum();

			auto trace = NN.Trace();
			auto total{ 0.0 };

			uconv = nu / h;
			udiff = eilig::NormP2(u);
			beta = std::max({ 0.5, uconv, udiff });

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

			output = (1.0 / (beta * beta)) * (total / trace) * NN.Diagonal();
		}
		void MassLumpedPressure::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassLumpedPressure::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		void MassLumpedPressure::SetVelocity(IMatrix3DPtr velocity)
		{
			velocity_ = velocity;
		}
		Matrix MassLumpedPressure::FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return values::GetValueMatrix3D(velocity_, local, element) ;
		}
		Scalar MassLumpedPressure::FormKineticViscosity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto material = std::static_pointer_cast<material::IMaterialFluid>(element->GetMaterial());
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			auto dynamicViscosity = material->GetDynamicViscosity(temperature, pressure);
			auto density = material->GetDensity(temperature, pressure);

			return dynamicViscosity / density;
		}
		Matrix MassLumpedPressure::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
	} // namespace problems
} // namespace dive