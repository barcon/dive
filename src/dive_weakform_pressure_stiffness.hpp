#ifndef DIVE_WEAKFORM_PRESSURE_STIFFNESS_HPP_
#define DIVE_WEAKFORM_PRESSURE_STIFFNESS_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		StiffnessPressurePtr CreateWeakFormStiffnessPressure();

		class StiffnessPressure : public IWeakFormElement, virtual public std::enable_shared_from_this<StiffnessPressure>
		{
		public:
			virtual ~StiffnessPressure() = default;

			static StiffnessPressurePtr Create();
			StiffnessPressurePtr GetPtr();
			ConstStiffnessPressurePtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

		protected:
			StiffnessPressure() = default;

			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			using std::enable_shared_from_this<StiffnessPressure>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_PRESSURE_STIFFNESS_HPP_ */