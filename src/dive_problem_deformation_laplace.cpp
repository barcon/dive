#include "dive_problem_deformation_laplace.hpp"

#include "dive_mesh_basic.hpp"
#include "dive_weakform_deformation_stiffness.hpp"

namespace dive {
	namespace problem {

		CallbackIterative callbackIterative = [](Index iteration, Scalar residual) -> long long int
			{
				logger::Info(headerDive, utils::string::Format("Iteration: {}, Residual: {:.3e}", iteration, residual));

				dive::Scalar tolerance{ 1e-5 };

				if (std::isnan(residual))
				{
					return eilig::EILIG_NOT_CONVERGED;
				}

				if (residual < tolerance)
				{
					return eilig::EILIG_SUCCESS;
				}

				return eilig::EILIG_CONTINUE;
			};

		ProblemDeformationLaplacePtr CreateProblemDeformationLaplace(Tag problemTag, IMeshPtr mesh)
		{
			return ProblemDeformationLaplace::Create(problemTag, mesh);
		}
		ProblemDeformationLaplacePtr ProblemDeformationLaplace::Create(Tag problemTag, IMeshPtr mesh)
		{
			class MakeSharedEnabler : public ProblemDeformationLaplace
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			res->SetTag(problemTag);
			res->SetMesh(mesh);
			res->SetTemperature(value::CreateValueScalarCoordinates(mesh->GetNumberCoordinates(), 1.0));
			res->SetPressure(value::CreateValueScalarCoordinates(mesh->GetNumberCoordinates(), 1.0));

			return res;
		}
		ProblemDeformationLaplacePtr ProblemDeformationLaplace::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemDeformationLaplace>(shared_from_this());
		}
		ConstProblemDeformationLaplacePtr ProblemDeformationLaplace::GetPtr() const
		{
			return const_cast<ProblemDeformationLaplace*>(this)->GetPtr();
		}
		NumberDof ProblemDeformationLaplace::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberDof ProblemDeformationLaplace::GetTotalDof() const
		{
			return totalDof_;
		}
		DofIndex ProblemDeformationLaplace::GetPivot() const
		{
			return pivot_;
		}
		IMeshPtr ProblemDeformationLaplace::GetMesh() const
		{
			return mesh_;
		}
		Type ProblemDeformationLaplace::GetType() const
		{
			return type_;
		}
		Tag ProblemDeformationLaplace::GetTag() const
		{
			return tag_;
		}
		Loads& ProblemDeformationLaplace::GetLoads() 
		{
			return loads_;
		}
		const DofMeshIndices& ProblemDeformationLaplace::GetDofMeshIndices() const
		{
			return dofMeshIndices_;
		}
		const NodeMeshIndices& ProblemDeformationLaplace::GetNodeMeshIndices() const
		{
			return  nodeMeshIndices_;
		}
		const DirichletMeshIndices& ProblemDeformationLaplace::GetDirichletMeshIndices() const
		{
			return dirichletMeshIndices_;
		}
		void ProblemDeformationLaplace::SetTemperature(IScalarCoordinatesPtr temperature)
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
		void ProblemDeformationLaplace::SetPressure(IScalarCoordinatesPtr pressure)
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
		void ProblemDeformationLaplace::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemDeformationLaplace::SetMesh(IMeshPtr mesh)
		{
			if (mesh == nullptr)
			{
				throw std::invalid_argument("Mesh cannot be null.");
			}

			mesh_ = mesh;
			
			UpdateMeshMaterial(mesh_);
			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemDeformationLaplace::UpdateMeshMaterial(IMeshPtr mesh)
		{
			const auto& elements = mesh->GetElements();

			for (auto& element : elements)
			{
				auto material = material::CreateSolid(element->GetTag());

				material->SetThermalConductivity(value::CreateValueScalarCoordinates(2, 1.0));
				element->SetMaterial(material);
			}
		}
		void ProblemDeformationLaplace::DeformMesh(IMeshPtr mesh)
		{
			auto K = Partition(Stiffness(), totalDof_, pivot_);
			auto y = Partition(Displacement(), totalDof_, pivot_);
			
			auto status = eilig::IterativeBiCGStab(K[3], y[1], -K[2] * y[0], callbackIterative);

			/*const auto& nodes = mesh->GetNodes();

			for (DofIndex dof = 0; dof < numberDof_; dof++)
			{
				problem_->GetLoads().clear();
				for(Index i = 0; i < loads_.size(); i++)
				{
					auto load = std::static_pointer_cast<load::ILoadDirichlet>(loads_[i]);
					
					if (load->GetDofIndex() == dof)
					{
						problem_->ApplyLoad(load);
					}

				}
				
				problem_->Initialize();

				auto totalDof = problem_->GetTotalDof();
				auto pivot = problem_->GetPivot();

				std::cout << "Total DOF: " << totalDof << ", Pivot: " << pivot << std::endl;

				auto K = Partition(problem_->Stiffness(), totalDof, pivot);
				auto y = Partition(problem_->Energy(), totalDof, pivot);
				auto status = eilig::IterativeBiCGStab(K[3], y[1], -K[2] * y[0], callbackIterative);

				problem_->UpdateMeshValues(y[0], y[1]);

				for (NodeIndex i = 0; i < nodes.size(); i++)
				{
					auto& nodeIndex = nodes[i]->GetConnectivity().nodeIndex;
					auto offset = problem_->GetMesh()->GetNodes()[nodeIndex]->GetPoint().GetValue(dof);
					auto point = nodes[i]->GetPoint();

					point(dof) = point(dof) + offset;

					nodes[i]->SetPoint(point);
				}
			}*/
		}
		void ProblemDeformationLaplace::ApplyLoad(ILoadPtr load)
		{
			if(load == nullptr)
			{
				throw std::invalid_argument("Load cannot be null.");
			}

			if (load->GetType() != load::load_dirichlet)
			{
				throw std::invalid_argument("Load type is not supported for deformation problem.");
			}

			loads_.push_back(load);
		}
		void ProblemDeformationLaplace::Initialize()
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
		void ProblemDeformationLaplace::UpdateMeshValues(const Vector& u)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, u(globalIndex));
			}
		}
		void ProblemDeformationLaplace::UpdateMeshValues(const Vector& u0, const Vector& u1)
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
		Sparse ProblemDeformationLaplace::Stiffness() const
		{
			auto stiffnessWeak = weakform::CreateWeakFormStiffnessDeformation();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemDeformationLaplace = std::make_shared<ProblemDeformationLaplace>(*this);
			auto res = IntegralForm(stiffnessWeak, problemDeformationLaplace, problemDeformationLaplace);

			return res;
		}
		Vector ProblemDeformationLaplace::Displacement() const
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