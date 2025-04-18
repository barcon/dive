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
		void MassStructural::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& point, Matrix& output) const
		{
			auto N = FormMatrix_N(element, point, cacheIndex);
			auto rho = FormDensity(element, point);

			output = N.Transpose() * rho * N;
		}
		void MassStructural::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassStructural::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		Matrix MassStructural::FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			const auto& N = element->N(local, cacheIndex);

			Matrix res(numberDof, numberNodes * numberDof, 0.0);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					res(i, j * numberDof + i) = N(j);
				}
			}

			return res;
		}
		Scalar MassStructural::FormDensity(IElementPtr element, const Vector& local) const
		{
			auto material = std::static_pointer_cast<material::IMaterialSolid>(element->GetMaterial());
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			return material->GetDensity(temperature, pressure);
		}
	} // namespace problems
} // namespace dive