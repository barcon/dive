#include "dive_problems_fluid.hpp"
#include "dive_values_scalar_congruent.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace problems {
		ProblemFluidPtr CreateProblemFluid()
		{
			auto res = ProblemFluid::Create();

			return res;
		}
		ProblemFluidPtr CreateProblemFluid(Tag problemTag)
		{
			auto res = ProblemFluid::Create();

			res->SetTag(problemTag);

			return res;
		}
		ProblemFluid::ProblemFluid()
		{
		}
		ProblemFluidPtr ProblemFluid::Create()
		{
			class MakeSharedEnabler : public ProblemFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ProblemFluidPtr ProblemFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemFluid>(shared_from_this());
		}
		ConstProblemFluidPtr ProblemFluid::GetPtr() const
		{
			return const_cast<ProblemFluid*>(this)->GetPtr();
		}
		NumberDof ProblemFluid::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberDof ProblemFluid::GetTotalDof() const
		{
			return totalDof_;
		}
		DofIndex ProblemFluid::GetPivot() const
		{
			return pivot_;
		}
		IScalar3DPtr ProblemFluid::GetTemperature() const
		{
			return temperature_;
		}
		IScalar3DPtr ProblemFluid::GetPressure() const
		{
			return pressure_;
		}
		IMatrix3DPtr ProblemFluid::GetVelocity() const
		{
			return velocity_;
		}
		IMeshPtr ProblemFluid::GetMesh() const
		{
			return mesh_;
		}
		Type ProblemFluid::GetType() const
		{
			return type_;
		}
		Tag ProblemFluid::GetTag() const
		{
			return tag_;
		}
		const BoundaryConditions& ProblemFluid::GetDirichlets() const
		{
			return dirichlets_;
		}
		const Loads& ProblemFluid::GetLoads() const
		{
			return loads_;
		}
		const DofMeshIndices& ProblemFluid::GetDofMeshIndices() const
		{
			return dofMeshIndices_;
		}
		const NodeMeshIndices& ProblemFluid::GetNodeMeshIndices() const
		{
			return  nodeMeshIndices_;
		}
		const DirichletMeshIndices& ProblemFluid::GetDirichletMeshIndices() const
		{
			return dirichletMeshIndices_;
		}
		void ProblemFluid::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void ProblemFluid::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		void ProblemFluid::SetMesh(IMeshPtr mesh)
		{
			mesh_ = mesh;

			if (mesh != nullptr)
			{
				velocity_ = values::CreateValueMatrix3DCongruent(mesh_);
			}

			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemFluid::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemFluid::AddDirichlet(IBoundaryConditionPtr dirichlet)
		{
			dirichlets_.push_back(dirichlet);
		}
		void ProblemFluid::AddLoad(ILoadPtr load)
		{
			loads_.push_back(load);
		}
		void ProblemFluid::Initialize()
		{
			UpdateDofMeshIndices(mesh_, totalDof_, dofMeshIndices_);
			UpdateNodeMeshIndices(mesh_, dofMeshIndices_, nodeMeshIndices_);
			UpdateDirichletIndices(dirichlets_, pivot_, dofMeshIndices_, dirichletMeshIndices_);
			UpdateDirichletLoads(dofMeshIndices_);

			Reorder(mesh_, numberDof_, nodeMeshIndices_, dofMeshIndices_);
		}
		void ProblemFluid::UpdateMeshValues(const Vector& u)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, u(globalIndex));
			}
		}
		void ProblemFluid::UpdateMeshValuesMomentum(const Vector& q)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				const auto& node = dofMeshIndices_[i].node;
				const auto& element = node->GetConnectivity().elements[0];
				const auto& point = element->LocalCoordinates(node);
				const auto& material = element->GetMaterial();

				auto temperature = values::GetValue(temperature_, point, element);
				auto pressure = values::GetValue(pressure_, point, element);
				auto density = material->GetDensity(temperature, pressure);

				dofMeshIndices_[i].node->SetValue(dofIndex, q(globalIndex) / density);
			}
		}
		Sparse ProblemFluid::Mass() const
		{
			TimerStart();

			auto massWeak = weakforms::CreateWeakFormMassFluid();

			auto problemFluid = std::make_shared<ProblemFluid>(*this);
			
			auto res = IntegralForm(massWeak, problemFluid, problemFluid);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemFluid::Stiffness() const
		{
			TimerStart();

			auto stiffnessWeak = weakforms::CreateWeakFormStiffnessFluid();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemFluid = std::make_shared<ProblemFluid>(*this);

			auto res =  IntegralForm(stiffnessWeak, problemFluid, problemFluid);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemFluid::Convection() const
		{
			TimerStart();

			auto convectionWeak = weakforms::CreateWeakFormConvectionFluid();

			auto problemFluid = std::make_shared<ProblemFluid>(*this);

			auto res = IntegralForm(convectionWeak, problemFluid, problemFluid);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemFluid::ConvectionSymmetric() const
		{
			return Sparse();
		}
		Sparse ProblemFluid::ConvectionAsymmetric() const
		{
			return Sparse();
		}
		Sparse ProblemFluid::Stabilization() const
		{
			TimerStart();

			auto stabilizationWeak = weakforms::CreateWeakFormStabilizationFluid();

			auto problemFluid = std::make_shared<ProblemFluid>(*this);

			auto res = IntegralForm(stabilizationWeak, problemFluid, problemFluid);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemFluid::LoadDistributedVolume() const
		{
			TimerStart();

			auto loadDistributedVolumeWeak = weakforms::CreateWeakFormLoadDistributedVolumeFluid();

			auto problemFluid = std::make_shared<ProblemFluid>(*this);

			auto res = Vector(IntegralForm(loadDistributedVolumeWeak, problemFluid, loads_, 0.0), 0);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemFluid::LoadDistributedVolumeStabilization() const
		{
			TimerStart();

			auto loadDistributedVolumeStabilizationWeak = weakforms::CreateWeakFormLoadDistributedVolumeStabilizationFluid();

			auto problemFluid = std::make_shared<ProblemFluid>(*this);

			auto res = Vector(IntegralForm(loadDistributedVolumeStabilizationWeak, problemFluid, loads_, 0.0), 0);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemFluid::Momentum() const
		{
			Vector res(totalDof_, 0.0);
			
			const auto& elements = mesh_->GetElements();

			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				const auto& node = dofMeshIndices_[i].node;
				const auto& element = dofMeshIndices_[i].node->GetConnectivity().elements[0];
				const auto& point = element->LocalCoordinates(node);
				
				auto material = std::static_pointer_cast<material::IMaterialFluid>(element->GetMaterial());
				auto temperature = values::GetValue(temperature_, point, element);
				auto pressure = values::GetValue(pressure_, point, element);
				auto density = material->GetDensity(temperature, pressure);

				res(globalIndex) = density * dofMeshIndices_[i].node->GetValue(dofIndex);
			}

			return res;
		}
		Vector ProblemFluid::Velocity() const
		{
			Vector res(totalDof_, 0.0);

			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				res(globalIndex) = dofMeshIndices_[i].node->GetValue(dofIndex);
			}

			return res;
		}
	} // namespace problems
} // namespace dive