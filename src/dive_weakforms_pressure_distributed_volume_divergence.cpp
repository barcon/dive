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
		void DistributedVolumeDivergencePressure::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto du = FormMatrix_Div(element, local);
			
			output = N.Transpose() * du;
		}
		void DistributedVolumeDivergencePressure::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix DistributedVolumeDivergencePressure::FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->N(local, cacheIndex);
		}
		Matrix DistributedVolumeDivergencePressure::FormMatrix_Div(IElementPtr element, const Vector& local) const
		{
			const auto& elementMomentum = problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()];
			auto numberNodes = elementMomentum->GetNumberNodes();
			auto numberDof = elementMomentum->GetNode(0)->GetNumberDof();
			auto dimension = elementMomentum->GetDimension();
			auto dN = eilig::Inverse(elementMomentum->J(local)) * elementMomentum->dN(local);

			Matrix res(1, numberNodes * numberDof, 0.0);

			for (NodeIndex i = 0; i < numberNodes; ++i)
			{
				for (Dimension j = 0; j < dimension; ++j)
				{
					res(1, i * dimension + j) = dN(j, i);
				}
			}

			return res;
		}
	} // namespace problems
} // namespace dive

/*
		Scalar DistributedVolumeDivergencePressure::FormDivergence(IElementPtr element, const Vector& local) const
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
*/