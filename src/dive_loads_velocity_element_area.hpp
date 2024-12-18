#ifndef DIVE_LOADS_VELOCITY_ELEMENT_AREA_HPP_
#define DIVE_LOADS_VELOCITY_ELEMENT_AREA_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		VelocityElementAreaPtr CreateVelocityElementArea();

		class VelocityElementArea : public IBoundary, virtual public std::enable_shared_from_this<VelocityElementArea>
		{
		public:
			virtual ~VelocityElementArea() = default;

			static VelocityElementAreaPtr Create();
			VelocityElementAreaPtr GetPtr();
			ConstVelocityElementAreaPtr GetPtr() const;

			Type GetType() const override;
			IElementPtr GetElement() const override;
			FaceIndex GetFaceIndex() const override;
			Matrix GetVelocity() const override;

			void SetElement(IElementPtr element) override;
			void SetFaceIndex(FaceIndex faceIndex) override;
			void SetVelocity(const Matrix& velocity) const override;

		protected:
			VelocityElementArea() = default;

			const Type type_{ load_velocityElementArea };

			FaceIndex faceIndex_{ 0 };
			IElementPtr element_{ nullptr };
			Matrix velocity_;
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_VELOCITY_ELEMENT_AREA_HPP_ */