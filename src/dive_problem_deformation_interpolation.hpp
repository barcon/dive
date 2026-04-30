#ifndef DIVE_PROBLEM_DEFORMATION_INTERPOLATION_HPP_
#define DIVE_PROBLEM_DEFORMATION_INTERPOLATION_HPP_

#include "dive_problem.hpp"
#include "interpolation.hpp"

namespace dive
{
	namespace problem
	{
		ProblemDeformationInterpolationPtr CreateProblemDeformationInterpolation(Tag problemTag, IMeshPtr mesh);

		class ProblemDeformationInterpolation : public IDeformation, virtual public std::enable_shared_from_this<ProblemDeformationInterpolation>
		{
		public:
			virtual ~ProblemDeformationInterpolation() = default;

			static ProblemDeformationInterpolationPtr Create(Tag problemTag, IMeshPtr mesh);
			ProblemDeformationInterpolationPtr GetPtr();
			ConstProblemDeformationInterpolationPtr GetPtr() const;

			NumberDof GetNumberDof() const override;
			IMeshPtr GetMesh() const override;
			Type GetType() const override;
			Tag	GetTag() const override;

			Loads& GetLoads() override;

			void SetTag(Tag tag) override;

			void DeformMesh(IMeshPtr mesh) override;
			void ApplyLoad(ILoadPtr load) override;
			void Initialize(IInterpolationPtr interpolation);

		protected:
			ProblemDeformationInterpolation() = default;

			void SetMesh(IMeshPtr mesh) override;

			Tag tag_{ 0 };
			Type type_{ problem_deformation };
			NumberDof numberDof_{ 3 };
			IInterpolationPtr interpolation_{ nullptr };

			IMeshPtr mesh_{ nullptr };

			Loads loads_;
		};

	} //namespace problem
} //namespace dive

#endif /* DIVE_PROBLEM_DEFORMATION_INTERPOLATION_HPP_*/