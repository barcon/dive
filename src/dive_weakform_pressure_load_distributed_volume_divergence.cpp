#include "dive_weakform_pressure_load_distributed_volume_divergence.hpp"
#include "dive_load_distributed_volume.hpp"
#include "dive_value_scalar_congruent.hpp"

namespace dive {
	namespace weakform {
		LoadDistributedVolumeDivergencePressurePtr CreateLoadDistributedVolumeDivergencePressure()
		{
			auto res = LoadDistributedVolumeDivergencePressure::Create();

			return res;
		}
		LoadDistributedVolumeDivergencePressurePtr LoadDistributedVolumeDivergencePressure::Create()
		{
			class MakeSharedEnabler : public LoadDistributedVolumeDivergencePressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedVolumeDivergencePressurePtr LoadDistributedVolumeDivergencePressure::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedVolumeDivergencePressure>(shared_from_this());
		}
		void LoadDistributedVolumeDivergencePressure::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<load::LoadDistributedVolume>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto div = FormDivergence(element, point);
			
			output = Vector(N.Transpose() * div, 0);
		}
		void LoadDistributedVolumeDivergencePressure::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix LoadDistributedVolumeDivergencePressure::FormMatrix_N(IElementMappedPtr element, const Vector& local) const
		{
			return element->N(local);
		}
		Scalar LoadDistributedVolumeDivergencePressure::FormDivergence(IElementMappedPtr element, const Vector& local) const
		{
			Scalar divergence{ 0.0 };
			const auto& elementMomentum = std::dynamic_pointer_cast<element::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()]);
			auto du = elementMomentum->InvJ(local) * elementMomentum->du(local);

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
	} // namespace weakform
} // namespace dive