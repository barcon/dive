#include "dive_load_distributed_face.hpp"
#include "dive_value_vector_congruent.hpp"
#include "dive_status.hpp"

namespace dive
{
	namespace load
	{
		LoadDistributedFacePtr CreateLoadDistributedFace(IElementPtr element, FaceIndex faceIndex, IVectorCoordinatesPtr value)
		{
			auto res = LoadDistributedFace::Create();

			res->SetElement(std::dynamic_pointer_cast<element::IElementMapped>(element));
			res->SetFaceIndex(faceIndex);
			res->SetValue(value);

			return res;
		}
		LoadDistributedFacePtr CastToLoadDistributedFace(ILoadPtr load)
		{
			return std::dynamic_pointer_cast<LoadDistributedFace>(load);
		}
		LoadDistributedFacePtr LoadDistributedFace::Create()
		{
			class MakeSharedEnabler : public LoadDistributedFace
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedFacePtr LoadDistributedFace::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedFace>(shared_from_this());
		}
		ConstLoadDistributedFacePtr LoadDistributedFace::GetPtr() const
		{
			return const_cast<LoadDistributedFace*>(this)->GetPtr();
		}
		Type LoadDistributedFace::GetType() const
		{
			return type_;
		}
		IElementMappedPtr LoadDistributedFace::GetElement() const
		{
			return element_;
		}
		Vector LoadDistributedFace::GetValue(const Vector& point) const
		{
			return value::GetValueVectorCoordinates(value_, point, element_);
		}
		FaceIndex LoadDistributedFace::GetFaceIndex() const
		{
			return faceIndex_;
		}
		void LoadDistributedFace::SetElement(IElementMappedPtr element)
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
		void LoadDistributedFace::SetFaceIndex(FaceIndex faceIndex)
		{
			faceIndex_ = faceIndex;
		}
		void LoadDistributedFace::SetValue(IVectorCoordinatesPtr value)
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