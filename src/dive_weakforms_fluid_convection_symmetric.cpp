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
		void ConvectionSymmetricFluid::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto du = FormDivergence(element, local, cacheIndex);

			output = N.Transpose() * N * du;
		}
		Matrix ConvectionSymmetricFluid::FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();

			auto dN = element->InvJ(cacheIndex) * element->dN(local);

			Matrix res(numberDof, numberNodes * numberDof, eilig::matrix_zeros);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					//res(i, j * numberDof + i) = N(j);
				}
			}

			return res;
		}
		Matrix ConvectionSymmetricFluid::FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			const auto& N = element->N(cacheIndex);

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