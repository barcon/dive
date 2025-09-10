#include "dive_problems_thermal.hpp"
#include "dive_values_scalar_congruent.hpp"

namespace dive {
	namespace problems {
		ProblemThermalPtr CreateProblemThermal()
		{
			auto res = ProblemThermal::Create();

			return res;
		}
		ProblemThermalPtr CreateProblemThermal(Tag problemTag)
		{
			auto res = ProblemThermal::Create();

			res->SetTag(problemTag);

			return res;
		}
		ProblemThermalPtr ProblemThermal::Create()
		{
			class MakeSharedEnabler : public ProblemThermal
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ProblemThermal::ProblemThermal()
		{
		}
		ProblemThermalPtr ProblemThermal::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemThermal>(shared_from_this());
		}
		ConstProblemThermalPtr ProblemThermal::GetPtr() const
		{
			return const_cast<ProblemThermal*>(this)->GetPtr();
		}
		NumberDof ProblemThermal::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberDof ProblemThermal::GetTotalDof() const
		{
			return totalDof_;
		}
		DofIndex ProblemThermal::GetPivot() const
		{
			return pivot_;
		}
		IScalar3DPtr ProblemThermal::GetTemperature() const
		{
			return temperature_;
		}
		IScalar3DPtr ProblemThermal::GetPressure() const
		{
			return pressure_;
		}
		IMeshPtr ProblemThermal::GetMesh() const
		{
			return mesh_;
		}
		Type ProblemThermal::GetType() const
		{
			return type_;
		}
		Tag ProblemThermal::GetTag() const
		{
			return tag_;
		}
		const BoundaryConditions& ProblemThermal::GetDirichlets() const
		{
			return dirichlets_;
		}
		const Loads& ProblemThermal::GetLoads() const
		{
			return loads_;
		}
		const DofMeshIndices& ProblemThermal::GetDofMeshIndices() const
		{
			return dofMeshIndices_;
		}
		const NodeMeshIndices& ProblemThermal::GetNodeMeshIndices() const
		{
			return  nodeMeshIndices_;
		}
		const DirichletMeshIndices& ProblemThermal::GetDirichletMeshIndices() const
		{
			return dirichletMeshIndices_;
		}
		void ProblemThermal::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		void ProblemThermal::SetMesh(IMeshPtr mesh)
		{
			mesh_ = mesh;
			temperature_ = values::CreateValueScalar3DCongruent(mesh_);

			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemThermal::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemThermal::AddDirichlet(IBoundaryConditionPtr dirichlet)
		{
			dirichlets_.push_back(dirichlet);
		}
		void ProblemThermal::AddLoad(ILoadPtr load)
		{
			loads_.push_back(load);
		}
		void ProblemThermal::Initialize()
		{
			UpdateDofMeshIndices(mesh_, totalDof_, dofMeshIndices_);
			UpdateNodeMeshIndices(mesh_, dofMeshIndices_, nodeMeshIndices_);
			UpdateDirichletIndices(dirichlets_, pivot_, dofMeshIndices_, dirichletMeshIndices_);
			UpdateDirichletLoads(dofMeshIndices_);

			Reorder(mesh_, numberDof_, nodeMeshIndices_, dofMeshIndices_);
		}
		void ProblemThermal::UpdateMeshValues(const Vector& u)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, u(globalIndex));
			}
		}
		Sparse ProblemThermal::Mass() const
		{
			TimerStart();

			auto massWeak = weakforms::CreateWeakFormMassThermal();
			massWeak->SetTemperature(temperature_);
			massWeak->SetPressure(pressure_);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);

			auto res = IntegralForm(massWeak, problemThermal, problemThermal);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemThermal::Stiffness() const
		{
			TimerStart();

			auto stiffnessWeak = weakforms::CreateWeakFormStiffnessThermal();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);

			auto res = IntegralForm(stiffnessWeak, problemThermal, problemThermal);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemThermal::StiffnessParallel() const
		{
			TimerStart();

			auto stiffnessWeak = weakforms::CreateWeakFormStiffnessThermal();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);

			auto res = IntegralFormParallel(stiffnessWeak, problemThermal, problemThermal);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemThermal::Convection(IProblemPtr problemMomentum) const
		{
			TimerStart();

			auto convectionWeak = weakforms::CreateWeakFormConvectionThermal();
			convectionWeak->SetTemperature(temperature_);
			convectionWeak->SetPressure(pressure_);
			convectionWeak->SetProblemMomentum(problemMomentum);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);

			auto res = IntegralForm(convectionWeak, problemThermal, problemThermal);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemThermal::Stabilization(IProblemPtr problemMomentum) const
		{
			TimerStart();

			auto stabilizationWeak = weakforms::CreateWeakFormStabilizationThermal();
			stabilizationWeak->SetTemperature(temperature_);
			stabilizationWeak->SetPressure(pressure_);
			stabilizationWeak->SetProblemMomentum(problemMomentum);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);

			auto res = IntegralForm(stabilizationWeak, problemThermal, problemThermal);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemThermal::Energy() const
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