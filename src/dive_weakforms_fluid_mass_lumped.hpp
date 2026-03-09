#ifndef DIVE_WEAKFORMS_FLUID_MASS_LUMPED_HPP_
#define DIVE_WEAKFORMS_FLUID_MASS_LUMPED_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		MassLumpedFluidPtr CreateWeakFormMassLumpedFluid();

		class MassLumpedFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<MassLumpedFluid>
		{
		public:
			virtual ~MassLumpedFluid() = default;

			static MassLumpedFluidPtr Create();
			MassLumpedFluidPtr GetPtr();
			ConstMassLumpedFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& point, Matrix& output, const CacheIndex& cacheIndex) const override;

		protected:
			MassLumpedFluid() = default;

			Matrix FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			using std::enable_shared_from_this<MassLumpedFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_MASS_LUMPED_HPP_ */