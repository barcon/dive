#include "dive_weakforms_fluid_load_distributed_volume_stabilization.hpp"
#include "dive_load_distributed_volume.hpp"
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
			auto force = std::static_pointer_cast<load::LoadDistributedVolume>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto udN = FormMatrix_udN(element, point);
			auto du = FormDivergence(element, point);
			
			auto dNu = du * N + udN;
			
			auto f = force->GetValue(point);

			output = -(1.0 / 2.0) * dNu.Transpose() * f;
		}
		Matrix LoadDistributedVolumeStabilizationFluid::FormMatrix_N(IElementMappedPtr element, const Vector& point) const
		{
			return element->N(point);
		}
		Matrix LoadDistributedVolumeStabilizationFluid::FormMatrix_udN(IElementMappedPtr element, const Vector& point) const
		{
			auto u = FormVelocity(element, point);
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto numberDimensions = element->GetNumberDimensions();
			auto dN = eilig::Inverse(element->J(point)) * element->dN(point);

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
		Matrix LoadDistributedVolumeStabilizationFluid::FormVelocity(IElementMappedPtr element, const Vector& point) const
		{
			return element->u(point);
		}
		Scalar LoadDistributedVolumeStabilizationFluid::FormDivergence(IElementMappedPtr element, const Vector& point) const
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