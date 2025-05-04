#ifndef DIVE_WEAKFORMS_FLUID_CONVECTION_HPP_
#define DIVE_WEAKFORMS_FLUID_CONVECTION_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		ConvectionFluidPtr CreateWeakFormConvectionFluid();

		class ConvectionFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<ConvectionFluid>
		{
		public:
			virtual ~ConvectionFluid() = default;

			static ConvectionFluidPtr Create();
			ConvectionFluidPtr GetPtr();
			ConstConvectionFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

		protected:
			ConvectionFluid() = default;


			Matrix FormVelocity(IElementMappedPtr element, const Vector& local) const;
			Scalar FormDivergence(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_udN(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;

			using std::enable_shared_from_this<ConvectionFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_CONVECTION_HPP_ */