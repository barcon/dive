#include "dive_weakforms_thermal_stabilization.hpp"
#include "dive_problem.hpp"
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
		void StabilizationThermal::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			auto rho = FormDensity(element, local, cacheIndex);
			auto cp = FormSpecificHeat(element, local, cacheIndex);
			auto u = FormVelocity(element, local, cacheIndex);
			auto du = FormDivergence(element, local, cacheIndex);

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
		void StabilizationThermal::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Scalar StabilizationThermal::FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetDensity(temperature, pressure);
		}
		Scalar StabilizationThermal::FormSpecificHeat(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetSpecificHeat(temperature, pressure);
		}
		Matrix StabilizationThermal::FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			const auto& elementIndex = element->GetElementIndex();
			const auto& elementVelocity = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[elementIndex]);

			return elementVelocity->u(local);
		}
		Scalar StabilizationThermal::FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			const auto& elementIndex = element->GetElementIndex();
			const auto& elementVelocity = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[elementIndex]);
			auto du = eilig::Inverse(elementVelocity->J(local)) * elementVelocity->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix StabilizationThermal::FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->N(cacheIndex);
		}
		Matrix StabilizationThermal::FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->InvJ(cacheIndex) * element->dN(cacheIndex);
		}
	} // namespace problems
} // namespace dive