#include "dive_weakforms_fluid_stabilization.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		StabilizationFluidPtr CreateWeakFormStabilizationFluid()
		{
			auto res = StabilizationFluid::Create();

			return res;
		}
		StabilizationFluidPtr StabilizationFluid::Create()
		{
			class MakeSharedEnabler : public StabilizationFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		StabilizationFluidPtr StabilizationFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<StabilizationFluid>(shared_from_this());
		}
		ConstStabilizationFluidPtr StabilizationFluid::GetPtr() const
		{
			return const_cast<StabilizationFluid*>(this)->GetPtr();
		}
		void StabilizationFluid::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto udN = FormMatrix_udN(element, local, cacheIndex);
			auto du = FormDivergence(element, local, cacheIndex);

			auto dNu = du * N + udN;

			output = -(1.0 / 2.0) * dNu.Transpose() * dNu;
		}
		Matrix StabilizationFluid::FormMomentum(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->u(local);
		}
		Scalar StabilizationFluid::FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto du = element->InvJ(cacheIndex) * element->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix StabilizationFluid::FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->N(cacheIndex);
		}
		Matrix StabilizationFluid::FormMatrix_udN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto u = FormMomentum(element, local, cacheIndex);
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNode(0)->GetNumberDof();
			auto numberDimensions = element->GetNumberDimensions();
			auto dN = element->InvJ(cacheIndex) * element->dN(cacheIndex);

			Matrix res(numberDof, numberNodes * numberDof, eilig::matrix_zeros);

			for (DofIndex m = 0; m < numberDof; ++m)
			{
				for (NodeIndex n = 0; n < numberNodes; ++n)
				{
					for (DimensionIndex k = 0; k < numberDimensions; ++k)
					{
						res(k, n * numberDof + m) += u(k) * dN(k, n);
					}
				}
			}

			return res;
		}
	} // namespace problems
} // namespace dive