#include "dive_weakforms_pressure_load_distributed_face_flux.hpp"
#include "dive_meshes.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedFaceFluxPressurePtr CreateWeakFormLoadDistributedFaceFluxPressure()
		{
			auto res = LoadDistributedFaceFluxPressure::Create();

			return res;
		}
		LoadDistributedFaceFluxPressurePtr LoadDistributedFaceFluxPressure::Create()
		{
			class MakeSharedEnabler : public LoadDistributedFaceFluxPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedFaceFluxPressurePtr LoadDistributedFaceFluxPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedFaceFluxPressure>(shared_from_this());
		}
		ConstLoadDistributedFaceFluxPressurePtr LoadDistributedFaceFluxPressure::GetPtr() const
		{
			return const_cast<LoadDistributedFaceFluxPressure*>(this)->GetPtr();
		}
		void LoadDistributedFaceFluxPressure::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedFace>(load);
			auto elementPressure = force->GetElement();
			auto elementIndex = elementPressure->GetElementIndex();
			auto elementVelocity = problemVelocity_->GetMesh()->GetElements()[elementIndex];
			auto faceIndex = force->GetFaceIndex();

			auto N = FormMatrix_N(elementPressure, faceIndex, point);
			auto n = FormVector_n(elementVelocity, faceIndex, point);
			auto q = FormMatrix_q(elementVelocity, momentum_, point);

			output = N.Transpose() * (q.Transpose() * n);
		}
		void LoadDistributedFaceFluxPressure::SetMomentum(const Vector& momentum)
		{
			momentum_ = momentum;
		}
		void LoadDistributedFaceFluxPressure::SetProblemVelocity(IProblemPtr problemVelocity)
		{
			problemVelocity_ = problemVelocity;
		}
		Matrix LoadDistributedFaceFluxPressure::FormMatrix_N(IElementPtr element, FaceIndex faceIndex, const Vector& point) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto N = element->N(point);
			auto helper = element->GetIntegralAreaHelper(faceIndex);

			Matrix res(numberDof, numberNodes * numberDof, 0.0);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					if (element->LocalCoordinates(j)(helper.dim3) == helper.coord3)
					{
						res(i, j * numberDof + i) = N(j);
					}
				}
			}

			return res;
		}
		Vector LoadDistributedFaceFluxPressure::FormVector_n(IElementPtr element, FaceIndex faceIndex, const Vector& point) const
		{
			auto helper = element->GetIntegralAreaHelper(faceIndex);

			Vector du = element->GlobalDerivatives(point, helper.dim1);
			Vector dv = element->GlobalDerivatives(point, helper.dim2);
			Vector dw = eilig::Cross(du, dv);

			Vector normal = dw * (1.0 / eilig::NormP2(dw));

			return normal;
		}
		Matrix LoadDistributedFaceFluxPressure::FormMatrix_q(IElementPtr element, const Vector& momentum, const Vector& point) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto elementIndex = element->GetElementIndex();
			auto aux = element->N(point);
			auto q = Matrix(numberNodes * numberDof, 1, 0.0);
			
			const auto& nodeMeshIndices = problemVelocity_->GetNodeMeshIndices();

			Matrix N(numberDof, numberNodes * numberDof, 0.0);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					N(i, j * numberDof + i) = aux(j);
				}
			}

			for (NodeIndex i = 0; i < numberNodes; ++i)
			{
				for (DofIndex j = 0; j < numberDof; ++j)
				{
					auto globalIndex = nodeMeshIndices[elementIndex][i].dofIndices[j];
					
					q(i * numberDof + j) = momentum(globalIndex);
				}
			}

			return N * q;
		}
	} // namespace problems
} // namespace dive