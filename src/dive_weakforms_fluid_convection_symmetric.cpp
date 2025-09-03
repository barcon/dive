#include "dive_weakforms_fluid_convection_symmetric.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		ConvectionSymmetricFluidPtr CreateWeakFormConvectionSymmetricFluid()
		{
			auto res = ConvectionSymmetricFluid::Create();

			return res;
		}
		ConvectionSymmetricFluidPtr ConvectionSymmetricFluid::Create()
		{
			class MakeSharedEnabler : public ConvectionSymmetricFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ConvectionSymmetricFluidPtr ConvectionSymmetricFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<ConvectionSymmetricFluid>(shared_from_this());
		}
		ConstConvectionSymmetricFluidPtr ConvectionSymmetricFluid::GetPtr() const
		{
			return const_cast<ConvectionSymmetricFluid*>(this)->GetPtr();
		}
		void ConvectionSymmetricFluid::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local);
			auto div = FormDivergence(element, local);

			output = N.Transpose() * N * div;
		}
		Matrix ConvectionSymmetricFluid::FormDivergence(IElementMappedPtr element, const Vector& local) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();

			auto dN = eilig::Inverse(element->J(local)) * element->dN(local);

			Matrix res(numberDof, numberNodes * numberDof, eilig::matrix_zeros);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					//res(i, j * numberDof + i) = N(j);
				}
			}

			return res;

			//for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			//{
			//	divergence += du(i, i);
			//}

			return res;
		}
		Matrix ConvectionSymmetricFluid::FormMatrix_N(IElementMappedPtr element, const Vector& local) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			const auto& N = element->N(local);

			Matrix res(numberDof, numberNodes * numberDof, eilig::matrix_zeros);

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