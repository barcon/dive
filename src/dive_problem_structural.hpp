#ifndef DIVE_PROBLEM_STRUCTURAL_HPP_
#define DIVE_PROBLEM_STRUCTURAL_HPP_

#include "dive_problem.hpp"

namespace dive
{
	namespace problem
	{
		ProblemStructuralPtr CreateProblemStructural(Tag problemTag, IMeshPtr mesh);

		class ProblemStructural : public IStructural, virtual public std::enable_shared_from_this<ProblemStructural>
		{
		public:
			virtual ~ProblemStructural() = default;

			static ProblemStructuralPtr Create(Tag problemTag, IMeshPtr mesh);
			ProblemStructuralPtr GetPtr();

			NumberDof GetNumberDof() const override;
			NumberDof GetTotalDof() const override;
			DofIndex GetPivot() const override;

			IScalarCoordinatesPtr GetTemperature() const override;
			IScalarCoordinatesPtr GetPressure() const override;
			IMeshPtr GetMesh() const override;
			Type GetType() const override;
			Tag	GetTag() const override;

			Loads& GetLoads() override;

			const DofMeshIndices& GetDofMeshIndices() const override;
			const NodeMeshIndices& GetNodeMeshIndices() const override;
			const DirichletMeshIndices& GetDirichletMeshIndices() const override;

			void SetTemperature(IScalarCoordinatesPtr temperature) override;
			void SetPressure(IScalarCoordinatesPtr pressure) override;
			void SetTag(Tag tag) override;

			void ApplyLoad(ILoadPtr load) override;
			void Initialize() override;
			void UpdateMeshValues(const Vector& u) override;
			void UpdateMeshValues(const Vector& u0, const Vector& u1) override;

			Sparse Mass(bool lumped = false) const override;
			Sparse Stiffness() const override;
			Sparse Damping() const override;
			Vector LoadDistributedEdge() const override;
			Vector LoadDistributedFace() const override;
			Vector LoadDistributedVolume() const override;
			Vector LoadNode() const override;		
			Vector Displacement() const override;

		protected:
			ProblemStructural() = default;

			void SetMesh(IMeshPtr mesh) override;

			Tag tag_{ 0 };
			Type type_{ problem_structural };
			NumberDof numberDof_{ 3 };
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

#endif /* DIVE_PROBLEM_STRUCTURAL_HPP_*/