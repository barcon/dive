#include "dive_problems_structural.hpp"
#include "dive_selection.hpp"
#include "dive_values_scalar_congruent.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace problems {
		ProblemStructuralPtr CreateProblemStructural()
		{
			auto res = ProblemStructural::Create();

			return res;
		}
		ProblemStructuralPtr CreateProblemStructural(Tag problemTag)
		{
			auto res = ProblemStructural::Create();

			res->SetTag(problemTag);

			return res;
		}
		ProblemStructural::ProblemStructural()
		{
		}
		ProblemStructuralPtr ProblemStructural::Create()
		{
			class MakeSharedEnabler : public ProblemStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		ProblemStructuralPtr ProblemStructural::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemStructural>(shared_from_this());
		}
		ConstProblemStructuralPtr ProblemStructural::GetPtr() const
		{
			return const_cast<ProblemStructural*>(this)->GetPtr();
		}
		NumberDof ProblemStructural::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberDof ProblemStructural::GetTotalDof() const
		{
			return totalDof_;
		}
		DofIndex ProblemStructural::GetPivot() const
		{
			return pivot_;
		}
		ITimerPtr ProblemStructural::GetTimer() const
		{
			return timer_;
		}
		IScalar3DPtr ProblemStructural::GetTemperature() const
		{
			return temperature_;
		}
		IScalar3DPtr ProblemStructural::GetPressure() const
		{
			return pressure_;
		}
		IMeshPtr ProblemStructural::GetMesh() const
		{
			return mesh_;
		}
		Type ProblemStructural::GetType() const
		{
			return type_;
		}
		Tag ProblemStructural::GetTag() const
		{
			return tag_;
		}
		const Dirichlets& ProblemStructural::GetDirichlets() const
		{
			return dirichlets_;
		}
		const Loads& ProblemStructural::GetLoads() const
		{
			return loads_;
		}
		const DofMeshIndices& ProblemStructural::GetDofMeshIndices() const
		{
			return dofMeshIndices_;
		}
		const NodeMeshIndices& ProblemStructural::GetNodeMeshIndices() const
		{
			return  nodeMeshIndices_;
		}
		const DirichletMeshIndices& ProblemStructural::GetDirichletMeshIndices() const
		{
			return dirichletMeshIndices_;
		}
		void ProblemStructural::SetTimer(ITimerPtr timer)
		{
			timer_ = timer;
		}
		void ProblemStructural::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void ProblemStructural::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		void ProblemStructural::SetMesh(IMeshPtr mesh)
		{
			mesh_ = mesh;
		}
		void ProblemStructural::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemStructural::AddDirichlet(IDirichletPtr dirichlet)
		{
			dirichlets_.push_back(dirichlet);
		}
		void ProblemStructural::AddLoad(ILoadPtr load)
		{
			loads_.push_back(load);
		}
		void ProblemStructural::Initialize()
		{
			UpdateMeshElements(mesh_, numberDof_);
			UpdateDofMeshIndices(mesh_, totalDof_, dofMeshIndices_);
			UpdateNodeMeshIndices(mesh_, dofMeshIndices_, nodeMeshIndices_);
			UpdateDirichletIndices(dirichlets_, pivot_, dofMeshIndices_, dirichletMeshIndices_);
			UpdateDirichletLoads(dofMeshIndices_);

			Reorder(mesh_, numberDof_, nodeMeshIndices_, dofMeshIndices_);
		}
		void ProblemStructural::UpdateMeshValues(const Vector& u)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, u(globalIndex));
			}
		}
		Sparse ProblemStructural::Mass() const
		{
			TimerStart();

			auto massWeak = weakforms::CreateWeakFormMassStructural();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);
			
			auto res = IntegralForm(massWeak, problemStructural, problemStructural);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Sparse ProblemStructural::Stiffness() const
		{
			TimerStart();

			auto stiffnessWeak = weakforms::CreateWeakFormStiffnessStructural();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res =  IntegralForm(stiffnessWeak, problemStructural, problemStructural);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::LoadDistributedVolume() const
		{
			TimerStart();

			auto loadDistributedVolumeWeak = weakforms::CreateWeakFormLoadDistributedVolumeStructural();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res = Vector(IntegralForm(loadDistributedVolumeWeak, problemStructural, loads_), 0);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::LoadDistributedFace() const
		{
			TimerStart();

			auto loadDistributedFaceWeak = weakforms::CreateWeakFormLoadDistributedFaceStructural();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res = Vector(IntegralForm(loadDistributedFaceWeak, problemStructural, loads_), 0);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::LoadDistributedEdge() const
		{
			TimerStart();

			auto loadDistributedEdgeWeak = weakforms::CreateWeakFormLoadDistributedEdgeStructural();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res = Vector(IntegralForm(loadDistributedEdgeWeak, problemStructural, loads_), 0);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::LoadNode() const
		{
			TimerStart();

			auto loadNodalWeak = weakforms::CreateWeakFormLoadNodeStructural();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res = Vector(IntegralForm(loadNodalWeak, problemStructural, loads_), 0);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::Displacement() const
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