#include "dive_weakforms_fluid_load_distributed_volume_stabilization.hpp"
#include "dive_loads_distributed_volume.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedVolumeStabilizationFluidPtr CreateWeakFormLoadDistributedVolumeStabilizationFluid()
		{
			auto res = LoadDistributedVolumeStabilizationFluid::Create();

			return res;
		}
		LoadDistributedVolumeStabilizationFluidPtr LoadDistributedVolumeStabilizationFluid::Create()
		{
			class MakeSharedEnabler : public LoadDistributedVolumeStabilizationFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedVolumeStabilizationFluidPtr LoadDistributedVolumeStabilizationFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedVolumeStabilizationFluid>(shared_from_this());
		}
		ConstLoadDistributedVolumeStabilizationFluidPtr LoadDistributedVolumeStabilizationFluid::GetPtr() const
		{
			return const_cast<LoadDistributedVolumeStabilizationFluid*>(this)->GetPtr();
		}
		void LoadDistributedVolumeStabilizationFluid::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedVolume>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto udN = FormMatrix_udN(element, point);
			auto du = FormDivergence(element, point);
			
			auto dNu = du * N + udN;
			
			auto f = force->GetValue(point);

			output = -(1.0 / 2.0) * dNu.Transpose() * f;
		}
		Matrix LoadDistributedVolumeStabilizationFluid::FormMatrix_N(IElementPtr element, const Vector& point) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto N = element->N(point);

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
		Matrix LoadDistributedVolumeStabilizationFluid::FormMatrix_udN(IElementPtr element, const Vector& point) const
		{
			auto u = FormMomentum(element, point);
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto dimension = element->GetDimension();
			auto dN = eilig::Inverse(element->J(point)) * element->dN(point);

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
		Matrix LoadDistributedVolumeStabilizationFluid::FormMomentum(IElementPtr element, const Vector& point) const
		{
			return element->u(point);
		}
		Scalar LoadDistributedVolumeStabilizationFluid::FormDivergence(IElementPtr element, const Vector& point) const
		{
			auto du = eilig::Inverse(element->J(point)) * element->du(point);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
	} // namespace problems
} // namespace dive