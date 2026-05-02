#include "dive_problem_fluid.hpp"
#include "dive_value_scalar_congruent.hpp"
#include "dive_value_matrix_congruent.hpp"

#include "dive_weakform.hpp"
#include "dive_weakform_fluid_mass.hpp"
#include "dive_weakform_fluid_stiffness.hpp"
#include "dive_weakform_fluid_convection.hpp"
#include "dive_weakform_fluid_stabilization.hpp"
#include "dive_weakform_fluid_load_distributed_volume.hpp"
#include "dive_weakform_fluid_load_distributed_volume_stabilization.hpp"

namespace dive {
	namespace problem {
		ProblemFluidPtr CreateProblemFluid(Tag problemTag, IMeshPtr mesh)
		{
			return ProblemFluid::Create(problemTag, mesh);
		}
		ProblemFluidPtr ProblemFluid::Create(Tag problemTag, IMeshPtr mesh)
		{
			class MakeSharedEnabler : public ProblemFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			res->SetTag(problemTag);
			res->SetMesh(mesh);

			return res;
		}
		ProblemFluidPtr ProblemFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<ProblemFluid>(shared_from_this());
		}
		ConstProblemFluidPtr ProblemFluid::GetPtr() const
		{
			return const_cast<ProblemFluid*>(this)->GetPtr();
		}
		NumberDof ProblemFluid::GetNumberDof() const
		{
			return numberDof_;
		}
		NumberDof ProblemFluid::GetTotalDof() const
		{
			return totalDof_;
		}
		DofIndex ProblemFluid::GetPivot() const
		{
			return pivot_;
		}
		IScalarCoordinatesPtr ProblemFluid::GetTemperature() const
		{
			return temperature_;
		}
		IScalarCoordinatesPtr ProblemFluid::GetPressure() const
		{
			return pressure_;
		}
		IMatrixCoordinatesPtr ProblemFluid::GetVelocity() const
		{
			return velocity_;
		}
		IMeshPtr ProblemFluid::GetMesh() const
		{
			return mesh_;
		}
		Type ProblemFluid::GetType() const
		{
			return type_;
		}
		Tag ProblemFluid::GetTag() const
		{
			return tag_;
		}
		Loads& ProblemFluid::GetLoads()
		{
			return loads_;
		}
		const DofMeshIndices& ProblemFluid::GetDofMeshIndices() const
		{
			return dofMeshIndices_;
		}
		const NodeMeshIndices& ProblemFluid::GetNodeMeshIndices() const
		{
			return  nodeMeshIndices_;
		}
		const DirichletMeshIndices& ProblemFluid::GetDirichletMeshIndices() const
		{
			return dirichletMeshIndices_;
		}
		void ProblemFluid::SetTemperature(IScalarCoordinatesPtr temperature)
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
		void ProblemFluid::SetPressure(IScalarCoordinatesPtr pressure)
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
		void ProblemFluid::SetMesh(IMeshPtr mesh)
		{
			if (mesh == nullptr)
			{
				throw std::invalid_argument("Mesh cannot be null.");
			}
		
			mesh_ = mesh;
			velocity_ = value::CreateValueMatrixCoordinatesCongruent(mesh_);

			UpdateMeshElements(mesh_, numberDof_);
		}
		void ProblemFluid::SetTag(Tag tag)
		{
			tag_ = tag;
		}
		void ProblemFluid::ApplyLoad(ILoadPtr load)
		{
			if (load == nullptr)
			{
				throw std::invalid_argument("Load cannot be null.");
			}

			loads_.push_back(load);
		}
		void ProblemFluid::Initialize()
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
		void ProblemFluid::UpdateMeshValues(const Vector& u)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				dofMeshIndices_[i].node->SetValue(dofIndex, u(globalIndex));
			}
		}
		void ProblemFluid::UpdateMeshValues(const Vector& u0, const Vector& u1)
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
		void ProblemFluid::UpdateMeshValuesMomentum(const Vector& q)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				const auto& node = dofMeshIndices_[i].node;
				const auto& element = node->GetConnectivity().elements[0];
				const auto& point = element->LocalCoordinates(node);
				const auto& material = element->GetMaterial();

				auto state = Vector(2);
				state(0) = value::GetValueScalarCoordinates(temperature_, point, element);
				state(1) = value::GetValueScalarCoordinates(pressure_, point, element);

				auto density = material->GetDensity(state);

				dofMeshIndices_[i].node->SetValue(dofIndex, q(globalIndex) / density);
			}
		}
		void ProblemFluid::UpdateMeshValuesMomentum(const Vector& q0, const Vector& q1)
		{
			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				const auto& node = dofMeshIndices_[i].node;
				const auto& element = node->GetConnectivity().elements[0];
				const auto& point = element->LocalCoordinates(node);
				const auto& material = element->GetMaterial();

				auto state = Vector(2);
				state(0) = value::GetValueScalarCoordinates(temperature_, point, element);
				state(1) = value::GetValueScalarCoordinates(pressure_, point, element);

				auto density = material->GetDensity(state);

				if (globalIndex < pivot_)
				{
					dofMeshIndices_[i].node->SetValue(dofIndex, q0(globalIndex) / density);
				}
				else
				{
					dofMeshIndices_[i].node->SetValue(dofIndex, q1(globalIndex - pivot_) / density);

				}
			}
		}
		Sparse ProblemFluid::Mass(bool lumped) const
		{
			auto massWeak = weakform::CreateWeakFormMassFluid();
			auto problemFluid = std::make_shared<ProblemFluid>(*this);		
			auto res = IntegralForm(massWeak, problemFluid, problemFluid);

			if (lumped)
			{
				auto trace = res.Trace();
				auto mass = res.Sum();

				res = (mass / trace) * res.Diagonal();
			}

			return res;
		}
		Sparse ProblemFluid::Stiffness() const
		{
			auto stiffnessWeak = weakform::CreateWeakFormStiffnessFluid();
			stiffnessWeak->SetTemperature(temperature_);
			stiffnessWeak->SetPressure(pressure_);

			auto problemFluid = std::make_shared<ProblemFluid>(*this);
			auto res =  IntegralForm(stiffnessWeak, problemFluid, problemFluid);

			return res;
		}
		Sparse ProblemFluid::Convection() const
		{
			auto convectionWeak = weakform::CreateWeakFormConvectionFluid();
			auto problemFluid = std::make_shared<ProblemFluid>(*this);
			auto res = IntegralForm(convectionWeak, problemFluid, problemFluid);

			return res;
		}
		Sparse ProblemFluid::Stabilization() const
		{
			auto stabilizationWeak = weakform::CreateWeakFormStabilizationFluid();
			auto problemFluid = std::make_shared<ProblemFluid>(*this);
			auto res = IntegralForm(stabilizationWeak, problemFluid, problemFluid);

			return res;
		}
		Vector ProblemFluid::LoadDistributedVolume() const
		{
			auto loadDistributedVolumeWeak = weakform::CreateWeakFormLoadDistributedVolumeFluid();
			auto problemFluid = std::make_shared<ProblemFluid>(*this);
			auto res = IntegralForm(loadDistributedVolumeWeak, problemFluid, loads_);

			return res;
		}
		Vector ProblemFluid::LoadDistributedVolumeStabilization() const
		{
			auto loadDistributedVolumeStabilizationWeak = weakform::CreateWeakFormLoadDistributedVolumeStabilizationFluid();
			auto problemFluid = std::make_shared<ProblemFluid>(*this);
			auto res = IntegralForm(loadDistributedVolumeStabilizationWeak, problemFluid, loads_);

			return res;
		}
		Vector ProblemFluid::Momentum() const
		{
			Vector res(totalDof_, 0.0);
			
			const auto& elements = mesh_->GetElements();

			for (Index i = 0; i < dofMeshIndices_.size(); ++i)
			{
				auto globalIndex = dofMeshIndices_[i].globalIndex;
				auto dofIndex = dofMeshIndices_[i].dofIndex;

				const auto& element = dofMeshIndices_[i].node->GetConnectivity().elements[0];
				const auto& point = element->LocalCoordinates(dofMeshIndices_[i].node);
				
				auto material = std::static_pointer_cast<material::IMaterialFluid>(element->GetMaterial());

				auto state = Vector(2);
				state(0) = value::GetValueScalarCoordinates(temperature_, point, element);
				state(1) = value::GetValueScalarCoordinates(pressure_, point, element);

				auto density = material->GetDensity(state);

				res(globalIndex) = density * dofMeshIndices_[i].node->GetValue(dofIndex);
			}

			return res;
		}
		Vector ProblemFluid::Velocity() const
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