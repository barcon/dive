#ifndef DIVE_PROBLEMS_THERMAL_HPP_
#define DIVE_PROBLEMS_THERMAL_HPP_

#include "dive_problems.hpp"
#include "dive_problems_basic.hpp"
#include "dive_weakforms.hpp"
#include "dive_weakforms_basic.hpp"
#include "dive_weakforms_thermal_mass.hpp"
#include "dive_weakforms_thermal_stiffness.hpp"
#include "dive_weakforms_thermal_convection.hpp"
#include "dive_weakforms_thermal_stabilization.hpp"
//#include "dive_weakforms_thermal_load_distributed_volume.hpp"
//#include "dive_weakforms_thermal_load_distributed_volume_stabilization.hpp"

namespace dive
{
	namespace problems
	{
		ProblemThermalPtr CreateProblemThermal();
		ProblemThermalPtr CreateProblemThermal(Tag problemTag);

		class ProblemThermal : public IThermal, virtual public std::enable_shared_from_this<ProblemThermal>
		{
		public:
			virtual ~ProblemThermal() = default;

			static ProblemThermalPtr Create();
			ProblemThermalPtr GetPtr();
			ConstProblemThermalPtr GetPtr() const;
			
			NumberDof GetNumberDof() const override;
			NumberDof GetTotalDof() const override;
			DofIndex GetPivot() const override;

			ITimerPtr GetTimer() const override;
			IScalar3DPtr GetTemperature() const override;
			IScalar3DPtr GetPressure() const override;
			IMatrix3DPtr GetVelocity() const override;
			IMeshPtr GetMesh() const override;
			Type GetType() const override;
			Tag	GetTag() const override;

			const Dirichlets& GetDirichlets() const override;
			const Loads& GetLoads() const override;

			const DofMeshIndices& GetDofMeshIndices() const override;
			const NodeMeshIndices& GetNodeMeshIndices() const override;
			const DirichletMeshIndices& GetDirichletMeshIndices() const override;

			void SetTimer(ITimerPtr timer) override;
			void SetPressure(IScalar3DPtr pressure) override;
			void SetVelocity(IMatrix3DPtr velocity) override;
			void SetMesh(IMeshPtr mesh) override;
			void SetTag(Tag tag) override;

			void AddDirichlet(IDirichletPtr dirichlet) override;
			void AddLoad(ILoadPtr load) override;
			void Initialize() override;
			void UpdateMeshValues(const Vector& u) override;

			Sparse Mass() const override;
			Sparse Stiffness() const override;
			Sparse Convection() const override;
			Sparse Stabilization() const override;

			Vector Energy() const override;

		protected:
			ProblemThermal();

			Tag tag_{ 0 };
			Type type_{ problem_thermal };
			NumberDof numberDof_{ 1 };
			NumberDof totalDof_{ 0 };
			DofIndex pivot_{ 0 };

			ITimerPtr timer_{ nullptr };
			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };
			IMatrix3DPtr velocity_{ nullptr };
			IMeshPtr mesh_{ nullptr };

			Loads loads_;
			Dirichlets dirichlets_;

			DofMeshIndices dofMeshIndices_;
			NodeMeshIndices nodeMeshIndices_;
			DirichletMeshIndices dirichletMeshIndices_;
		};

	} //namespace problems
} //namespace dive

#endif /* DIVE_PROBLEMS_THERMAL_HPP_*/