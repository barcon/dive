#ifndef DIVE_LOAD_DISTRIBUTED_VOLUME_HPP_
#define DIVE_LOAD_DISTRIBUTED_VOLUME_HPP_

#include "dive_load.hpp"

namespace dive
{
	namespace load
	{
		LoadDistributedVolumePtr CreateLoadDistributedVolume(IElementPtr element, IVectorCoordinatesPtr value);
		LoadDistributedVolumePtr CastToLoadDistributedVolume(ILoadPtr load);

		class LoadDistributedVolume : public ILoadDistributedVolume, virtual public std::enable_shared_from_this<LoadDistributedVolume>
		{
		public:
			virtual ~LoadDistributedVolume() = default;

			static LoadDistributedVolumePtr Create();
			LoadDistributedVolumePtr GetPtr();
			ConstLoadDistributedVolumePtr GetPtr() const;

			Type GetType() const override;
			IElementMappedPtr GetElement() const override;
			Vector GetValue(const Vector& point) const override;

			void SetElement(IElementMappedPtr element) override;
			void SetValue(IVectorCoordinatesPtr value) override;

		protected:
			LoadDistributedVolume() = default;

			const Type type_{ load_distributed_volume };

			IVectorCoordinatesPtr value_{ nullptr };
			IElementMappedPtr element_{ nullptr };
		};

	} //namespace load
} //namespace dive

#endif /* DIVE_LOAD_DISTRIBUTED_VOLUME_HPP_ */