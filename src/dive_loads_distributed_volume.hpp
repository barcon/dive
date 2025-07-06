#ifndef DIVE_LOADS_DISTRIBUTED_VOLUME_HPP_
#define DIVE_LOADS_DISTRIBUTED_VOLUME_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		LoadDistributedVolumePtr CreateLoadDistributedVolume(IElementPtr element, IVector3DPtr value);
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
			Vector GetValue(const Vector& point) const;

			void SetElement(IElementMappedPtr element) override;
			void SetValue(IVector3DPtr value);

		protected:
			LoadDistributedVolume() = default;

			const Type type_{ load_distributed_volume };

			IVector3DPtr value_{ nullptr };
			IElementMappedPtr element_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_DISTRIBUTED_VOLUME_HPP_ */