#ifndef DIVE_PROBLEM_DEFORMATION_STRAIN_HPP_
#define DIVE_PROBLEM_DEFORMATION_STRAIN_HPP_

#include "dive_problem.hpp"
#include "dive_problem_thermal.hpp"

#include <limits>

namespace dive
{
	namespace problem
	{
		ProblemDeformationStrainPtr CreateProblemDeformationStrain(Tag problemTag, IMeshPtr mesh);

		class ProblemDeformationStrain : public IDeformationStrain, virtual public std::enable_shared_from_this<ProblemDeformationStrain>
		{
		public:
			virtual ~ProblemDeformationStrain() = default;

			static ProblemDeformationStrainPtr Create(Tag problemTag, IMeshPtr mesh);
			ProblemDeformationStrainPtr GetPtr();

			NumberDof GetNumberDof() const override;
			NumberDof GetTotalDof() const override;
			DofIndex GetPivot() const override;

			IMeshPtr GetMesh() const override;
			Type GetType() const override;
			Tag	GetTag() const override;
			Loads& GetLoads() override;

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
			ProblemDeformationStrain() = default;

			void SetMesh(IMeshPtr mesh) override;
			void UpdateMeshMaterial1(IMeshPtr mesh) const;
			void UpdateMeshMaterial2(IMeshPtr mesh) const;
			void SolveElasticityProblem();
			void CalculateStrainEnergy();

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

			Scalar strainEnergyMax_{ 0.0 };
			Scalar strainEnergyMin_{ 0.0 };
			Scalars strainEnergies_;
		};

	} //namespace problem
} //namespace dive

#endif /* DIVE_PROBLEM_DEFORMATION_STRAIN_HPP_*/