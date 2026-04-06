#include "dive_weakforms_fluid_stiffness.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace weakforms {
		StiffnessFluidPtr CreateWeakFormStiffnessFluid()
		{
			auto res = StiffnessFluid::Create();

			return res;
		}
		StiffnessFluidPtr StiffnessFluid::Create()
		{
			class MakeSharedEnabler : public StiffnessFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		StiffnessFluidPtr StiffnessFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<StiffnessFluid>(shared_from_this());
		}
		ConstStiffnessFluidPtr StiffnessFluid::GetPtr() const
		{
			return const_cast<StiffnessFluid*>(this)->GetPtr();
		}
		void StiffnessFluid::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{		
			auto B = FormMatrix_B(element, local, cacheIndex);
			auto Id = FormMatrix_Id(element, local, cacheIndex);
			auto Z = FormVector_Z(element, local, cacheIndex);

			output = eilig::ScaleByVector(B.Transpose() * Id * B, Z);
		}
		void StiffnessFluid::SetTemperature(IScalarCoordinatesPtr temperature)
		{
			temperature_ = temperature;
		}
		void StiffnessFluid::SetPressure(IScalarCoordinatesPtr pressure)
		{
			pressure_ = pressure;
		}
		Matrix StiffnessFluid::FormMatrix_Id(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto material = std::static_pointer_cast<material::IMaterialFluid>(element->GetMaterial());
			
			auto state = Vector(2);
			state(0) = values::GetValueScalarCoordinates(temperature_, local, element);
			state(1) = values::GetValueScalarCoordinates(pressure_, local, element);

			return material->D(state);
		}
		Matrix StiffnessFluid::FormMatrix_B(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto dN = element->InvJ(cacheIndex) * element->dN(cacheIndex);
			
			Matrix res(6, numberNodes * numberDof, eilig::matrix_zeros);

			for (NodeIndex i = 0; i < numberNodes; ++i)
			{
				res(0, i * numberDof + 0) = dN(0, i);
				res(1, i * numberDof + 1) = dN(1, i);
				res(2, i * numberDof + 2) = dN(2, i);

				res(3, i * numberDof + 0) = dN(1, i);
				res(3, i * numberDof + 1) = dN(0, i);

				res(4, i * numberDof + 1) = dN(2, i);
				res(4, i * numberDof + 2) = dN(1, i);

				res(5, i * numberDof + 0) = dN(2, i);
				res(5, i * numberDof + 2) = dN(0, i);
			}

			return res;
		}
		Vector StiffnessFluid::FormVector_Z(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			
			auto state = Vector(2);
			state(0) = values::GetValueScalarCoordinates(temperature_, local, element);
			state(1) = values::GetValueScalarCoordinates(pressure_, local, element);
			
			auto rho = element->GetMaterial()->GetDensity(state);
			auto rhoInv = 1. / rho;

			Vector res(numberNodes * numberDof);

			for (Index i = 0; i < res.GetRows(); ++i)
			{
				res(i) = rhoInv;
			}

			return res;
		}
	} // namespace problems
} // namespace dive