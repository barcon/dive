#ifndef DIVE_PROBLEMS_FLUID_HPP_
#define DIVE_PROBLEMS_FLUID_HPP_

#include "dive_problems.hpp"
#include "dive_weakforms.hpp"
#include "dive_weakforms_fluid_mass.hpp"
#include "dive_weakforms_fluid_stiffness.hpp"
#include "dive_weakforms_fluid_convection.hpp"
#include "dive_weakforms_fluid_convection_symmetric.hpp"
#include "dive_weakforms_fluid_convection_asymmetric.hpp"
#include "dive_weakforms_fluid_stabilization.hpp"
#include "dive_weakforms_fluid_load_distributed_volume.hpp"
#include "dive_weakforms_fluid_load_distributed_volume_stabilization.hpp"

namespace dive
{
	namespace problems
	{
		ProblemFluidPtr CreateProblemFluid();
		ProblemFluidPtr CreateProblemFluid(Tag problemTag);

		class ProblemFluid : public IFluid, virtual public std::enable_shared_from_this<ProblemFluid>
		{
		public:
			virtual ~ProblemFluid() = default;

			static ProblemFluidPtr Create();
			ProblemFluidPtr GetPtr();
			ConstProblemFluidPtr GetPtr() const;

			NumberDof GetNumberDof() const override;
			NumberDof GetTotalDof() const override;
			DofIndex GetPivot() const override;

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

			void SetTemperature(IScalar3DPtr temperature) override;
			void SetPressure(IScalar3DPtr pressure) override;
			void SetMesh(IMeshPtr mesh) override;
			void SetTag(Tag tag) override;

			void AddLoad(ILoadPtr load) override;
			void Initialize() override;

			void UpdateMeshValues(const Vector& u) override;
			void UpdateMeshValues(const Vector& u0, const Vector& u1) override;
			void UpdateMeshValuesMomentum(const Vector& q) override;
			void UpdateMeshValuesMomentum(const Vector& q0, const Vector& q1) override;

			Sparse Mass() const override;
			Sparse Stiffness() const override;
			Sparse Convection() const override;
			Sparse ConvectionSymmetric() const override;
			Sparse ConvectionAsymmetric() const override;
			Sparse Stabilization() const override;	
			Vector LoadDistributedVolume() const override;
			Vector LoadDistributedVolumeStabilization() const override;
			
			Vector Momentum() const override;
			Vector Velocity() const override;

		protected:
			ProblemFluid();

			Tag tag_{ 0 };
			Type type_{ problem_fluid };
			NumberDof numberDof_{ 3 };
			NumberDof totalDof_{ 0 };
			DofIndex pivot_{ 0 };

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

#endif /* DIVE_PROBLEMS_FLUID_HPP_*/