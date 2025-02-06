#include "dive_weakforms_pressure_crossed_Udp.hpp"

namespace dive {
	namespace weakforms {
		CrossedPressure_Udp_Ptr CreateWeakFormCrossedPressure_Udp()
		{
			auto res = CrossedPressure_Udp::Create();

			return res;
		}
		CrossedPressure_Udp_Ptr CrossedPressure_Udp::Create()
		{
			class MakeSharedEnabler : public CrossedPressure_Udp
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		CrossedPressure_Udp_Ptr CrossedPressure_Udp::GetPtr()
		{
			return std::dynamic_pointer_cast<CrossedPressure_Udp>(shared_from_this());
		}
		ConstCrossedPressure_Udp_Ptr CrossedPressure_Udp::GetPtr() const
		{
			return const_cast<CrossedPressure_Udp*>(this)->GetPtr();
		}
		void CrossedPressure_Udp::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local);
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			
			output = dN.Transpose() * N;
		}
		void CrossedPressure_Udp::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix CrossedPressure_Udp::FormMatrix_N(IElementPtr element, const Vector& local) const
		{
			const auto& elementMomentum = problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()];

			auto numberNodes = elementMomentum->GetNumberNodes();
			auto numberDof = elementMomentum->GetNumberDof();
			auto N = elementMomentum->N(local);

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
		Matrix CrossedPressure_Udp::FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->InvJ(local, cacheIndex) * element->dN(local, cacheIndex);
		}
	} // namespace problems
} // namespace dive