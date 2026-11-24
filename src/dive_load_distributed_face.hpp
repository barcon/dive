#ifndef DIVE_LOAD_DISTRIBUTED_FACE_HPP_
#define DIVE_LOAD_DISTRIBUTED_FACE_HPP_

#include "dive_load.hpp"

namespace dive
{
	namespace load
	{
		LoadDistributedFacePtr CreateLoadDistributedFace(IElementPtr element, FaceIndex faceIndex, IVectorCoordinatesPtr value, IWeakFormLoadPtr weakForm);

		class LoadDistributedFace : public ILoadDistributedFace, virtual public std::enable_shared_from_this<LoadDistributedFace>
		{
		public:
			virtual ~LoadDistributedFace() = default;

			static LoadDistributedFacePtr Create();
			LoadDistributedFacePtr GetPtr();

			Type GetType() const override;
			IElementMappedPtr GetElement() const override;
			Vector GetValue(const Vector& point) const;
			FaceIndex GetFaceIndex() const;
			IWeakFormLoadPtr GetWeakForm() const override;

			void SetElement(IElementMappedPtr element) override;
			void SetFaceIndex(FaceIndex faceIndex);
			void SetValue(IVectorCoordinatesPtr value);
			void SetWeakForm(IWeakFormLoadPtr weakForm) override;

		protected:
			LoadDistributedFace() = default;

			const Type type_{ load_distributed_face };

			FaceIndex faceIndex_{ 0 };
			IElementMappedPtr element_{ nullptr };
			IVectorCoordinatesPtr value_{ nullptr };
			IWeakFormLoadPtr weakForm_{ nullptr };
		};

	} //namespace load
} //namespace dive

#endif /* DIVE_LOAD_DISTRIBUTED_AREA_HPP_ */