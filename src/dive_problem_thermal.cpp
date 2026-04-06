#include "dive_problem_thermal.hpp"
#include "dive_values_scalar_congruent.hpp"

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
		ConstProblemThermalPtr ProblemThermal::GetPtr() const
		{
			return const_cast<ProblemThermal*>(this)->GetPtr();
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
		const Loads& ProblemThermal::GetLoads() const
		{
			return loads_;
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
			temperature_ = values::CreateValueScalarCoordinatesCongruent(mesh_);

			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemThermal::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemThermal::ApplyLoad(ILoadPtr load)
		{
			loads_.push_back(load);
		}
		void ProblemThermal::Initialize()
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
		void ProblemThermal::UpdateMeshValues(const Vector& u)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, u(globalIndex));
			}
		}
		void ProblemThermal::UpdateMeshValues(const Vector& u0, const Vector& u1)
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
		Sparse ProblemThermal::Mass(bool lumped) const
		{
			auto massWeak = weakforms::CreateWeakFormMassThermal();
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
			auto stiffnessWeak = weakforms::CreateWeakFormStiffnessThermal();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);
			auto res = IntegralForm(stiffnessWeak, problemThermal, problemThermal);

			return res;
		}
		Sparse ProblemThermal::Convection(IProblemPtr problemMomentum) const
		{
			auto convectionWeak = weakforms::CreateWeakFormConvectionThermal();
			convectionWeak->SetTemperature(temperature_);
			convectionWeak->SetPressure(pressure_);
			convectionWeak->SetProblemMomentum(problemMomentum);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);
			auto res = IntegralForm(convectionWeak, problemThermal, problemThermal);

			return res;
		}
		Sparse ProblemThermal::Stabilization(IProblemPtr problemMomentum) const
		{
			auto stabilizationWeak = weakforms::CreateWeakFormStabilizationThermal();
			stabilizationWeak->SetTemperature(temperature_);
			stabilizationWeak->SetPressure(pressure_);
			stabilizationWeak->SetProblemMomentum(problemMomentum);

			auto problemThermal = std::make_shared<ProblemThermal>(*this);
			auto res = IntegralForm(stabilizationWeak, problemThermal, problemThermal);

			return res;
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