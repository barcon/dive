#include "dive_problem_thermal.hpp"
#include "dive_value_scalar_congruent.hpp"

#include "dive_weakform.hpp"
#include "dive_weakform_thermal_mass.hpp"
#include "dive_weakform_thermal_stiffness.hpp"
#include "dive_weakform_thermal_convection.hpp"
#include "dive_weakform_thermal_stabilization.hpp"
//#include "dive_weakform_thermal_load_distributed_volume.hpp"
//#include "dive_weakform_thermal_load_distributed_volume_stabilization.hpp"

namespace dive {
	namespace problem {
		ProblemThermalPtr CreateProblemThermal(Tag problemTag, IMeshPtr mesh)
		{
			return ProblemThermal::Create(problemTag, mesh);
		}
		ProblemThermalPtr ProblemThermal::Create(Tag problemTag, IMeshPtr mesh)
		{
			class MakeSharedEnabler : public ProblemThermal
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			res->SetTag(problemTag);
			res->SetMesh(mesh);

			return res;
		}
		ProblemThermalPtr ProblemThermal::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemThermal>(shared_from_this());
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
		IScalarCoordinatesPtr ProblemThermal::GetTemperature() const
		{
			return temperature_;
		}
		IScalarCoordinatesPtr ProblemThermal::GetPressure() const
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
		void ProblemThermal::SetPressure(IScalarCoordinatesPtr pressure)
		{
			if (pressure == nullptr)
			{
				throw std::invalid_argument("Pressure cannot be null.");
			}

			if (pressure->GetNumberCoordinates() != mesh_->GetNumberCoordinates())
			{
				throw std::invalid_argument("Pressure number of coordinates must match mesh number of coordinates.");
			}

			pressure_ = pressure;
		}
		void ProblemThermal::SetMesh(IMeshPtr mesh)
		{
			if (mesh == nullptr)
			{
				throw std::invalid_argument("Mesh cannot be null.");
			}

			mesh_ = mesh;
			temperature_ = value::CreateValueScalarCoordinatesCongruent(mesh_);

			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemThermal::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemThermal::ApplyLoad(String key, const Loads& loads)
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
		void ProblemThermal::Initialize()
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
		void ProblemThermal::UpdateMeshValues(const Vector& t)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, t(globalIndex));
			}
		}
		void ProblemThermal::UpdateMeshValues(const Vector& t0, const Vector& t1)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				if (globalIndex < pivot_)
				{
					dofMeshIndices_[i].node->SetValue(dofIndex, t0(globalIndex));
				}
				else
				{
					dofMeshIndices_[i].node->SetValue(dofIndex, t1(globalIndex - pivot_));

				}
			}
		}
		Sparse ProblemThermal::Mass(bool lumped) const
		{
			auto massWeak = weakform::CreateMassThermal();
			massWeak->SetTemperature(temperature_);
			massWeak->SetPressure(pressure_);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);
			auto res = IntegralForm(massWeak, problemThermal, problemThermal);

			if (lumped)
			{
				auto trace = res.Trace();
				auto mass = res.Sum();

				res = (mass / trace) * res.Diagonal();
			}

			return res;
		}
		Sparse ProblemThermal::Stiffness() const
		{
			auto stiffnessWeak = weakform::CreateStiffnessThermal();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);
			auto res = IntegralForm(stiffnessWeak, problemThermal, problemThermal);

			return res;
		}
		Sparse ProblemThermal::Convection(IProblemPtr problemMomentum) const
		{
			auto convectionWeak = weakform::CreateConvectionThermal();
			convectionWeak->SetTemperature(temperature_);
			convectionWeak->SetPressure(pressure_);
			convectionWeak->SetProblemMomentum(problemMomentum);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);
			auto res = IntegralForm(convectionWeak, problemThermal, problemThermal);

			return res;
		}
		Sparse ProblemThermal::Stabilization(IProblemPtr problemMomentum) const
		{
			auto stabilizationWeak = weakform::CreateStabilizationThermal();
			stabilizationWeak->SetTemperature(temperature_);
			stabilizationWeak->SetPressure(pressure_);
			stabilizationWeak->SetProblemMomentum(problemMomentum);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);
			auto res = IntegralForm(stabilizationWeak, problemThermal, problemThermal);

			return res;
		}
		Vector ProblemThermal::Load(String key) const
		{
			auto it = loadsMap_.find(key);
			if (it != loadsMap_.end())
			{
				return IntegralForm(std::make_shared<ProblemThermal>(*this), it->second);
			}

			throw std::invalid_argument("Load with the specified key does not exist.");
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
	} // namespace problem
} // namespace dive