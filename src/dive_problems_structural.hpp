#ifndef DIVE_PROBLEMS_STRUCTURAL_HPP_
#define DIVE_PROBLEMS_STRUCTURAL_HPP_

#include "dive_problems.hpp"
#include "dive_weakforms.hpp"
#include "dive_weakforms_structural_mass.hpp"
#include "dive_weakforms_structural_stiffness.hpp"
#include "dive_weakforms_structural_load_distributed_volume.hpp"
#include "dive_weakforms_structural_load_distributed_face.hpp"
#include "dive_weakforms_structural_load_distributed_edge.hpp"

namespace dive
{
	namespace problems
	{
		ProblemStructuralPtr CreateProblemStructural();
		ProblemStructuralPtr CreateProblemStructural(Tag problemTag);

		class ProblemStructural : public IStructural, virtual public std::enable_shared_from_this<ProblemStructural>
		{
		public:
			virtual ~ProblemStructural() = default;

			static ProblemStructuralPtr Create();
			ProblemStructuralPtr GetPtr();
			ConstProblemStructuralPtr GetPtr() const;

			NumberDof GetNumberDof() const override;
			NumberDof GetTotalDof() const override;
			DofIndex GetPivot() const override;

			IScalar3DPtr GetTemperature() const override;
			IScalar3DPtr GetPressure() const override;
			IMeshPtr GetMesh() const override;
			Type GetType() const override;
			Tag	GetTag() const override;

			const BoundaryConditions& GetDirichlets() const override;
			const BoundaryConditions& GetVelocity() const override;
			const Loads& GetLoads() const override;

			const DofMeshIndices& GetDofMeshIndices() const override;
			const NodeMeshIndices& GetNodeMeshIndices() const override;
			const DirichletMeshIndices& GetDirichletMeshIndices() const override;

			void SetTemperature(IScalar3DPtr temperature) override;
			void SetPressure(IScalar3DPtr pressure) override;
			void SetMesh(IMeshPtr mesh) override;
			void SetTag(Tag tag) override;

			void AddDirichlet(IBoundaryConditionPtr dirichlet) override;
			void AddVelocity(IBoundaryConditionPtr velocity) override;
			void AddLoad(ILoadPtr load) override;
			void Initialize() override;
			void UpdateMeshValues(const Vector& u) override;

			Sparse Mass() const override;
			Sparse Stiffness() const override;
			Sparse Damping() const override;
			Vector LoadDistributedEdge() const override;
			Vector LoadDistributedFace() const override;
			Vector LoadDistributedVolume() const override;
			Vector LoadNode() const override;
			
			Vector Displacement() const override;

		protected:
			ProblemStructural();

			Tag tag_{ 0 };
			Type type_{ problem_structural };
			NumberDof numberDof_{ 3 };
			NumberDof totalDof_{ 0 };
			DofIndex pivot_{ 0 };

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };
			IMeshPtr mesh_{ nullptr };

			Loads loads_;
			BoundaryConditions dirichlets_;
			BoundaryConditions velocities_;

			DofMeshIndices dofMeshIndices_;
			NodeMeshIndices nodeMeshIndices_;
			DirichletMeshIndices dirichletMeshIndices_;
		};

	} //namespace problems
} //namespace dive

#endif /* DIVE_PROBLEMS_STRUCTURAL_HPP_*/