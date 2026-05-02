#include "dive_load_distributed_volume.hpp"
#include "dive_value_vector_congruent.hpp"
#include "dive_status.hpp"

namespace dive
{
	namespace load
	{
		LoadDistributedVolumePtr CreateLoadDistributedVolume(IElementPtr element, IVectorCoordinatesPtr value)
		{
			auto res = LoadDistributedVolume::Create();

			res->SetElement(std::dynamic_pointer_cast<element::IElementMapped>(element));
			res->SetValue(value);

			return res;
		}
		LoadDistributedVolumePtr CastToLoadDistributedVolume(ILoadPtr load)
		{
			return std::dynamic_pointer_cast<LoadDistributedVolume>(load);
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
			return value::GetValueVectorCoordinates(value_, point, element_);
		}
		void LoadDistributedVolume::SetElement(IElementMappedPtr element)
		{
			if (element == nullptr)
			{
				throw std::invalid_argument("Element is nullptr");
			}

			if (!element->IsMapped())
			{
				throw std::invalid_argument("Element is not mapped");
			}

			element_ = element;
		}
		void LoadDistributedVolume::SetValue(IVectorCoordinatesPtr value)
		{
			if (value == nullptr)
			{
				throw std::invalid_argument("Value is nullptr");
			}

			if (value->GetNumberCoordinates() != element_->GetNumberCoordinates())
			{
				throw std::invalid_argument("Number coordinates of value does not match number coordinates of element");
			}

			value_ = value;
		}
	} //namespace load
} //namespace dive