#ifndef DIVE_WEAKFORMS_PRESSURE_STIFFNESS_HPP_
#define DIVE_WEAKFORMS_PRESSURE_STIFFNESS_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		StiffnessPressurePtr CreateWeakFormStiffnessPressure();

		class StiffnessPressure : public WeakFormElement, virtual public std::enable_shared_from_this<StiffnessPressure>
		{
		public:
			virtual ~StiffnessPressure() = default;

			static StiffnessPressurePtr Create();
			StiffnessPressurePtr GetPtr();
			ConstStiffnessPressurePtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

		protected:
			StiffnessPressure() = default;

			Matrix FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			using std::enable_shared_from_this<StiffnessPressure>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_STIFFNESS_HPP_ */