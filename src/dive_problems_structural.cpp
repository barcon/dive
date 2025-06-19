#include "dive_problems_structural.hpp"
#include "dive_elements_mass.hpp"
#include "dive_elements_spring.hpp"
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
		const BoundaryConditions& ProblemStructural::GetDirichlets() const
		{
			return dirichlets_;
		}
		const BoundaryConditions& ProblemStructural::GetVelocity() const
		{
			return velocities_;
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

			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemStructural::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemStructural::AddDirichlet(IBoundaryConditionPtr dirichlet)
		{
			dirichlets_.push_back(dirichlet);
		}
		void ProblemStructural::AddVelocity(IBoundaryConditionPtr velocity)
		{
			velocities_.push_back(velocity);
		}
		void ProblemStructural::AddLoad(ILoadPtr load)
		{
			loads_.push_back(load);
		}
		void ProblemStructural::Initialize()
		{
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
			massWeak->SetTemperature(temperature_);
			massWeak->SetPressure(pressure_);

			const auto& problemStructural = std::make_shared<ProblemStructural>(*this);
			const auto& elements = mesh_->GetElements();
			const auto& nodeMeshIndices = GetNodeMeshIndices();

			Sparse global(totalDof_, totalDof_);
			Matrix local;

			for (ElementIndex i = 0; i < elements.size(); ++i)
			{
				auto numberNodes = elements[i]->GetNumberNodes();
				auto numberDof = elements[i]->GetNumberDof();

				if (elements[i]->IsMapped())
				{
					std::dynamic_pointer_cast<elements::IElementMapped>(elements[i])->IntegralWeakFormElement(massWeak, local);
				}
				else if (elements[i]->GetType() == elements::element_mass)
				{
					std::dynamic_pointer_cast<elements::ElementMass>(elements[i])->Mass(local);
				}
				else
				{
					continue;
				}

				for (NodeIndex m = 0; m < numberNodes; ++m)
				{
					for (NodeIndex n = 0; n < numberNodes; ++n)
					{
						for (DofIndex dof1 = 0; dof1 < numberDof; ++dof1)
						{
							for (DofIndex dof2 = 0; dof2 < numberDof; ++dof2)
							{
								auto aux = global.GetValue(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2]);
								aux += local.GetValue(m * numberDof + dof1, n * numberDof + dof2);

								global.SetValue(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2], aux);
							}
						}
					}
				}
			}

			TimerElapsed(__FUNCTION__);

			return global;
		}
		Sparse ProblemStructural::Stiffness() const
		{
			TimerStart();

			auto stiffnessWeak = weakforms::CreateWeakFormStiffnessStructural();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			const auto& problemStructural = std::make_shared<ProblemStructural>(*this);
			const auto& elements = mesh_->GetElements();
			const auto& nodeMeshIndices = GetNodeMeshIndices();

			Sparse global(totalDof_, totalDof_);
			Matrix local;

			for (ElementIndex i = 0; i < elements.size(); ++i)
			{
				auto numberNodes = elements[i]->GetNumberNodes();
				auto numberDof = elements[i]->GetNumberDof();

				if (elements[i]->IsMapped())
				{
					std::dynamic_pointer_cast<elements::IElementMapped>(elements[i])->IntegralWeakFormElement(stiffnessWeak, local);
				}
				else if (elements[i]->GetType() == elements::element_spring)
				{
					std::dynamic_pointer_cast<elements::ElementSpring>(elements[i])->Stiffness(local);
				}
				else
				{
					continue;
				}

				for (NodeIndex m = 0; m < numberNodes; ++m)
				{
					for (NodeIndex n = 0; n < numberNodes; ++n)
					{
						for (DofIndex dof1 = 0; dof1 < numberDof; ++dof1)
						{
							for (DofIndex dof2 = 0; dof2 < numberDof; ++dof2)
							{
								auto aux = global.GetValue(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2]);
								aux += local.GetValue(m * numberDof + dof1, n * numberDof + dof2);

								global.SetValue(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2], aux);
							}
						}
					}
				}
			}

			TimerElapsed(__FUNCTION__);
			
			return global;
		}
		Vector ProblemStructural::LoadDistributedEdge() const
		{
			TimerStart();

			auto loadDistributedEdgeWeak = weakforms::CreateWeakFormLoadDistributedEdgeStructural();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res = IntegralForm(loadDistributedEdgeWeak, problemStructural, loads_);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::LoadDistributedFace() const
		{
			TimerStart();

			auto loadDistributedFaceWeak = weakforms::CreateWeakFormLoadDistributedFaceStructural();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res = IntegralForm(loadDistributedFaceWeak, problemStructural, loads_);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::LoadDistributedVolume() const
		{
			TimerStart();

			auto loadDistributedVolumeWeak = weakforms::CreateWeakFormLoadDistributedVolumeStructural();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res = IntegralForm(loadDistributedVolumeWeak, problemStructural, loads_);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::LoadNode() const
		{
			TimerStart();

			auto problemStructural = std::make_shared<ProblemStructural>(*this);

			auto res = Vector(IntegralForm(nullptr, problemStructural, loads_), 0);

			TimerElapsed(__FUNCTION__);

			return res;
		}
		Vector ProblemStructural::Displacement() const
		{
			Vector res(totalDof_, 0.0);

			for (Index i = 0; i < velocities_.size(); ++i)
			{
				auto dofIndex = dirichlets_[i]->GetDofIndex();
				auto globalIndex = dirichlets_[i]->GetNode()->GetConnectivity().globalDofIndices[dofIndex];

				res(globalIndex) = dirichlets_[i]->GetValue();
			}

			return res;
		}
		Vector ProblemStructural::Velocity() const
		{
			Vector res(totalDof_, 0.0);

			for (Index i = 0; i < velocities_.size(); ++i)
			{
				auto dofIndex = velocities_[i]->GetDofIndex();
				auto globalIndex = velocities_[i]->GetNode()->GetConnectivity().globalDofIndices[dofIndex];
				
				res(globalIndex) = velocities_[i]->GetValue();
			}

			return res;
		}
	} // namespace problems
} // namespace dive

/*

Vector res(totalDof_, 0.0);

for (Index i = 0; i < dofMeshIndices_.size(); ++i)
{
	auto globalIndex = dofMeshIndices_[i].globalIndex;
	auto dofIndex = dofMeshIndices_[i].dofIndex;

	res(globalIndex) = dofMeshIndices_[i].node->GetValue(dofIndex);
}

return res;

*/