#ifndef DIVE_WEAKFORMS_FLUID_CONVECTION_ASYMMETRIC_HPP_
#define DIVE_WEAKFORMS_FLUID_CONVECTION_ASYMMETRIC_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		ConvectionAsymmetricFluidPtr CreateWeakFormConvectionAsymmetricFluid();

		class ConvectionAsymmetricFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<ConvectionAsymmetricFluid>
		{
		public:
			virtual ~ConvectionAsymmetricFluid() = default;

			static ConvectionAsymmetricFluidPtr Create();
			ConvectionAsymmetricFluidPtr GetPtr();
			ConstConvectionAsymmetricFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output) const override;

		protected:
			ConvectionAsymmetricFluid() = default;


			Matrix FormDivergence(IElementMappedPtr element, const Vector& local) const;
			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local) const;

			using std::enable_shared_from_this<ConvectionAsymmetricFluid>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_FLUID_CONVECTION_ASYMMETRIC_HPP_ */