#include "dive_loads_distributed_face.hpp"
#include "dive_values_vector_congruent.hpp"
#include "dive_status.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedFacePtr CreateLoadDistributedFace(IElementPtr element, FaceIndex faceIndex, IVector3DPtr value)
		{
			if (!element->IsMapped())
			{
				logger::Error(headerDive, "Element is not mapped: " + dive::messages.at(dive::DIVE_INVALID_TYPE));
				return nullptr;
			}

			auto res = LoadDistributedFace::Create();

			res->SetElement(std::dynamic_pointer_cast<elements::IElementMapped>(element));
			res->SetFaceIndex(faceIndex);
			res->SetValue(value);

			return res;
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
			return values::GetValueVector3D(value_, point, element_);
		}
		FaceIndex LoadDistributedFace::GetFaceIndex() const
		{
			return faceIndex_;
		}
		void LoadDistributedFace::SetElement(IElementMappedPtr element)
		{
			element_ = element;
		}
		void LoadDistributedFace::SetFaceIndex(FaceIndex faceIndex)
		{
			faceIndex_ = faceIndex;
		}
		void LoadDistributedFace::SetValue(IVector3DPtr value)
		{
			value_ = value;
		}
	} //namespace loads
} //namespace dive