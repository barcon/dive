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
		void StabilizationFluid::WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto udN = FormMatrix_udN(element, local, cacheIndex);
			auto du = FormDivergence(element, local, cacheIndex);

			auto dNu = du * N + udN;

			output = -(1.0 / 2.0) * dNu.Transpose() * dNu;
		}
		Matrix StabilizationFluid::FormMomentum(IElementMappedPtr element, const Vector& local) const
		{
			return element->u(local);
		}
		Scalar StabilizationFluid::FormDivergence(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto du = eilig::Inverse(element->J(local, cacheIndex)) * element->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix StabilizationFluid::FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNode(0)->GetNumberDof();
			const auto& N = element->N(local, cacheIndex);

			Matrix res(numberDof, numberNodes * numberDof, 0.0);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					res(i, i + j * numberDof) = N(0, j);
				}
			}

			return res;
		}
		Matrix StabilizationFluid::FormMatrix_udN(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			auto u = FormMomentum(element, local);
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNode(0)->GetNumberDof();
			auto numberDimensions = element->GetNumberDimensions();
			auto dN = element->InvJ(local, cacheIndex) * element->dN(local, cacheIndex);

			Matrix res(numberDof, numberNodes * numberDof, 0.0);

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