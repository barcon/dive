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
		void ConvectionAsymmetricFluid::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			auto div = FormMatrix_Div(element, local, cacheIndex);

			//output = N.Transpose() * (du * N + udN);
			output = N.Transpose() * dN;
		}
		Scalar ConvectionAsymmetricFluid::FormDivergence(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto du = eilig::Inverse(element->J(local, cacheIndex)) * element->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix ConvectionAsymmetricFluid::FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
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
		Matrix ConvectionAsymmetricFluid::FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto dimension = element->GetDimension();
			auto dN = element->InvJ(local, cacheIndex) * element->dN(local, cacheIndex);

			Matrix res(numberDof, numberNodes * numberDof, 0.0);

			for (DofIndex m = 0; m < numberDof; ++m)
			{
				for (NodeIndex n = 0; n < numberNodes; ++n)
				{
					for (Dimension k = 0; k < dimension; ++k)
					{
						res(k, n * numberDof + m) += u(k) * dN(k, n);
					}
				}
			}

			return res;
		}
		Matrix ConvectionAsymmetricFluid::FormMatrix_Div(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto dN = eilig::Inverse(element->J(local)) * element->dN(local);

			Matrix res(1, numberNodes * numberDof, 0.0);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					res(0, j * numberDof + i) = dN(i, j);
				}
			}

			return res;
		}

	} // namespace problems
} // namespace dive