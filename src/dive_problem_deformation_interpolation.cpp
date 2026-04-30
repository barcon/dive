#include "dive_problem_deformation_interpolation.hpp"

namespace dive {
	namespace problem {
		ProblemDeformationInterpolationPtr CreateProblemDeformationInterpolation(Tag problemTag, IMeshPtr mesh)
		{
			return ProblemDeformationInterpolation::Create(problemTag, mesh);
		}
		ProblemDeformationInterpolationPtr ProblemDeformationInterpolation::Create(Tag problemTag, IMeshPtr mesh)
		{
			class MakeSharedEnabler : public ProblemDeformationInterpolation
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			res->SetTag(problemTag);
			res->SetMesh(mesh);

			return res;
		}
		ProblemDeformationInterpolationPtr ProblemDeformationInterpolation::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemDeformationInterpolation>(shared_from_this());
		}
		ConstProblemDeformationInterpolationPtr ProblemDeformationInterpolation::GetPtr() const
		{
			return const_cast<ProblemDeformationInterpolation*>(this)->GetPtr();
		}
		NumberDof ProblemDeformationInterpolation::GetNumberDof() const
		{
			return numberDof_;
		}
		IMeshPtr ProblemDeformationInterpolation::GetMesh() const
		{
			return mesh_;
		}
		Type ProblemDeformationInterpolation::GetType() const
		{
			return type_;
		}
		Tag ProblemDeformationInterpolation::GetTag() const
		{
			return tag_;
		}
		Loads& ProblemDeformationInterpolation::GetLoads()
		{
			return loads_;
		}
		void ProblemDeformationInterpolation::SetMesh(IMeshPtr mesh)
		{
			if (mesh == nullptr)
			{
				throw std::invalid_argument("Mesh cannot be null.");
			}
		
			mesh_ = mesh;

			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemDeformationInterpolation::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemDeformationInterpolation::DeformMesh(IMeshPtr mesh)
		{
			const auto& nodes = mesh->GetNodes();
			const auto& numberCoordinates = mesh->GetNumberCoordinates();

			for (auto& node : nodes)
			{
				const auto& point = node->GetPoint();

				auto offset = Vector(interpolation_->GetValue(point), 0);

				node->SetPoint(point + offset);
			}
		}
		void ProblemDeformationInterpolation::ApplyLoad(ILoadPtr load)
		{
			if (load == nullptr)
			{
				throw std::invalid_argument("Load cannot be null.");
			}

			if (load->GetType() != load::load_dirichlet)
			{
				throw std::invalid_argument("Load type is not supported for deformation problem.");
			}

			auto loadDirichlet = std::dynamic_pointer_cast<load::ILoadDirichlet>(load);

			if(loadDirichlet->GetNode()->GetNumberCoordinates() != mesh_->GetNumberCoordinates())
			{
				throw std::invalid_argument("Load node number of coordinates does not match basis number of coordinates.");
			}

			if (loadDirichlet->GetNode()->GetNumberDof() != numberDof_)
			{
				throw std::invalid_argument("Load node number of degrees of freedom does not match problem number of degrees of freedom.");
			}

			loads_.push_back(load);
		}
		void ProblemDeformationInterpolation::Initialize(IInterpolationPtr interpolation)
		{
			Nodes nodes;

			if(interpolation == nullptr)
			{
				throw std::runtime_error("Interpolation is not set.");
			}

			for(auto& load: loads_)
			{
				auto loadDirichlet = std::dynamic_pointer_cast<load::ILoadDirichlet>(load);
				auto node = loadDirichlet->GetNode();
				auto value = loadDirichlet->GetValue();

				node->SetValue(loadDirichlet->GetDofIndex(), value);
				nodes.push_back(node);
			}

			interpolation_ = interpolation;
			interpolation_->SetNodes(nodes);
		}
	} // namespace problem
} // namespace dive