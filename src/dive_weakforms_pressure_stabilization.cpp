#include "dive_weakforms_pressure_stabilization.hpp"
#include "dive_values_scalar_congruent.hpp"
#include "dive_values_matrix_congruent.hpp"
#include "dive_problems.hpp"

namespace dive {
	namespace weakforms {
		StabilizationPressurePtr CreateWeakFormStabilizationPressure()
		{
			auto res = StabilizationPressure::Create();

			return res;
		}
		StabilizationPressurePtr StabilizationPressure::Create()
		{
			class MakeSharedEnabler : public StabilizationPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		StabilizationPressurePtr StabilizationPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<StabilizationPressure>(shared_from_this());
		}
		ConstStabilizationPressurePtr StabilizationPressure::GetPtr() const
		{
			return const_cast<StabilizationPressure*>(this)->GetPtr();
		}
		void StabilizationPressure::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local);
			auto dNp = FormMatrix_dNp(element, local);
			auto du = FormDivergence(element, local);
			auto udN = FormMatrix_udN(element, local);

			output = dNp.Transpose() * (du * N + udN);
		}
		void StabilizationPressure::SetProblemMomentum(IProblemPtr problemMomentum)
		{
			problemMomentum_ = problemMomentum;
		}
		Matrix StabilizationPressure::FormMomentum(IElementMappedPtr element, const Vector& local) const
		{
			const auto& elementMomentum = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()]);

			return elementMomentum->u(local);
		}
		Scalar StabilizationPressure::FormDivergence(IElementMappedPtr element, const Vector& local) const
		{
			const auto& elementMomentum = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()]);
			auto du = eilig::Inverse(elementMomentum->J(local)) * elementMomentum->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix StabilizationPressure::FormMatrix_N(IElementMappedPtr element, const Vector& local) const
		{
			const auto& elementMomentum = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()]);

			auto numberNodes = elementMomentum->GetNumberNodes();
			auto numberDof = elementMomentum->GetNode(0)->GetNumberDof();
			auto N = elementMomentum->N(local);

			Matrix res(numberDof, numberNodes * numberDof, eilig::matrix_zeros);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					res(i, i + j * numberDof) = N(0, j);
				}
			}

			return res;
		}
		Matrix StabilizationPressure::FormMatrix_udN(IElementMappedPtr element, const Vector& local) const
		{
			const auto& elementMomentum = std::dynamic_pointer_cast<elements::IElementMapped>(problemMomentum_->GetMesh()->GetElements()[element->GetElementIndex()]);

			auto numberNodes = elementMomentum->GetNumberNodes();
			auto numberDof = elementMomentum->GetNode(0)->GetNumberDof();
			auto numberDimensions = elementMomentum->GetNumberDimensions();
			auto momentum = FormMomentum(elementMomentum, local);
			auto dN = elementMomentum->InvJ(local) * elementMomentum->dN(local);

			Matrix res(numberDimensions, numberNodes * numberDof, eilig::matrix_zeros);

			for (NodeIndex i = 0; i < numberNodes; ++i)
			{
				res(0, i * numberDof + 0) = momentum(0) * dN(0, i) + momentum(1) * dN(1, i) + momentum(2) * dN(2, i);
				res(1, i * numberDof + 1) = momentum(0) * dN(0, i) + momentum(1) * dN(1, i) + momentum(2) * dN(2, i);
				res(2, i * numberDof + 2) = momentum(0) * dN(0, i) + momentum(1) * dN(1, i) + momentum(2) * dN(2, i);
			}

			return res;
		}
		Matrix StabilizationPressure::FormMatrix_dNp(IElementMappedPtr element, const Vector& local) const
		{
			return element->InvJ(local) * element->dN(local);
		}
	} // namespace problems
} // namespace dive