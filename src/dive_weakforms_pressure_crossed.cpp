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
		void CrossedPressure::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto N = FormMatrix_N(element, local, cacheIndex);
			auto dN = FormMatrix_dN(element, local, cacheIndex);
			
			output = dN.Transpose() * N;
		}
		void CrossedPressure::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix CrossedPressure::FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			const auto& elementMomentum = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()]);

			return elementMomentum->N(local);
		}
		Matrix CrossedPressure::FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->InvJ(cacheIndex) * element->dN(cacheIndex);
		}
	} // namespace problems
} // namespace dive