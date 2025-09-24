#include "dive_weakforms_structural_load_distributed_volume.hpp"
#include "dive_loads_distributed_volume.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedVolumeStructuralPtr CreateWeakFormLoadDistributedVolumeStructural()
		{
			auto res = LoadDistributedVolumeStructural::Create();

			return res;
		}
		LoadDistributedVolumeStructuralPtr LoadDistributedVolumeStructural::Create()
		{
			class MakeSharedEnabler : public LoadDistributedVolumeStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedVolumeStructuralPtr LoadDistributedVolumeStructural::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedVolumeStructural>(shared_from_this());
		}
		ConstLoadDistributedVolumeStructuralPtr LoadDistributedVolumeStructural::GetPtr() const
		{
			return const_cast<LoadDistributedVolumeStructural*>(this)->GetPtr();
		}
		void LoadDistributedVolumeStructural::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedVolume>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto f = force->GetValue(point);

			output = N.Transpose() * f;
		}
		Matrix LoadDistributedVolumeStructural::FormMatrix_N(IElementMappedPtr element, const Vector& point) const
		{
			return element->N(point);
		}
	} // namespace problems
} // namespace dive