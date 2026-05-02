#ifndef DIVE_WEAKFORM_FLUID_MASS_HPP_
#define DIVE_WEAKFORM_FLUID_MASS_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		MassFluidPtr CreateWeakFormMassFluid();

		class MassFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<MassFluid>
		{
		public:
			virtual ~MassFluid() = default;

			static MassFluidPtr Create();
			MassFluidPtr GetPtr();
			ConstMassFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& point, Matrix& output, const CacheIndex& cacheIndex) const override;

		protected:
			MassFluid() = default;

			Matrix FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			using std::enable_shared_from_this<MassFluid>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_FLUID_MASS_HPP_ */