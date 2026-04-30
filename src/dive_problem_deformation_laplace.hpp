#ifndef DIVE_PROBLEM_DEFORMATION_LAPLACE_HPP_
#define DIVE_PROBLEM_DEFORMATION_LAPLACE_HPP_

#include "dive_problem.hpp"
#include "dive_problem_thermal.hpp"

namespace dive
{
	namespace problem
	{
		ProblemDeformationLaplacePtr CreateProblemDeformationLaplace(Tag problemTag, IMeshPtr mesh);

		class ProblemDeformationLaplace : public IDeformationLaplace, virtual public std::enable_shared_from_this<ProblemDeformationLaplace>
		{
		public:
			virtual ~ProblemDeformationLaplace() = default;

			static ProblemDeformationLaplacePtr Create(Tag problemTag, IMeshPtr mesh);
			ProblemDeformationLaplacePtr GetPtr();
			ConstProblemDeformationLaplacePtr GetPtr() const;

			NumberDof GetNumberDof() const override;
			NumberDof GetTotalDof() const override;
			DofIndex GetPivot() const override;

			IMeshPtr GetMesh() const override;
			Type GetType() const override;
			Tag	GetTag() const override;
			Loads& GetLoads() override;

			void SetTemperature(IScalarCoordinatesPtr temperature) override;
			void SetPressure(IScalarCoordinatesPtr pressure) override;
			void SetTag(Tag tag) override;

			const DofMeshIndices& GetDofMeshIndices() const override;
			const NodeMeshIndices& GetNodeMeshIndices() const override;
			const DirichletMeshIndices& GetDirichletMeshIndices() const override;

			void ApplyLoad(ILoadPtr load) override;
			void DeformMesh(IMeshPtr mesh) override;
			void Initialize();

			void UpdateMeshValues(const Vector& u) override;
			void UpdateMeshValues(const Vector& u0, const Vector& u1) override;

			Sparse Stiffness() const override;
			Vector Displacement() const override;

		protected:
			ProblemDeformationLaplace() = default;

			void SetMesh(IMeshPtr mesh) override;
			void UpdateMeshMaterial(IMeshPtr mesh);

			Tag tag_{ 0 };
			Type type_{ problem_deformation };
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

#endif /* DIVE_PROBLEM_DEFORMATION_LAPLACE_HPP_*/