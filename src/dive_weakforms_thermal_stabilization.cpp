#include "dive_weakforms_thermal_stabilization.hpp"
#include "dive_values_scalar_congruent.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		StabilizationThermalPtr CreateWeakFormStabilizationThermal()
		{
			auto res = StabilizationThermal::Create();

			return res;
		}
		StabilizationThermalPtr StabilizationThermal::Create()
		{
			class MakeSharedEnabler : public StabilizationThermal
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		StabilizationThermalPtr StabilizationThermal::GetPtr()
		{
			return std::dynamic_pointer_cast<StabilizationThermal>(shared_from_this());
		}
		ConstStabilizationThermalPtr StabilizationThermal::GetPtr() const
		{
			return const_cast<StabilizationThermal*>(this)->GetPtr();
		}
		void StabilizationThermal::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			auto rho = FormDensity(element, local);
			auto cp = FormSpecificHeat(element, local);
			auto u = FormVelocity(element, local);
			auto du = FormDivergence(element, local);

			output = -(1.0 / 2.0) * (u.Transpose() * dN + du * N).Transpose() * (rho * cp) * dN;
		}
		void StabilizationThermal::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void StabilizationThermal::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}		
		void StabilizationThermal::SetVelocity(IMatrix3DPtr velocity)
		{
			velocity_ = velocity;
		}
		Scalar StabilizationThermal::FormDensity(IElementPtr element, const Vector& local) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetDensity(temperature, pressure);
		}
		Scalar StabilizationThermal::FormSpecificHeat(IElementPtr element, const Vector& local) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetSpecificHeat(temperature, pressure);
		}
		Matrix StabilizationThermal::FormVelocity(IElementPtr element, const Vector& local) const
		{
			return values::GetValueMatrix3D(velocity_, local, element);
		}
		Scalar StabilizationThermal::FormDivergence(IElementPtr element, const Vector& local) const
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
		Matrix StabilizationThermal::FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->N(local, cacheIndex);
		}
		Matrix StabilizationThermal::FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->InvJ(local, cacheIndex) * element->dN(local, cacheIndex);
		}
	} // namespace problems
} // namespace dive