#include "dive_problem_pressure.hpp"
#include "dive_value_scalar_congruent.hpp"

#include "dive_weakform.hpp"
#include "dive_weakform_pressure_mass.hpp"
#include "dive_weakform_pressure_stiffness.hpp"
#include "dive_weakform_pressure_crossed.hpp"
#include "dive_weakform_pressure_stabilization.hpp"
#include "dive_weakform_pressure_load_distributed_face_flow.hpp"
#include "dive_weakform_pressure_load_distributed_volume_divergence.hpp"

namespace dive {
	namespace problem {
		ProblemPressurePtr CreateProblemPressure(Tag problemTag, IMeshPtr mesh)
		{
			return ProblemPressure::Create(problemTag, mesh);
		}
		ProblemPressurePtr ProblemPressure::Create(Tag problemTag, IMeshPtr mesh)
		{
			class MakeSharedEnabler : public ProblemPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			res->SetTag(problemTag);
			res->SetMesh(mesh);

			return res;
		}
		ProblemPressurePtr ProblemPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemPressure>(shared_from_this());
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
		IScalarCoordinatesPtr ProblemPressure::GetTemperature() const
		{
			return temperature_;
		}
		IScalarCoordinatesPtr ProblemPressure::GetPressure() const
		{
			return pressure_;
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
		void ProblemPressure::SetTemperature(IScalarCoordinatesPtr temperature)
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
		void ProblemPressure::SetMesh(IMeshPtr mesh)
		{
			if (mesh == nullptr)
			{
				throw std::invalid_argument("Mesh cannot be null.");
			}

			mesh_ = mesh;
			pressure_ = value::CreateValueScalarCoordinatesCongruent(mesh_);
			
			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemPressure::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemPressure::ApplyLoad(String key, const Loads& loads)
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

				if (load->GetWeakForm() == nullptr)
				{
					if (load->GetType() == load::load_node || load->GetType() == load::load_dirichlet)
					{
						continue;
					}

					throw std::invalid_argument("Weak form cannot be null.");
				}
			}

			loadsMap_.insert({ key, loads });
		}
		void ProblemPressure::Initialize()
		{
			Dirichlets dirichlets;

			for (const auto& it : loadsMap_)
			{
				for (const auto& load : it.second)
				{
					if (load->GetType() == load::load_dirichlet)
					{
						dirichlets.push_back(std::static_pointer_cast<load::ILoadDirichlet>(load));
					}
				}
			}

			UpdateDofMeshIndices(mesh_, totalDof_, dofMeshIndices_);
			UpdateNodeMeshIndices(mesh_, dofMeshIndices_, nodeMeshIndices_);
			UpdateDirichletIndices(dirichlets, pivot_, dofMeshIndices_, dirichletMeshIndices_);
			UpdateDirichletLoads(dofMeshIndices_);

			Reorder(mesh_, numberDof_, nodeMeshIndices_, dofMeshIndices_);
		}
		void ProblemPressure::UpdateMeshValues(const Vector& p)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, p(globalIndex));
			}
		}
		void ProblemPressure::UpdateMeshValues(const Vector& p0, const Vector& p1)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				if (globalIndex < pivot_)
				{
					dofMeshIndices_[i].node->SetValue(dofIndex, p0(globalIndex));
				}
				else
				{
					dofMeshIndices_[i].node->SetValue(dofIndex, p1(globalIndex - pivot_));

				}
			}
		}
		Sparse ProblemPressure::Mass(IFluidPtr problemMomentum, bool lumped) const
		{
			auto massWeak = weakform::CreateMassPressure();
			
			massWeak->SetTemperature(temperature_);
			massWeak->SetPressure(pressure_);
			massWeak->SetMomentum(problemMomentum->GetMomentum());

			auto problemPressure = std::make_shared<ProblemPressure>(*this);
			auto res = IntegralForm(massWeak, problemPressure, problemPressure);

			if (lumped)
			{
				auto trace = res.Trace();
				auto mass = res.Sum();

				res = (mass / trace) * res.Diagonal();
			}

			return res;
		}
		Sparse ProblemPressure::Stiffness() const
		{
			auto stiffnessWeak = weakform::CreateStiffnessPressure();
			auto problemPressure = std::make_shared<ProblemPressure>(*this);
			auto res = IntegralForm(stiffnessWeak, problemPressure, problemPressure);

			return res;
		}
		Sparse ProblemPressure::Crossed(IProblemPtr problemMomentum) const
		{
			auto crossedWeak = weakform::CreateCrossedPressure();
			crossedWeak->SetProblemMomentum(problemMomentum);

			auto problemPressure = std::make_shared<ProblemPressure>(*this);
			auto res = IntegralForm(crossedWeak, problemPressure, problemMomentum);

			return res;
		}		
		Sparse ProblemPressure::Stabilization(IProblemPtr problemMomentum) const
		{
			auto stabilizationWeak = weakform::CreateStabilizationPressure();
			stabilizationWeak->SetProblemMomentum(problemMomentum);

			auto problemPressure = std::make_shared<ProblemPressure>(*this);
			auto res = IntegralForm(stabilizationWeak, problemPressure, problemMomentum);

			return res;
		}
		Vector ProblemPressure::Load(String key) const
		{
			auto it = loadsMap_.find(key);
			if (it != loadsMap_.end())
			{
				return IntegralForm(std::make_shared<ProblemPressure>(*this), it->second);
			}

			throw std::invalid_argument("Load with the specified key does not exist.");
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
	} // namespace problem
} // namespace dive