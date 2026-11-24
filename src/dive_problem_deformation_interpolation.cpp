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

			Vectors points;

			std::transform(nodes.begin(), nodes.end(), std::back_inserter(points), [](INodePtr node) -> Vector {
				return node->GetPoint();
				});

			auto offset = interpolation_->GetValue(points);

			for (NodeIndex i = 0; i < nodes.size(); ++i)
			{
				nodes[i]->SetPoint(points[i] + Vector(offset[i], 0));
			}
		}
		void ProblemDeformationInterpolation::ApplyLoad(String key, const Loads& loads)
		{
			auto it = loadsMap_.find(key);
			if (it != loadsMap_.end())
			{
				throw std::invalid_argument("Load with the same key already exists.");
			}

			for (auto& load : loads)
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

				if (loadDirichlet->GetNode()->GetNumberCoordinates() != mesh_->GetNumberCoordinates())
				{
					throw std::invalid_argument("Load node number of coordinates does not match basis number of coordinates.");
				}

				if (loadDirichlet->GetNode()->GetNumberDof() != numberDof_)
				{
					throw std::invalid_argument("Load node number of degrees of freedom does not match problem number of degrees of freedom.");
				}
			}

			loadsMap_.insert({ key, loads });
		}
		void ProblemDeformationInterpolation::Initialize(IInterpolationPtr interpolation)
		{
			Nodes nodes;

			if(interpolation == nullptr)
			{
				throw std::runtime_error("Interpolation is not set.");
			}

			for (const auto& it : loadsMap_)
			{
				for (const auto& load : it.second)
				{
					if (load->GetType() == load::load_dirichlet)
					{
						auto loadDirichlet = std::dynamic_pointer_cast<load::ILoadDirichlet>(load);
						auto node = loadDirichlet->GetNode();
						auto value = loadDirichlet->GetValue();

						node->SetValue(loadDirichlet->GetDofIndex(), value);
						nodes.push_back(node);
					}
				}
			}

			interpolation_ = interpolation;
			interpolation_->SetNodes(nodes);
		}
		Vector ProblemDeformationInterpolation::Load(String key) const
		{
			auto it = loadsMap_.find(key);
			if (it != loadsMap_.end())
			{
				return IntegralForm(std::make_shared<ProblemDeformationInterpolation>(*this), it->second);
			}

			throw std::invalid_argument("Load with the specified key does not exist.");
		}
	} // namespace problem
} // namespace dive