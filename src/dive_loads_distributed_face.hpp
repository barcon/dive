#ifndef DIVE_LOADS_DISTRIBUTED_FACE_HPP_
#define DIVE_LOADS_DISTRIBUTED_FACE_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedFacePtr CreateLoadDistributedFace(IElementPtr element, FaceIndex faceIndex, IVector3DPtr value);

		class LoadDistributedFace : public ILoadDistributedFace, virtual public std::enable_shared_from_this<LoadDistributedFace>
		{
		public:
			virtual ~LoadDistributedFace() = default;

			static LoadDistributedFacePtr Create();
			LoadDistributedFacePtr GetPtr();
			ConstLoadDistributedFacePtr GetPtr() const;

			Type GetType() const override;
			IElementMappedPtr GetElement() const override;
			Vector GetValue(const Vector& point) const;
			FaceIndex GetFaceIndex() const;

			void SetElement(IElementMappedPtr element) override;
			void SetFaceIndex(FaceIndex faceIndex);
			void SetValue(IVector3DPtr value);

		protected:
			LoadDistributedFace() = default;

			const Type type_{ load_distributedFace };

			FaceIndex faceIndex_{ 0 };
			IVector3DPtr value_{ nullptr };
			IElementMappedPtr element_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_DISTRIBUTED_AREA_HPP_ */