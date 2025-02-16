#include "dive_weakforms_pressure_crossed.hpp"

namespace dive {
	namespace weakforms {
		CrossedPressurePtr CreateWeakFormCrossedPressure()
		{
			auto res = CrossedPressure::Create();

			return res;
		}
		CrossedPressurePtr CrossedPressure::Create()
		{
			class MakeSharedEnabler : public CrossedPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		CrossedPressurePtr CrossedPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<CrossedPressure>(shared_from_this());
		}
		ConstCrossedPressurePtr CrossedPressure::GetPtr() const
		{
			return const_cast<CrossedPressure*>(this)->GetPtr();
		}
		void CrossedPressure::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local);
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			
			output = dN.Transpose() * N;
		}
		void CrossedPressure::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix CrossedPressure::FormMatrix_N(IElementPtr element, const Vector& local) const
		{
			const auto& elementVelocity = problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()];

			auto numberNodes = elementVelocity->GetNumberNodes();
			auto numberDof = elementVelocity->GetNumberDof();
			auto N = elementVelocity->N(local);

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
		Matrix CrossedPressure::FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->InvJ(local, cacheIndex) * element->dN(local, cacheIndex);
		}
	} // namespace problems
} // namespace dive