#include "dive_weakforms_thermal_convection.hpp"
#include "dive_problems.hpp"
#include "dive_values_scalar_congruent.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		ConvectionThermalPtr CreateWeakFormConvectionThermal()
		{
			auto res = ConvectionThermal::Create();

			return res;
		}
		ConvectionThermalPtr ConvectionThermal::Create()
		{
			class MakeSharedEnabler : public ConvectionThermal
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ConvectionThermalPtr ConvectionThermal::GetPtr()
		{
			return std::dynamic_pointer_cast<ConvectionThermal>(shared_from_this());
		}
		ConstConvectionThermalPtr ConvectionThermal::GetPtr() const
		{
			return const_cast<ConvectionThermal*>(this)->GetPtr();
		}
		void ConvectionThermal::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			
			auto rho = FormDensity(element, local, cacheIndex);
			auto cp = FormSpecificHeat(element, local, cacheIndex);
			auto u = FormVelocity(element, local, cacheIndex);
			auto divergence = FormDivergence(element, local, cacheIndex);

			output = N.Transpose() * (rho * cp) * (divergence * N + u.Transpose() * dN);
		}
		void ConvectionThermal::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void ConvectionThermal::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		void ConvectionThermal::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Scalar ConvectionThermal::FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetDensity(temperature, pressure);
		}
		Scalar ConvectionThermal::FormSpecificHeat(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return element->GetMaterial()->GetSpecificHeat(temperature, pressure);
		}
		Matrix ConvectionThermal::FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			const auto& elementIndex = element->GetElementIndex();
			const auto& elementVelocity = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[elementIndex]);

			return elementVelocity->u(local);
		}
		Scalar ConvectionThermal::FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
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
		Matrix ConvectionThermal::FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{	
			return element->N(cacheIndex);
		}
		Matrix ConvectionThermal::FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{		
			return element->InvJ(cacheIndex) * element->dN(cacheIndex);
		}
	} // namespace problems
} // namespace dive