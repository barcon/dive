#ifndef DIVE_WEAKFORMS_FLUID_CONVECTION_SYMMETRIC_HPP_
#define DIVE_WEAKFORMS_FLUID_CONVECTION_SYMMETRIC_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		ConvectionSymmetricFluidPtr CreateWeakFormConvectionSymmetricFluid();

		class ConvectionSymmetricFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<ConvectionSymmetricFluid>
		{
		public:
			virtual ~ConvectionSymmetricFluid() = default;

			static ConvectionSymmetricFluidPtr Create();
			ConvectionSymmetricFluidPtr GetPtr();
			ConstConvectionSymmetricFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

		protected:
			ConvectionSymmetricFluid() = default;


			Matrix FormDivergence(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;

			using std::enable_shared_from_this<ConvectionSymmetricFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_CONVECTION_SYMMETRIC_HPP_ */