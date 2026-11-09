#ifndef DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_FACE_HPP_
#define DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_FACE_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		LoadDistributedFacePressurePtr CreateWeakFormLoadDistributedFacePressure();

		class LoadDistributedFacePressure : public IWeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedFacePressure>
		{
		public:
			virtual ~LoadDistributedFacePressure() = default;

			static LoadDistributedFacePressurePtr Create();
			LoadDistributedFacePressurePtr GetPtr();

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

		protected:
			LoadDistributedFacePressure() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& point) const;

			using std::enable_shared_from_this<LoadDistributedFacePressure>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_PRESSURE_LOAD_DISTRIBUTED_FACE_HPP_ */