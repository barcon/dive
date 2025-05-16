#include "dive_loads_distributed_volume.hpp"
#include "dive_values_vector_congruent.hpp"
#include "dive_status.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedVolumePtr CreateLoadDistributedVolume(IElementPtr element, IVector3DPtr value)
		{
			if (!element->IsMapped())
			{
				logger::Error(headerDive, "Element is not mapped: " + dive::messages.at(dive::DIVE_INVALID_TYPE));
				return nullptr;
			}

			auto res = LoadDistributedVolume::Create();
			res->SetElement(std::dynamic_pointer_cast<elements::IElementMapped>(element));
			res->SetValue(value);

			return res;
		}
		LoadDistributedVolumePtr LoadDistributedVolume::Create()
		{
			class MakeSharedEnabler : public LoadDistributedVolume
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedVolumePtr LoadDistributedVolume::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedVolume>(shared_from_this());;
		}
		ConstLoadDistributedVolumePtr LoadDistributedVolume::GetPtr() const
		{
			return const_cast<LoadDistributedVolume*>(this)->GetPtr();
		}
		Type LoadDistributedVolume::GetType() const
		{
			return type_;
		}
		IElementMappedPtr LoadDistributedVolume::GetElement() const
		{
			return element_;
		}
		Vector LoadDistributedVolume::GetValue(const Vector& point) const
		{
			return values::GetValueVector3D(value_, point, element_);
		}
		void LoadDistributedVolume::SetElement(IElementMappedPtr element)
		{
			element_ = element;
		}
		void LoadDistributedVolume::SetValue(IVector3DPtr value)
		{
			value_ = value;
		}
	} //namespace loads
} //namespace dive