#include "dive_weakform_deformation_stiffness.hpp"
#include "dive_value_scalar_congruent.hpp"

namespace dive {
	namespace weakform {
		StiffnessDeformationPtr CreateWeakFormStiffnessDeformation()
		{
			auto res = StiffnessDeformation::Create();

			return res;
		}
		StiffnessDeformationPtr StiffnessDeformation::Create()
		{
			class MakeSharedEnabler : public StiffnessDeformation
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		StiffnessDeformationPtr StiffnessDeformation::GetPtr()
		{
			return std::dynamic_pointer_cast<StiffnessDeformation>(shared_from_this());
		}
		ConstStiffnessDeformationPtr StiffnessDeformation::GetPtr() const
		{
			return const_cast<StiffnessDeformation*>(this)->GetPtr();
		}
		void StiffnessDeformation::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			auto K = FormMatrix_K(element, local, cacheIndex);

			output = dN.Transpose() * K * dN;

			//std::cout << eilig::ListMatrix(output) << std::endl;
		}
		void StiffnessDeformation::SetTemperature(IScalarCoordinatesPtr temperature)
		{
			temperature_ = temperature;
		}
		void StiffnessDeformation::SetPressure(IScalarCoordinatesPtr pressure)
		{
			pressure_ = pressure;
		}
		Matrix StiffnessDeformation::FormMatrix_K(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto temperature = value::GetValueScalarCoordinates(temperature_, local, element);
			auto pressure = value::GetValueScalarCoordinates(pressure_, local, element);
			auto state = material::CreateState(temperature, pressure);
			auto thermalConductivity = element->GetMaterial()->GetThermalConductivity(state);

			return thermalConductivity * Matrix(9, 9, eilig::matrix_diagonal);
		}
		Matrix StiffnessDeformation::FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto dN = element->InvJ(cacheIndex) * element->dN(cacheIndex);

			Matrix res(9, numberNodes * numberDof, eilig::matrix_zeros);

			for (NodeIndex i = 0; i < numberNodes; ++i)
			{
				res(0, i * numberDof + 0) = dN(0, i);
				res(1, i * numberDof + 1) = dN(1, i);
				res(2, i * numberDof + 2) = dN(2, i);

				res(3, i * numberDof + 0) = dN(0, i);
				res(4, i * numberDof + 1) = dN(1, i);
				res(5, i * numberDof + 2) = dN(2, i);

				res(6, i * numberDof + 0) = dN(0, i);
				res(7, i * numberDof + 1) = dN(1, i);
				res(8, i * numberDof + 2) = dN(2, i);
			}

			return res;
		}
	} // namespace problems
} // namespace dive