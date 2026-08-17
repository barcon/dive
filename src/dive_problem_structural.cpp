#include "dive_problem_structural.hpp"
#include "dive_element_mass.hpp"
#include "dive_element_combined.hpp"
#include "dive_value_scalar_congruent.hpp"
#include "dive_value_matrix_congruent.hpp"

#include "dive_weakform.hpp"
#include "dive_weakform_structural_mass.hpp"
#include "dive_weakform_structural_stiffness.hpp"
#include "dive_weakform_structural_load_distributed_volume.hpp"
#include "dive_weakform_structural_load_distributed_face.hpp"
#include "dive_weakform_structural_load_distributed_edge.hpp"

namespace dive {
	namespace problem {
		ProblemStructuralPtr CreateProblemStructural(Tag problemTag, IMeshPtr mesh)
		{
			return ProblemStructural::Create(problemTag, mesh);
		}
		ProblemStructuralPtr ProblemStructural::Create(Tag problemTag, IMeshPtr mesh)
		{
			class MakeSharedEnabler : public ProblemStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			res->SetTag(problemTag);
			res->SetMesh(mesh);

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
		IScalarCoordinatesPtr ProblemStructural::GetTemperature() const
		{
			return temperature_;
		}
		IScalarCoordinatesPtr ProblemStructural::GetPressure() const
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
		Loads& ProblemStructural::GetLoads()
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
		void ProblemStructural::SetTemperature(IScalarCoordinatesPtr temperature)
		{
			if (temperature == nullptr)
			{
				throw std::invalid_argument("Temperature cannot be null.");
			}

			if (temperature->GetNumberCoordinates() != mesh_->GetNumberCoordinates())
			{
				throw std::invalid_argument("Temperature number of coordinates must match mesh number of coordinates.");
			}

			temperature_ = temperature;
		}
		void ProblemStructural::SetPressure(IScalarCoordinatesPtr pressure)
		{
			if (pressure == nullptr)
			{
				throw std::invalid_argument("Pressure cannot be null.");
			}

			if (pressure->GetNumberCoordinates() != mesh_->GetNumberCoordinates())
			{
				throw std::invalid_argument("Pressure number of coordinates must match mesh number of coordinates.");
			}

			pressure_ = pressure;;
		}
		void ProblemStructural::SetMesh(IMeshPtr mesh)
		{
			if (mesh == nullptr)
			{
				throw std::invalid_argument("Mesh cannot be null.");
			}

			mesh_ = mesh;

			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemStructural::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemStructural::ApplyLoad(ILoadPtr load)
		{
			if (load == nullptr)
			{
				throw std::invalid_argument("Load cannot be null.");
			}

			loads_.push_back(load);
		}
		void ProblemStructural::Initialize()
		{
			Dirichlets dirichlets;

			for (auto& load : loads_)
			{
				if (load->GetType() == load::load_dirichlet)
				{
					dirichlets.push_back(std::static_pointer_cast<load::ILoadDirichlet>(load));
				}
			}

			UpdateDofMeshIndices(mesh_, totalDof_, dofMeshIndices_);
			UpdateNodeMeshIndices(mesh_, dofMeshIndices_, nodeMeshIndices_);
			UpdateDirichletIndices(dirichlets, pivot_, dofMeshIndices_, dirichletMeshIndices_);
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
		void ProblemStructural::UpdateMeshValues(const Vector& u0, const Vector& u1)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				if (globalIndex < pivot_)
				{
					dofMeshIndices_[i].node->SetValue(dofIndex, u0(globalIndex));
				}
				else
				{
					dofMeshIndices_[i].node->SetValue(dofIndex, u1(globalIndex - pivot_));

				}
			}
		}
		Sparse ProblemStructural::Mass(bool lumped) const
		{
			auto massWeak = weakform::CreateWeakFormMassStructural();
			massWeak->SetTemperature(temperature_);
			massWeak->SetPressure(pressure_);

			const auto& problemStructural = std::make_shared<ProblemStructural>(*this);
			const auto& elements = mesh_->GetElements();
			const auto& nodeMeshIndices = GetNodeMeshIndices();

			Sparse res(totalDof_, totalDof_);
			Matrix local;

			for (ElementIndex i = 0; i < elements.size(); ++i)
			{
				auto numberNodes = elements[i]->GetNumberNodes();
				auto numberDof = elements[i]->GetNumberDof();

				if (elements[i]->IsMapped())
				{
					std::dynamic_pointer_cast<element::IElementMapped>(elements[i])->IntegralWeakFormElement(massWeak, local);
				}
				else if (elements[i]->GetType() == element::element_mass)
				{
					std::dynamic_pointer_cast<element::ElementMass>(elements[i])->Mass(local);
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
								auto aux = res.GetValue(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2]);
								aux += local.GetValue(m * numberDof + dof1, n * numberDof + dof2);

								res.Equal(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2], aux);
							}
						}
					}
				}
			}

			if (lumped)
			{
				auto trace = res.Trace();
				auto mass = res.Sum();

				res = (mass / trace) * res.Diagonal();
			}

			return res;
		}
		Sparse ProblemStructural::Stiffness() const
		{
			auto stiffnessWeak = weakform::CreateWeakFormStiffnessStructural();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			const auto& problemStructural = std::make_shared<ProblemStructural>(*this);
			const auto& elements = mesh_->GetElements();
			const auto& nodeMeshIndices = GetNodeMeshIndices();

			Sparse res(totalDof_, totalDof_);
			Matrix local;

			for (ElementIndex i = 0; i < elements.size(); ++i)
			{
				auto numberNodes = elements[i]->GetNumberNodes();
				auto numberDof = elements[i]->GetNumberDof();

				if (elements[i]->IsMapped())
				{
					std::dynamic_pointer_cast<element::IElementMapped>(elements[i])->IntegralWeakFormElement(stiffnessWeak, local);
				}
				else if (elements[i]->GetType() == element::element_combined)
				{
					std::dynamic_pointer_cast<element::ElementCombined>(elements[i])->Stiffness(local);
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
								auto aux = res.GetValue(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2]);
								aux += local.GetValue(m * numberDof + dof1, n * numberDof + dof2);

								res.Equal(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2], aux);
							}
						}
					}
				}
			}
			
			return res;
		}
		Sparse ProblemStructural::Damping() const
		{
			const auto& problemStructural = std::make_shared<ProblemStructural>(*this);
			const auto& elements = mesh_->GetElements();
			const auto& nodeMeshIndices = GetNodeMeshIndices();

			Sparse res(totalDof_, totalDof_);
			Matrix local;

			for (ElementIndex i = 0; i < elements.size(); ++i)
			{
				auto numberNodes = elements[i]->GetNumberNodes();
				auto numberDof = elements[i]->GetNumberDof();

				if (elements[i]->IsMapped())
				{
					continue;
				}
				else if (elements[i]->GetType() == element::element_combined)
				{
					std::dynamic_pointer_cast<element::ElementCombined>(elements[i])->Damping(local);
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
								auto aux = res.GetValue(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2]);
								aux += local.GetValue(m * numberDof + dof1, n * numberDof + dof2);

								res.Equal(nodeMeshIndices[i][m].dofIndices[dof1], nodeMeshIndices[i][n].dofIndices[dof2], aux);
							}
						}
					}
				}
			}

			return res;
		}
		Vector ProblemStructural::LoadDistributedEdge() const
		{
			auto loadDistributedEdgeWeak = weakform::CreateWeakFormLoadDistributedEdgeStructural();
			auto problemStructural = std::make_shared<ProblemStructural>(*this);
			auto res = IntegralForm(loadDistributedEdgeWeak, problemStructural, loads_);

			return res;
		}
		Vector ProblemStructural::LoadDistributedFace() const
		{
			auto loadDistributedFaceWeak = weakform::CreateWeakFormLoadDistributedFaceStructural();
			auto problemStructural = std::make_shared<ProblemStructural>(*this);
			auto res = IntegralForm(loadDistributedFaceWeak, problemStructural, loads_);

			return res;
		}
		Vector ProblemStructural::LoadDistributedVolume() const
		{
			auto loadDistributedVolumeWeak = weakform::CreateWeakFormLoadDistributedVolumeStructural();
			auto problemStructural = std::make_shared<ProblemStructural>(*this);
			auto res = IntegralForm(loadDistributedVolumeWeak, problemStructural, loads_);

			return res;
		}
		Vector ProblemStructural::LoadNode() const
		{
			auto problemStructural = std::make_shared<ProblemStructural>(*this);
			auto res = IntegralForm(nullptr, problemStructural, loads_);

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
	} // namespace problem
} // namespace dive