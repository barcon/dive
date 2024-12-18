#include "dive_loads_distributed_face.hpp"
#include "dive_values_vector_congruent.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedFacePtr CreateLoadDistributedFace(IElementPtr element, FaceIndex faceIndex, IVector3DPtr value)
		{
			auto res = LoadDistributedFace::Create();

			res->SetElement(element);
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
		IElementPtr LoadDistributedFace::GetElement() const
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
		void LoadDistributedFace::SetElement(IElementPtr element)
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