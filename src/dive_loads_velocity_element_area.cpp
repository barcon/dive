#include "dive_loads_velocity_element_area.hpp"

namespace dive
{
	namespace loads
	{
		VelocityElementAreaPtr CreateVelocityElementArea()
		{
			auto res = VelocityElementArea::Create();

			return res;
		}
		VelocityElementAreaPtr VelocityElementArea::Create()
		{
			class MakeSharedEnabler : public VelocityElementArea
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		VelocityElementAreaPtr VelocityElementArea::GetPtr()
		{
			return std::dynamic_pointer_cast<VelocityElementArea>(shared_from_this());
		}
		ConstVelocityElementAreaPtr VelocityElementArea::GetPtr() const
		{
			return const_cast<VelocityElementArea*>(this)->GetPtr();
		}
		Matrix VelocityElementArea::GetVelocity() const
		{
			return velocity_;
		}
		Type VelocityElementArea::GetType() const
		{
			return type_;
		}
		IElementPtr VelocityElementArea::GetElement() const
		{
			return element_;
		}
		FaceIndex VelocityElementArea::GetFaceIndex() const
		{
			return faceIndex_;
		}
		void VelocityElementArea::SetElement(IElementPtr element)
		{
			element_ = element;
		}
		void VelocityElementArea::SetFaceIndex(FaceIndex faceIndex)
		{
			faceIndex_ = faceIndex;
		}
		void VelocityElementArea::SetVelocity(const Matrix& velocity) const
		{
			velocity_ = velocity;
		}
	} //namespace loads
} //namespace dive