#ifndef DIVE_PROBLEM_DEFORMATION_ELASTIC_HPP_
#define DIVE_PROBLEM_DEFORMATION_ELASTIC_HPP_

#include "dive_problem.hpp"
#include "dive_problem_structural.hpp"

namespace dive
{
	namespace problem
	{
		ProblemDeformationElasticPtr CreateProblemDeformationElastic(Tag problemTag, IMeshPtr mesh);

		class ProblemDeformationElastic : public IDeformation, virtual public std::enable_shared_from_this<ProblemDeformationElastic>
		{
		public:
			virtual ~ProblemDeformationElastic() = default;

			static ProblemDeformationElasticPtr Create(Tag problemTag, IMeshPtr mesh);
			ProblemDeformationElasticPtr GetPtr();
			ConstProblemDeformationElasticPtr GetPtr() const;

			NumberDof GetNumberDof() const override;
			IMeshPtr GetMesh() const override;
			Type GetType() const override;
			Tag	GetTag() const override;

			const Loads& GetLoads() const override;
			void SetTag(Tag tag) override;

			void ApplyLoad(ILoadPtr load) override;
			void DeformMesh(IMeshPtr mesh) override;
			void Initialize(Scalar shape);

		protected:
			ProblemDeformationElastic() = default;

			void SetMesh(IMeshPtr mesh) override;

			Tag tag_{ 0 };
			Type type_{ problem_deformation };
			NumberDof numberDof_{ 3 };
			Scalar shape_{ 1.0e+4 };

			ProblemStructuralPtr problemStructural_{ nullptr };

			Loads loads_;
		};

	} //namespace problem
} //namespace dive

#endif /* DIVE_PROBLEM_DEFORMATION_ELASTIC_HPP_*/