#include "dive_weakform_pressure_load_distributed_face_flow.hpp"
#include "dive_load_distributed_face.hpp"

namespace dive {
	namespace weakform {
		LoadDistributedFaceFlowPressurePtr CreateWeakFormLoadDistributedFaceFlowPressure()
		{
			auto res = LoadDistributedFaceFlowPressure::Create();

			return res;
		}
		LoadDistributedFaceFlowPressurePtr LoadDistributedFaceFlowPressure::Create()
		{
			class MakeSharedEnabler : public LoadDistributedFaceFlowPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedFaceFlowPressurePtr LoadDistributedFaceFlowPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedFaceFlowPressure>(shared_from_this());
		}
		void LoadDistributedFaceFlowPressure::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<load::LoadDistributedFace>(load);
			auto element = force->GetElement();
			auto faceIndex = force->GetFaceIndex();

			auto N = FormMatrix_N(element, point);
			auto n = std::dynamic_pointer_cast<element::IElementSolid>(element)->Normal(faceIndex, point);
			auto f = force->GetValue(point);
			
			std::cout << eilig::List(n) << std::endl;

			//output = N.Transpose() * eilig::Dot(n, f);
		}
		void LoadDistributedFaceFlowPressure::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix LoadDistributedFaceFlowPressure::FormMatrix_N(IElementMappedPtr element, const Vector& point) const
		{
			return element->N(point);
		}
	} // namespace weakform
} // namespace dive