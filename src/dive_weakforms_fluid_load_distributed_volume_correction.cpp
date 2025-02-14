#include "dive_weakforms_fluid_load_distributed_volume_correction.hpp"
#include "dive_loads_distributed_volume.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedVolumeCorrectionFluidPtr CreateWeakFormLoadDistributedVolumeCorrectionFluid()
		{
			auto res = LoadDistributedVolumeCorrectionFluid::Create();

			return res;
		}
		LoadDistributedVolumeCorrectionFluidPtr LoadDistributedVolumeCorrectionFluid::Create()
		{
			class MakeSharedEnabler : public LoadDistributedVolumeCorrectionFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedVolumeCorrectionFluidPtr LoadDistributedVolumeCorrectionFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedVolumeCorrectionFluid>(shared_from_this());
		}
		ConstLoadDistributedVolumeCorrectionFluidPtr LoadDistributedVolumeCorrectionFluid::GetPtr() const
		{
			return const_cast<LoadDistributedVolumeCorrectionFluid*>(this)->GetPtr();
		}
		void LoadDistributedVolumeCorrectionFluid::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedVolume>(load);
			auto element = force->GetElement();

			auto dN = FormVector_dN(element, point);
			auto p = FormPressure(element, point);

			output = dN * p;
		}
		void LoadDistributedVolumeCorrectionFluid::SetProblemPressure(IProblemPtr problemPressure)
		{
			problemPressure_ = problemPressure;
		}
		Vector LoadDistributedVolumeCorrectionFluid::FormVector_dN(IElementPtr element, const Vector& point) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto dimension = element->GetDimension();
			auto dN = eilig::Inverse(element->J(point)) * element->dN(point);

			Vector res(numberNodes * numberDof, 0.0);

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
		Scalar LoadDistributedVolumeCorrectionFluid::FormPressure(IElementPtr element, const Vector& point) const
		{
			const auto& elementPressure = problemPressure_->GetMesh()->GetElements()[element->GetElementIndex()];
			return elementPressure->u(point)(0);
		}
	} // namespace problems
} // namespace dive