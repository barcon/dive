#include "dive_weakforms_fluid_convection_asymmetric.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		ConvectionAsymmetricFluidPtr CreateWeakFormConvectionAsymmetricFluid()
		{
			auto res = ConvectionAsymmetricFluid::Create();

			return res;
		}
		ConvectionAsymmetricFluidPtr ConvectionAsymmetricFluid::Create()
		{
			class MakeSharedEnabler : public ConvectionAsymmetricFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ConvectionAsymmetricFluidPtr ConvectionAsymmetricFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<ConvectionAsymmetricFluid>(shared_from_this());
		}
		ConstConvectionAsymmetricFluidPtr ConvectionAsymmetricFluid::GetPtr() const
		{
			return const_cast<ConvectionAsymmetricFluid*>(this)->GetPtr();
		}
		void ConvectionAsymmetricFluid::WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto div = FormDivergence(element, local, cacheIndex);

			output = N.Transpose() * N * div;
		}
		Matrix ConvectionAsymmetricFluid::FormDivergence(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();

			auto dN = eilig::Inverse(element->J(local, cacheIndex)) * element->dN(local);

			Matrix res(numberDof, numberNodes * numberDof, 0.0);

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
		Matrix ConvectionAsymmetricFluid::FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const
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