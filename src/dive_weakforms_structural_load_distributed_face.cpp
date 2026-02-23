#include "dive_weakforms_structural_load_distributed_face.hpp"
#include "dive_load_distributed_face.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedFaceStructuralPtr CreateWeakFormLoadDistributedFaceStructural()
		{
			auto res = LoadDistributedFaceStructural::Create();

			return res;
		}
		LoadDistributedFaceStructuralPtr LoadDistributedFaceStructural::Create()
		{
			class MakeSharedEnabler : public LoadDistributedFaceStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedFaceStructuralPtr LoadDistributedFaceStructural::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedFaceStructural>(shared_from_this());
		}
		ConstLoadDistributedFaceStructuralPtr LoadDistributedFaceStructural::GetPtr() const
		{
			return const_cast<LoadDistributedFaceStructural*>(this)->GetPtr();
		}
		void LoadDistributedFaceStructural::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<load::LoadDistributedFace>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto f = force->GetValue(point);

			output = N.Transpose() * f;
		}
		Matrix LoadDistributedFaceStructural::FormMatrix_N(IElementMappedPtr element, const Vector& point) const
		{
			return element->N(point);
		}
	} // namespace problems
} // namespace dive