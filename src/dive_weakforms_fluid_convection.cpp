#include "dive_weakforms_fluid_convection.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		ConvectionFluidPtr CreateWeakFormConvectionFluid()
		{
			auto res = ConvectionFluid::Create();

			return res;
		}
		ConvectionFluidPtr ConvectionFluid::Create()
		{
			class MakeSharedEnabler : public ConvectionFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ConvectionFluidPtr ConvectionFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<ConvectionFluid>(shared_from_this());
		}
		ConstConvectionFluidPtr ConvectionFluid::GetPtr() const
		{
			return const_cast<ConvectionFluid*>(this)->GetPtr();
		}
		void ConvectionFluid::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto udN = FormMatrix_udN(element, local, cacheIndex);
			auto u = FormMomentum(element, local);
			auto div = FormMatrix_Div(element, local, cacheIndex);

			//output = N.Transpose() * (du * N + udN);
			output = N.Transpose() * N * div;
		}
		Matrix ConvectionFluid::FormMomentum(IElementPtr element, const Vector& local) const
		{
			return element->u(local);
		}
		Scalar ConvectionFluid::FormDivergence(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto du = eilig::Inverse(element->J(local, cacheIndex)) * element->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix ConvectionFluid::FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
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
		Matrix ConvectionFluid::FormMatrix_udN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto dimension = element->GetDimension();
			auto u = FormMomentum(element, local);
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
		Matrix ConvectionFluid::FormMatrix_Div(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
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