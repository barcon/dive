#ifndef DIVE_WEAKFORMS_FLUID_MASS_HPP_
#define DIVE_WEAKFORMS_FLUID_MASS_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		MassFluidPtr CreateWeakFormMassFluid();

		class MassFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<MassFluid>
		{
		public:
			virtual ~MassFluid() = default;

			static MassFluidPtr Create();
			MassFluidPtr GetPtr();
			ConstMassFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& point, Matrix& output) const override;

		protected:
			MassFluid() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;

			using std::enable_shared_from_this<MassFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_MASS_HPP_ */