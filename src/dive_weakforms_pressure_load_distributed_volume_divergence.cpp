#include "dive_weakforms_pressure_load_distributed_volume_divergence.hpp"
#include "dive_loads_distributed_volume.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedVolumeDivergencePressurePtr CreateWeakFormLoadDistributedVolumeDivergencePressure()
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
		ConstLoadDistributedVolumeDivergencePressurePtr LoadDistributedVolumeDivergencePressure::GetPtr() const
		{
			return const_cast<LoadDistributedVolumeDivergencePressure*>(this)->GetPtr();
		}
		void LoadDistributedVolumeDivergencePressure:: WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedVolume>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto Nt = Vector(N.Transpose(), 0);
			auto du = FormDivergence(element, point);
			
			output = Nt * du;
		}
		void LoadDistributedVolumeDivergencePressure::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix LoadDistributedVolumeDivergencePressure::FormMatrix_N(IElementPtr element, const Vector& local) const
		{
			return element->N(local);
		}
		Scalar LoadDistributedVolumeDivergencePressure::FormDivergence(IElementPtr element, const Vector& local) const
		{
			const auto& elementMomentum = problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()];
			auto du = eilig::Inverse(elementMomentum->J(local)) * elementMomentum->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
	} // namespace problems
} // namespace dive