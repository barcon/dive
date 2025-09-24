#include "dive_weakforms_fluid_load_distributed_volume.hpp"
#include "dive_loads_distributed_volume.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedVolumeFluidPtr CreateWeakFormLoadDistributedVolumeFluid()
		{
			auto res = LoadDistributedVolumeFluid::Create();

			return res;
		}
		LoadDistributedVolumeFluidPtr LoadDistributedVolumeFluid::Create()
		{
			class MakeSharedEnabler : public LoadDistributedVolumeFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedVolumeFluidPtr LoadDistributedVolumeFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedVolumeFluid>(shared_from_this());
		}
		ConstLoadDistributedVolumeFluidPtr LoadDistributedVolumeFluid::GetPtr() const
		{
			return const_cast<LoadDistributedVolumeFluid*>(this)->GetPtr();
		}
		void LoadDistributedVolumeFluid::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedVolume>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto f = force->GetValue(point);

			output = N.Transpose() * f;
		}
		Matrix LoadDistributedVolumeFluid::FormMatrix_N(IElementMappedPtr element, const Vector& point) const
		{
			return element->N(point);
		}
	} // namespace problems
} // namespace dive