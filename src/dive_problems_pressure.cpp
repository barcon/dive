#include "dive_problems_pressure.hpp"
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
		const BoundaryConditions& ProblemPressure::GetDirichlets() const
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
			
			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemPressure::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemPressure::AddDirichlet(IBoundaryConditionPtr dirichlet)
		{
			dirichlets_.push_back(dirichlet);
		}
		void ProblemPressure::AddLoad(ILoadPtr load)
		{
			loads_.push_back(load);
		}
		void ProblemPressure::Initialize()
		{
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

			auto problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(massWeak, problemPressure, problemPressure);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemPressure::Stiffness() const
		{
			TimerStart();

			auto stiffnessWeak = weakforms::CreateWeakFormStiffnessPressure();

			auto problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(stiffnessWeak, problemPressure, problemPressure);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemPressure::Crossed(IProblemPtr problemMomentum) const
		{
			TimerStart();

			auto crossedWeak = weakforms::CreateWeakFormCrossedPressure();
			crossedWeak->SetProblemMomentum(problemMomentum);

			auto problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(crossedWeak, problemPressure, problemMomentum);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemPressure::Stabilization(IProblemPtr problemMomentum) const
		{
			TimerStart();

			auto stabilizationWeak = weakforms::CreateWeakFormStabilizationPressure();
			stabilizationWeak->SetProblemMomentum(problemMomentum);

			auto problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(stabilizationWeak, problemPressure, problemMomentum);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemPressure::DistributedVolumeDivergence(IProblemPtr problemMomentum) const
		{
			TimerStart();

			auto distributedVolumeDivergencePressureWeak = weakforms::CreateWeakFormDistributedVolumeDivergencePressure();
			distributedVolumeDivergencePressureWeak->SetProblemMomentum(problemMomentum);

			auto problemPressure = std::make_shared<ProblemPressure>(*this);

			auto res = IntegralForm(distributedVolumeDivergencePressureWeak, problemPressure, problemMomentum);

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

#ifdef EILIG_ENABLE_OPENCL
		void ProblemPressure::UpdateMeshValues(const VectorCL& u)
		{
			UpdateMeshValues(u.Convert());
		}
		SparseCL ProblemPressure::Mass(KernelsPtr kernels) const
		{
			return SparseCL(kernels, Mass());
		}
		SparseCL ProblemPressure::Stiffness(KernelsPtr kernels) const
		{
			return SparseCL(kernels, Stiffness());
		}
		SparseCL ProblemPressure::Crossed(KernelsPtr kernels, IProblemPtr problemMomentum) const
		{
			return SparseCL(kernels, Crossed(problemMomentum));
		}
		SparseCL ProblemPressure::Stabilization(KernelsPtr kernels, IProblemPtr problemMomentum) const
		{
			return SparseCL(kernels, Stabilization(problemMomentum));
		}
		SparseCL ProblemPressure::DistributedVolumeDivergence(KernelsPtr kernels, IProblemPtr problemMomentum) const
		{
			return SparseCL(kernels, DistributedVolumeDivergence(problemMomentum));;
		}
		VectorCL ProblemPressure::Pressure(KernelsPtr kernels) const
		{
			return VectorCL(kernels, Pressure());
		}
#endif
	} // namespace problems
} // namespace dive