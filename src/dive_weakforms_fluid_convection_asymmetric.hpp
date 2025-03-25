#ifndef DIVE_WEAKFORMS_FLUID_CONVECTION_ASYMMETRIC_HPP_
#define DIVE_WEAKFORMS_FLUID_CONVECTION_ASYMMETRIC_HPP_

#include "dive_weakforms_basic.hpp"

namespace dive
{
    namespace weakforms
    {
		ConvectionAsymmetricFluidPtr CreateWeakFormConvectionAsymmetricFluid();

		class ConvectionAsymmetricFluid : public WeakFormElement, virtual public std::enable_shared_from_this<ConvectionAsymmetricFluid>
		{
		public:
			virtual ~ConvectionAsymmetricFluid() = default;

			static ConvectionAsymmetricFluidPtr Create();
			ConvectionAsymmetricFluidPtr GetPtr();
			ConstConvectionAsymmetricFluidPtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

		protected:
			ConvectionAsymmetricFluid() = default;


			Matrix FormDivergence(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Matrix FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			using std::enable_shared_from_this<ConvectionAsymmetricFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_CONVECTION_ASYMMETRIC_HPP_ */