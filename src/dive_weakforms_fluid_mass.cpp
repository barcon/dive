#include "dive_weakforms_fluid_mass.hpp"

namespace dive {
	namespace weakforms {
		MassFluidPtr CreateWeakFormMassFluid()
		{
			auto res = MassFluid::Create();

			return res;
		}
		MassFluidPtr MassFluid::Create()
		{
			class MakeSharedEnabler : public MassFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassFluidPtr MassFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<MassFluid>(shared_from_this());
		}
		ConstMassFluidPtr MassFluid::GetPtr() const
		{
			return const_cast<MassFluid*>(this)->GetPtr();
		}
		void MassFluid::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& point, Matrix& output) const
		{
			auto N = FormMatrix_N(element, point, cacheIndex);

			output = N.Transpose() * N;
		}
		Matrix MassFluid::FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
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
	} // namespace problems
} // namespace dive