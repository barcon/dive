#include "dive_weakforms_structural_stiffness.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace weakforms {
		StiffnessStructuralPtr CreateWeakFormStiffnessStructural()
		{
			auto res = StiffnessStructural::Create();

			return res;
		}
		StiffnessStructuralPtr StiffnessStructural::Create()
		{
			class MakeSharedEnabler : public StiffnessStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		StiffnessStructuralPtr StiffnessStructural::GetPtr()
		{
			return std::dynamic_pointer_cast<StiffnessStructural>(shared_from_this());
		}
		ConstStiffnessStructuralPtr StiffnessStructural::GetPtr() const
		{
			return const_cast<StiffnessStructural*>(this)->GetPtr();
		}
		void StiffnessStructural::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{		
			auto B = FormMatrix_B(element, local, cacheIndex);
			auto D = FormMatrix_D(element, local, cacheIndex);

			output = B.Transpose() * D * B;
		}
		void StiffnessStructural::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void StiffnessStructural::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		Matrix StiffnessStructural::FormMatrix_D(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto material = std::static_pointer_cast<material::IMaterialSolid>(element->GetMaterial());
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return material->D(temperature, pressure);
		}
		Matrix StiffnessStructural::FormMatrix_B(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
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
	} // namespace problems
} // namespace dive