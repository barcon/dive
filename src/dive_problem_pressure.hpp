#ifndef DIVE_PROBLEM_PRESSURE_HPP_
#define DIVE_PROBLEM_PRESSURE_HPP_

#include "dive_problem.hpp"
#include "dive_weakforms.hpp"
#include "dive_weakforms_pressure_mass.hpp"
#include "dive_weakforms_pressure_stiffness.hpp"
#include "dive_weakforms_pressure_crossed.hpp"
#include "dive_weakforms_pressure_stabilization.hpp"
#include "dive_weakforms_pressure_distributed_volume_divergence.hpp"

namespace dive
{
	namespace problem
	{
		ProblemPressurePtr CreateProblemPressure(Tag problemTag, IMeshPtr mesh);

		class ProblemPressure : public IPressure, virtual public std::enable_shared_from_this<ProblemPressure>
		{
		public:
			virtual ~ProblemPressure() = default;

			static ProblemPressurePtr Create(Tag problemTag, IMeshPtr mesh);
			ProblemPressurePtr GetPtr();
			ConstProblemPressurePtr GetPtr() const;

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

			void SetTemperature(IScalarCoordinatesPtr temperature) override;
			void SetTag(Tag tag) override;

			void ApplyLoad(ILoadPtr load) override;
			void Initialize() override;
			
			void UpdateMeshValues(const Vector& u) override;
			void UpdateMeshValues(const Vector& u0, const Vector& u1) override;

			Sparse Mass(IFluidPtr problemMomentum, bool lumped = false) const override;
			Sparse Stiffness() const override;
			Sparse Crossed(IProblemPtr problemMomentum) const override;
			Sparse Stabilization(IProblemPtr problemMomentum) const override;
			Sparse DistributedVolumeDivergence(IProblemPtr problemMomentum) const override;
			
			Vector Pressure() const override;

		protected:
			ProblemPressure() = default;

			void SetMesh(IMeshPtr mesh) override;

			Tag tag_{ 0 };
			Type type_{ problem_pressure };
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

#endif /* DIVE_PROBLEM_PRESSURE_HPP_*/