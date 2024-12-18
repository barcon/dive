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
		void StabilizationPressure::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto N = FormMatrix_N(element, local);
			auto dNp = FormMatrix_dNp(element, local, cacheIndex);
			auto du = FormDivergence(element, local);
			auto udN = FormMatrix_udN(element, local);

			output = dNp.Transpose() * (du * N + udN);
		}
		void StabilizationPressure::SetProblemVelocity(IProblemPtr problemVelocity)
		{
			problemVelocity_ = problemVelocity;
		}
		Matrix StabilizationPressure::FormVelocity(IElementPtr element, const Vector& local) const
		{
			const auto& elementVelocity = problemVelocity_->GetMesh()->GetElements()[element->GetElementIndex()];

			return elementVelocity->u(local);
		}
		Scalar StabilizationPressure::FormDivergence(IElementPtr element, const Vector& local) const
		{
			const auto& elementVelocity = problemVelocity_->GetMesh()->GetElements()[element->GetElementIndex()];
			auto du = eilig::Inverse(elementVelocity->J(local)) * elementVelocity->du(local);

			Scalar divergence{ 0.0 };

			for (Index i = 0; (i < du.GetRows()) && (i < du.GetCols()); ++i)
			{
				divergence += du(i, i);
			}

			return divergence;
		}
		Matrix StabilizationPressure::FormMatrix_N(IElementPtr element, const Vector& local) const
		{
			const auto& elementVelocity = problemVelocity_->GetMesh()->GetElements()[element->GetElementIndex()];

			auto numberNodes = elementVelocity->GetNumberNodes();
			auto numberDof = elementVelocity->GetNode(0)->GetNumberDof();
			auto N = elementVelocity->N(local);

			Matrix res(numberDof, numberNodes * numberDof, 0.0);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					res(i, i + j * numberDof) = N(0, j);
				}
			}

			return res;
		}
		Matrix StabilizationPressure::FormMatrix_udN(IElementPtr element, const Vector& local) const
		{
			const auto& elementVelocity = problemVelocity_->GetMesh()->GetElements()[element->GetElementIndex()];

			auto numberNodes = elementVelocity->GetNumberNodes();
			auto numberDof = elementVelocity->GetNode(0)->GetNumberDof();
			auto dimension = elementVelocity->GetDimension();
			auto velocity = FormVelocity(elementVelocity, local);
			auto dN = elementVelocity->InvJ(local) * elementVelocity->dN(local);

			Matrix res(dimension, numberNodes * numberDof, 0.0);

			for (NodeIndex i = 0; i < numberNodes; ++i)
			{
				res(0, i * numberDof + 0) = velocity(0) * dN(0, i) + velocity(1) * dN(1, i) + velocity(2) * dN(2, i);
				res(1, i * numberDof + 1) = velocity(0) * dN(0, i) + velocity(1) * dN(1, i) + velocity(2) * dN(2, i);
				res(2, i * numberDof + 2) = velocity(0) * dN(0, i) + velocity(1) * dN(1, i) + velocity(2) * dN(2, i);
			}

			return res;
		}
		Matrix StabilizationPressure::FormMatrix_dNp(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->InvJ(local, cacheIndex) * element->dN(local, cacheIndex);
		}
	} // namespace problems
} // namespace dive