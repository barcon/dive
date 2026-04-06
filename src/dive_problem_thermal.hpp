#ifndef DIVE_PROBLEM_THERMAL_HPP_
#define DIVE_PROBLEM_THERMAL_HPP_

#include "dive_problem.hpp"
#include "dive_weakforms.hpp"
#include "dive_weakforms_thermal_mass.hpp"
#include "dive_weakforms_thermal_stiffness.hpp"
#include "dive_weakforms_thermal_convection.hpp"
#include "dive_weakforms_thermal_stabilization.hpp"
//#include "dive_weakforms_thermal_load_distributed_volume.hpp"
//#include "dive_weakforms_thermal_load_distributed_volume_stabilization.hpp"

namespace dive
{
	namespace problem
	{
		ProblemThermalPtr CreateProblemThermal(Tag problemTag, IMeshPtr mesh);

		class ProblemThermal : public IThermal, virtual public std::enable_shared_from_this<ProblemThermal>
		{
		public:
			virtual ~ProblemThermal() = default;

			static ProblemThermalPtr Create(Tag problemTag, IMeshPtr mesh);
			ProblemThermalPtr GetPtr();
			ConstProblemThermalPtr GetPtr() const;
			
			NumberDof GetNumberDof() const override;
			NumberDof GetTotalDof() const override;
			DofIndex GetPivot() const override;

			IScalarCoordinatesPtr GetTemperature() const override;
			IScalarCoordinatesPtr GetPressure() const override;
			IMeshPtr GetMesh() const override;
			Type GetType() const override;
			Tag	GetTag() const override;

			const Loads& GetLoads() const override;

			const DofMeshIndices& GetDofMeshIndices() const override;
			const NodeMeshIndices& GetNodeMeshIndices() const override;
			const DirichletMeshIndices& GetDirichletMeshIndices() const override;

			void SetPressure(IScalarCoordinatesPtr pressure) override;
			void SetTag(Tag tag) override;

			void ApplyLoad(ILoadPtr load) override;
			void Initialize() override;
			void UpdateMeshValues(const Vector& u) override;
			void UpdateMeshValues(const Vector& u0, const Vector& u1) override;

			Sparse Mass(bool lumped = false) const override;
			Sparse Stiffness() const override;
			Sparse Convection(IProblemPtr problemMomentum) const override;
			Sparse Stabilization(IProblemPtr problemMomentum) const override;

			Vector Energy() const override;

		protected:
			ProblemThermal() = default;

			void SetMesh(IMeshPtr mesh) override;

			Tag tag_{ 0 };
			Type type_{ problem_thermal };
			NumberDof numberDof_{ 1 };
			NumberDof totalDof_{ 0 };
			DofIndex pivot_{ 0 };

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };
			IMeshPtr mesh_{ nullptr };

			Loads loads_;

			DofMeshIndices dofMeshIndices_;
			NodeMeshIndices nodeMeshIndices_;
			DirichletMeshIndices dirichletMeshIndices_;
		};

	} //namespace problem
} //namespace dive

#endif /* DIVE_PROBLEM_THERMAL_HPP_*/