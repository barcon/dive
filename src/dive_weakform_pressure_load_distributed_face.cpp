#include "dive_weakform_pressure_load_distributed_face.hpp"
#include "dive_load_distributed_face.hpp"

namespace dive {
	namespace weakform {
		LoadDistributedFacePressurePtr CreateWeakFormLoadDistributedFacePressure()
		{
			auto res = LoadDistributedFacePressure::Create();

			return res;
		}
		LoadDistributedFacePressurePtr LoadDistributedFacePressure::Create()
		{
			class MakeSharedEnabler : public LoadDistributedFacePressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedFacePressurePtr LoadDistributedFacePressure::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedFacePressure>(shared_from_this());
		}
		void LoadDistributedFacePressure::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<load::LoadDistributedFace>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto f = force->GetValue(point);

			output = N.Transpose() * f;
		}
		Matrix LoadDistributedFacePressure::FormMatrix_N(IElementMappedPtr element, const Vector& point) const
		{
			return element->N(point);
		}
	} // namespace weakform
} // namespace dive