#ifndef DIVE_PROBLEMS_PRESSURE_HPP_
#define DIVE_PROBLEMS_PRESSURE_HPP_

#include "dive_problems.hpp"
#include "dive_weakforms.hpp"
#include "dive_weakforms_pressure_mass.hpp"
#include "dive_weakforms_pressure_stiffness.hpp"
#include "dive_weakforms_pressure_crossed.hpp"
#include "dive_weakforms_pressure_stabilization.hpp"
#include "dive_weakforms_pressure_distributed_volume_divergence.hpp"

namespace dive
{
	namespace problems
	{
		ProblemPressurePtr CreateProblemPressure();
		ProblemPressurePtr CreateProblemPressure(Tag problemTag);

		class ProblemPressure : public IPressure, virtual public std::enable_shared_from_this<ProblemPressure>
		{
		public:
			virtual ~ProblemPressure() = default;

			static ProblemPressurePtr Create();
			ProblemPressurePtr GetPtr();
			ConstProblemPressurePtr GetPtr() const;

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

			const BoundaryConditions& GetDirichlets() const override;
			const Loads& GetLoads() const override;

			const DofMeshIndices& GetDofMeshIndices() const override;
			const NodeMeshIndices& GetNodeMeshIndices() const override;
			const DirichletMeshIndices& GetDirichletMeshIndices() const override;

			void SetTimer(ITimerPtr timer) override;
			void SetTemperature(IScalar3DPtr temperature) override;
			void SetVelocity(IMatrix3DPtr velocity) override;
			void SetMesh(IMeshPtr mesh) override;
			void SetTag(Tag tag) override;

			void AddDirichlet(IBoundaryConditionPtr dirichlet) override;
			void AddLoad(ILoadPtr load) override;
			void Initialize() override;
			void UpdateMeshValues(const Vector& u) override;

			Sparse Mass() const override;
			Sparse Stiffness() const override;
			Sparse Crossed(IProblemPtr problemMomentum) const override;
			Sparse Stabilization(IProblemPtr problemMomentum) const override;
			Sparse DistributedVolumeDivergence(IProblemPtr problemMomentum) const override;
			
			Vector Pressure() const override;

		protected:
			ProblemPressure();

			Tag tag_{ 0 };
			Type type_{ problem_pressure };
			NumberDof numberDof_{ 1 };
			NumberDof totalDof_{ 0 };
			DofIndex pivot_{ 0 };

			ITimerPtr timer_{ nullptr };
			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };
			IMatrix3DPtr velocity_{ nullptr };
			IMeshPtr mesh_{ nullptr };

			Loads loads_;
			BoundaryConditions dirichlets_;

			DofMeshIndices dofMeshIndices_;
			NodeMeshIndices nodeMeshIndices_;
			DirichletMeshIndices dirichletMeshIndices_;
		};

	} //namespace problems
} //namespace dive

#endif /* DIVE_PROBLEMS_PRESSURE_HPP_*/