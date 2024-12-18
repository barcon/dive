#include "dive_problems_pressure.hpp"
#include "dive_selection.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace problems {
		ProblemPressurePtr CreateProblemPressure()
		{
			auto res = ProblemPressure::Create();

			return res;
		}
		ProblemPressurePtr CreateProblemPressure(Tag problemTag)
		{
			auto res = ProblemPressure::Create();

			res->SetTag(problemTag);

			return res;
		}
		ProblemPressurePtr ProblemPressure::Create()
		{
			class MakeSharedEnabler : public ProblemPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ProblemPressure::ProblemPressure()
		{
		}
		ProblemPressurePtr ProblemPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemPressure>(shared_from_this());
		}
		ConstProblemPressurePtr ProblemPressure::GetPtr() const
		{
			return const_cast<ProblemPressure*>(this)->GetPtr();
		}
		NumberDof ProblemPressure::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberDof ProblemPressure::GetTotalDof() const
		{
			return totalDof_;
		}
		DofIndex ProblemPressure::GetPivot() const
		{
			return pivot_;
		}
		ITimerPtr ProblemPressure::GetTimer() const
		{
			return timer_;
		}
		IScalar3DPtr ProblemPressure::GetTemperature() const
		{
			return temperature_;
		}
		IScalar3DPtr ProblemPressure::GetPressure() const
		{
			return pressure_;
		}
		IMatrix3DPtr ProblemPressure::GetVelocity() const
		{
			return velocity_;
		}
		IMeshPtr ProblemPressure::GetMesh() const
		{
			return mesh_;
		}
		Type ProblemPressure::GetType() const
		{
			return type_;
		}
		Tag ProblemPressure::GetTag() const
		{
			return tag_;
		}
		const Dirichlets& ProblemPressure::GetDirichlets() const
		{
			return dirichlets_;
		}
		const Loads& ProblemPressure::GetLoads() const
		{
			return loads_;
		}
		const DofMeshIndices& ProblemPressure::GetDofMeshIndices() const
		{
			return dofMeshIndices_;
		}
		const NodeMeshIndices& ProblemPressure::GetNodeMeshIndices() const
		{
			return  nodeMeshIndices_;
		}
		const DirichletMeshIndices& ProblemPressure::GetDirichletMeshIndices() const
		{
			return dirichletMeshIndices_;
		}
		void ProblemPressure::SetTimer(ITimerPtr timer)
		{
			timer_ = timer;
		}
		void ProblemPressure::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void ProblemPressure::SetVelocity(IMatrix3DPtr velocity)
		{
			velocity_ = velocity;
		}
		void ProblemPressure::SetMesh(IMeshPtr mesh)
		{
			mesh_ = mesh;
			pressure_ = values::CreateValueScalar3DCongruent(mesh_);
		}
		void ProblemPressure::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemPressure::AddDirichlet(IDirichletPtr dirichlet)
		{
			dirichlets_.push_back(dirichlet);
		}
		void ProblemPressure::AddLoad(ILoadPtr load)
		{
			loads_.push_back(load);
		}
		void ProblemPressure::Initialize()
		{
			UpdateMeshElements(mesh_, numberDof_);
			UpdateDofMeshIndices(mesh_, totalDof_, dofMeshIndices_);
			UpdateNodeMeshIndices(mesh_, dofMeshIndices_, nodeMeshIndices_);
			UpdateDirichletIndices(dirichlets_, pivot_, dofMeshIndices_, dirichletMeshIndices_);
			UpdateDirichletLoads(dofMeshIndices_);

			Reorder(mesh_, numberDof_, nodeMeshIndices_, dofMeshIndices_);
		}
		void ProblemPressure::UpdateMeshValues(const Vector& u)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, u(globalIndex));
			}
		}
		Sparse ProblemPressure::Mass() const
		{
			TimerStart();

			auto massWeak = weakforms::CreateWeakFormMassPressure();
			massWeak->SetTemperature(temperature_);
			massWeak->SetPressure(pressure_);
			massWeak->SetVelocity(velocity_);

			IProblemPtr problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(massWeak, problemPressure, problemPressure);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemPressure::Stiffness() const
		{
			TimerStart();

			auto stiffnessWeak = weakforms::CreateWeakFormStiffnessPressure();

			IProblemPtr problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(stiffnessWeak, problemPressure, problemPressure);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemPressure::Crossed(IProblemPtr problemVelocity) const
		{
			TimerStart();

			auto crossedWeak = weakforms::CreateWeakFormCrossedPressure();
			crossedWeak->SetProblemVelocity(problemVelocity);

			IProblemPtr problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(crossedWeak, problemPressure, problemVelocity);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemPressure::Stabilization(IProblemPtr problemVelocity) const
		{
			TimerStart();

			auto stabilizationWeak = weakforms::CreateWeakFormStabilizationPressure();
			stabilizationWeak->SetProblemVelocity(problemVelocity);

			IProblemPtr problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(stabilizationWeak, problemPressure, problemVelocity);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemPressure::LoadDistributedFaceFlux(IProblemPtr problemVelocity, const Vector& momentum) const
		{
			TimerStart();

			auto loadDistributedFaceFluxPressureWeak = weakforms::CreateWeakFormLoadDistributedFaceFluxPressure();
			loadDistributedFaceFluxPressureWeak->SetProblemVelocity(problemVelocity);
			loadDistributedFaceFluxPressureWeak->SetMomentum(momentum);

			IProblemPtr problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(loadDistributedFaceFluxPressureWeak, problemPressure, loads_);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemPressure::Pressure() const
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