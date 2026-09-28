#include "dive_problem_deformation_strain.hpp"

#include "dive_mesh_basic.hpp"
#include "dive_weakform_deformation_stiffness.hpp"

namespace dive {
	namespace problem {

		eilig::CallbackIterative callbackIterative = [](Index iteration, Scalar residual) -> long long int
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

		ProblemDeformationStrainPtr CreateProblemDeformationStrain(Tag problemTag, IMeshPtr mesh)
		{
			return ProblemDeformationStrain::Create(problemTag, mesh);
		}
		ProblemDeformationStrainPtr ProblemDeformationStrain::Create(Tag problemTag, IMeshPtr mesh)
		{
			class MakeSharedEnabler : public ProblemDeformationStrain
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			res->SetTag(problemTag);
			res->SetMesh(mesh);

			return res;
		}
		ProblemDeformationStrain::ProblemDeformationStrain()
		{
			temperature_ = value::CreateValueScalarCoordinates(mesh_->GetNumberCoordinates(), 1.0);
			pressure_ = value::CreateValueScalarCoordinates(mesh_->GetNumberCoordinates(), 1.0);
		}
		ProblemDeformationStrainPtr ProblemDeformationStrain::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemDeformationStrain>(shared_from_this());
		}
		ConstProblemDeformationStrainPtr ProblemDeformationStrain::GetPtr() const
		{
			return const_cast<ProblemDeformationStrain*>(this)->GetPtr();
		}
		NumberDof ProblemDeformationStrain::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberDof ProblemDeformationStrain::GetTotalDof() const
		{
			return totalDof_;
		}
		DofIndex ProblemDeformationStrain::GetPivot() const
		{
			return pivot_;
		}
		IMeshPtr ProblemDeformationStrain::GetMesh() const
		{
			return mesh_;
		}
		Type ProblemDeformationStrain::GetType() const
		{
			return type_;
		}
		Tag ProblemDeformationStrain::GetTag() const
		{
			return tag_;
		}
		Loads& ProblemDeformationStrain::GetLoads() 
		{
			return loads_;
		}
		const DofMeshIndices& ProblemDeformationStrain::GetDofMeshIndices() const
		{
			return dofMeshIndices_;
		}
		const NodeMeshIndices& ProblemDeformationStrain::GetNodeMeshIndices() const
		{
			return  nodeMeshIndices_;
		}
		const DirichletMeshIndices& ProblemDeformationStrain::GetDirichletMeshIndices() const
		{
			return dirichletMeshIndices_;
		}
		void ProblemDeformationStrain::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemDeformationStrain::SetMesh(IMeshPtr mesh)
		{
			if (mesh == nullptr)
			{
				throw std::invalid_argument("Mesh cannot be null.");
			}

			mesh_ = mesh;
			
			UpdateMeshMaterial(mesh_);
			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemDeformationStrain::UpdateMeshMaterial(IMeshPtr mesh)
		{
			const auto& elements = mesh->GetElements();
			
			for (auto& element : elements)
			{
				auto material = material::CreateSolid(element->GetTag());

				material->SetThermalConductivity(value::CreateValueScalarCoordinates(2, 1.0));
				element->SetMaterial(material);
			}
		}
		void ProblemDeformationStrain::DeformMesh(IMeshPtr mesh)
		{
			auto& nodes = mesh->GetNodes();
			
			auto K = Partition<Sparses>(Stiffness(), totalDof_, pivot_);
			auto y = Partition<Vectors>(Displacement(), totalDof_, pivot_);
			auto status = eilig::IterativeBiCGStab(K[3], y[1], -K[2] * y[0], callbackIterative);
			
			UpdateMeshValues(y[0], y[1]);

			for (NodeIndex i = 0; i < nodes.size(); i++)
			{
				auto offset = Vector(mesh_->GetNodes()[i]->GetValue(), 0);
				auto point = nodes[i]->GetPoint();

				nodes[i]->SetPoint(point + offset);
			}
		}
		void ProblemDeformationStrain::ApplyLoad(ILoadPtr load)
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
		void ProblemDeformationStrain::Initialize()
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
		void ProblemDeformationStrain::UpdateMeshValues(const Vector& u)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, u(globalIndex));
			}
		}
		void ProblemDeformationStrain::UpdateMeshValues(const Vector& u0, const Vector& u1)
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
		Sparse ProblemDeformationStrain::Stiffness() const
		{
			auto stiffnessWeak = weakform::CreateWeakFormStiffnessDeformation();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemDeformationStrain = std::make_shared<ProblemDeformationStrain>(*this);
			auto res = IntegralForm(stiffnessWeak, problemDeformationStrain, problemDeformationStrain);

			return res;
		}
		Vector ProblemDeformationStrain::Displacement() const
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