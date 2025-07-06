#include "dive_weakforms_pressure_distributed_volume_divergence.hpp"

namespace dive {
	namespace weakforms {
		DistributedVolumeDivergencePressurePtr CreateWeakFormDistributedVolumeDivergencePressure()
		{
			auto res = DistributedVolumeDivergencePressure::Create();

			return res;
		}
		DistributedVolumeDivergencePressurePtr DistributedVolumeDivergencePressure::Create()
		{
			class MakeSharedEnabler : public DistributedVolumeDivergencePressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		DistributedVolumeDivergencePressurePtr DistributedVolumeDivergencePressure::GetPtr()
		{
			return std::dynamic_pointer_cast<DistributedVolumeDivergencePressure>(shared_from_this());
		}
		ConstDistributedVolumeDivergencePressurePtr DistributedVolumeDivergencePressure::GetPtr() const
		{
			return const_cast<DistributedVolumeDivergencePressure*>(this)->GetPtr();
		}
		void DistributedVolumeDivergencePressure::WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto div = FormMatrix_Div(element, local);
			
			output = N.Transpose() * div;
		}
		void DistributedVolumeDivergencePressure::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix DistributedVolumeDivergencePressure::FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->N(local, cacheIndex);
		}
		Matrix DistributedVolumeDivergencePressure::FormMatrix_Div(IElementMappedPtr element, const Vector& local) const
		{
			const auto& elementMomentum = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()]);

			auto numberNodes = elementMomentum->GetNumberNodes();
			auto numberDof = elementMomentum->GetNumberDof();
			auto dN = eilig::Inverse(elementMomentum->J(local)) * elementMomentum->dN(local);

			Matrix res(1, numberNodes * numberDof, eilig::matrix_zeros);

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