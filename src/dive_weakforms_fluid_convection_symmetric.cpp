#include "dive_weakforms_fluid_convection_symmetric.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		ConvectionSymmetricFluidPtr CreateWeakFormConvectionSymmetricFluid()
		{
			auto res = ConvectionSymmetricFluid::Create();

			return res;
		}
		ConvectionSymmetricFluidPtr ConvectionSymmetricFluid::Create()
		{
			class MakeSharedEnabler : public ConvectionSymmetricFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ConvectionSymmetricFluidPtr ConvectionSymmetricFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<ConvectionSymmetricFluid>(shared_from_this());
		}
		ConstConvectionSymmetricFluidPtr ConvectionSymmetricFluid::GetPtr() const
		{
			return const_cast<ConvectionSymmetricFluid*>(this)->GetPtr();
		}
		void ConvectionSymmetricFluid::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto Z = FormMatrix_Z(element, local, cacheIndex);
			auto NN = FormMatrix_NN(element, local, cacheIndex);
			auto I = eilig::Matrix(1, 1, eilig::matrix_diagonal);
			auto divergence = FormDivergence(element, local, cacheIndex);

			output = NN * I * (divergence * Z);
		}
		Matrix ConvectionSymmetricFluid::FormDivergence(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			const auto& du = element->InvJ(cacheIndex) * element->dN(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return Matrix();
		}
		Matrix ConvectionSymmetricFluid::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
		Matrix ConvectionSymmetricFluid::FormMatrix_Z(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto numberDof = element->GetNumberDof();
			auto numberNodes = element->GetNumberNodes();
			Matrix res(numberNodes, numberDof * numberNodes, eilig::matrix_zeros);

			for (Index i = 0; i < numberNodes; ++i)
			{

			}

			return Matrix();
		}

	} // namespace problems
} // namespace dive