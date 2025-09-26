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
		void ConvectionFluid::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto NN = FormMatrix_NN(element, local, cacheIndex);
			auto udN = FormMatrix_udN(element, local, cacheIndex);
			auto u = FormVelocity(element, local, cacheIndex);
			auto du = FormDivergence(element, local, cacheIndex);

			output =  du * NN + N.Transpose() * udN;
		}
		Matrix ConvectionFluid::FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->u(local);
		}
		Scalar ConvectionFluid::FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			const auto& du = element->InvJ(cacheIndex) * element->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix ConvectionFluid::FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->N(cacheIndex);
		}
		Matrix ConvectionFluid::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
		Matrix ConvectionFluid::FormMatrix_udN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto numberDimensions = element->GetNumberDimensions();
			auto u = FormVelocity(element, local, cacheIndex);
			auto dN = element->InvJ(cacheIndex) * element->dN(cacheIndex);

			Matrix res(numberDof, numberNodes * numberDof, eilig::matrix_zeros);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					for (DimensionIndex k = 0; k < numberDimensions; ++k)
					{
						res(i, j * numberDof + i) += u(k) * dN(k, j);
					}
				}
			}

			return res;
		}

	} // namespace problems
} // namespace dive