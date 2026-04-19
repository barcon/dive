#include "dive_problem_deformation_elastic.hpp"

namespace dive {
	namespace problem {

		CallbackIterative callbackIterative = [](Index iteration, Scalar residual) -> long long int
			{
				dive::Scalar tolerance{ 1e-2 };

				if (std::isnan(residual))
				{
					return eilig::EILIG_NOT_CONVERGED;
				}

				if (residual < tolerance)
				{
					return eilig::EILIG_SUCCESS;
				}

				logger::Info(headerDive, utils::string::Format("Iteration: {}, Residual: {}", iteration, residual));

				return eilig::EILIG_CONTINUE;
			};

		ProblemDeformationElasticPtr CreateProblemDeformationElastic(Tag problemTag, IMeshPtr mesh)
		{
			return ProblemDeformationElastic::Create(problemTag, mesh);
		}
		ProblemDeformationElasticPtr ProblemDeformationElastic::Create(Tag problemTag, IMeshPtr mesh)
		{
			class MakeSharedEnabler : public ProblemDeformationElastic
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			res->SetTag(problemTag);
			res->SetMesh(mesh);

			return res;
		}
		ProblemDeformationElasticPtr ProblemDeformationElastic::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemDeformationElastic>(shared_from_this());
		}
		ConstProblemDeformationElasticPtr ProblemDeformationElastic::GetPtr() const
		{
			return const_cast<ProblemDeformationElastic*>(this)->GetPtr();
		}
		NumberDof ProblemDeformationElastic::GetNumberDof() const
		{
			return numberDof_;
		}
		IMeshPtr ProblemDeformationElastic::GetMesh() const
		{
			return problemStructural_->GetMesh();
		}
		Type ProblemDeformationElastic::GetType() const
		{
			return type_;
		}
		Tag ProblemDeformationElastic::GetTag() const
		{
			return tag_;
		}
		const Loads& ProblemDeformationElastic::GetLoads() const
		{
			return loads_;
		}
		void ProblemDeformationElastic::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemDeformationElastic::SetMesh(IMeshPtr mesh)
		{
			if (mesh == nullptr)
			{
				throw std::invalid_argument("Mesh cannot be null.");
			}

			problemStructural_ = CreateProblemStructural(tag_, mesh);
			problemStructural_->SetTemperature(values::CreateValueScalarCoordinates(3, 1.0));
			problemStructural_->SetPressure(values::CreateValueScalarCoordinates(3, 1.0));
		}
		void ProblemDeformationElastic::DeformMesh(IMeshPtr mesh)
		{
			const auto& nodes = mesh->GetNodes();
			const auto& numberCoordinates = mesh->GetNumberCoordinates();

			for (auto& node : nodes)
			{
				const auto& point = node->GetPoint();

				//auto offset = Vector(interpolation_->GetValue(point), 0);

				//node->SetPoint(point + offset);
			}
		}
		void ProblemDeformationElastic::ApplyLoad(ILoadPtr load)
		{
			if(load == nullptr)
			{
				throw std::invalid_argument("Load cannot be null.");
			}

			if (load->GetType() != load::load_dirichlet)
			{
				throw std::invalid_argument("Load type is not supported for deformation problem.");
			}

			auto loadDirichlet = std::dynamic_pointer_cast<load::ILoadDirichlet>(load);

			if(loadDirichlet->GetNode()->GetNumberCoordinates() != problemStructural_->GetMesh()->GetNumberCoordinates())
			{
				throw std::invalid_argument("Load node number of coordinates does not match basis number of coordinates.");
			}

			if (loadDirichlet->GetNode()->GetNumberDof() != numberDof_)
			{
				throw std::invalid_argument("Load node number of degrees of freedom does not match problem number of degrees of freedom.");
			}

			problemStructural_->ApplyLoad(load);
		}
		void ProblemDeformationElastic::Initialize(Scalar shape)
		{
			Nodes nodes;

			for(auto& load: loads_)
			{
				auto loadDirichlet = std::dynamic_pointer_cast<load::ILoadDirichlet>(load);
				auto node = loadDirichlet->GetNode();
				auto value = loadDirichlet->GetValue();

				node->SetValue(loadDirichlet->GetDofIndex(), value);
				nodes.push_back(node);
			}

			const auto& elements = problemStructural_->GetMesh()->GetElements();

			for (auto& element : elements)
			{
				auto material = material::CreateMaterialSolid(element->GetTag());
				
				material->SetElasticModulus(values::CreateValueScalarCoordinates(2, 1.0));
				material->SetPoissonRatio(values::CreateValueScalarCoordinates(2, 0.1));
				element->SetMaterial(material);
			}

			problemStructural_->SetTemperature(values::CreateValueScalarCoordinates(3, 1.0));
			problemStructural_->SetPressure(values::CreateValueScalarCoordinates(3, 1.0));
			problemStructural_->Initialize();

			auto totalDof = problemStructural_->GetTotalDof();
			auto pivot = problemStructural_->GetPivot();
			
			auto K = Partition(problemStructural_->Stiffness(), totalDof, pivot);
			auto y = Partition(problemStructural_->Displacement(), totalDof, pivot);
			//auto status = eilig::IterativeBiCGStab(K[3], y[1], -K[2] * y[0], callbackIterative);

			//interpolation_ = interpolation;
			//interpolation_->SetNodes(nodes);
		}
	} // namespace problem
} // namespace dive