#ifndef DIVE_PROBLEMS_BASIC_HPP_
#define DIVE_PROBLEMS_BASIC_HPP_

#include "dive_problems.hpp"

namespace dive
{
	namespace problems
	{
		Sparse IntegralForm(IWeakFormElementPtr weakForm, IProblemPtr problem1, IProblemPtr problem2);
		Vector IntegralForm(IWeakFormLoadPtr weakForm, IProblemPtr problem1, const Loads& loads);

		void UpdateMeshElements(IMeshPtr mesh, NumberDof numberDof);
		void UpdateDofMeshIndices(IMeshPtr mesh, NumberDof& totalDof, DofMeshIndices& dofMeshIndices);
		void UpdateNodeMeshIndices(IMeshPtr mesh, const DofMeshIndices& dofMeshIndices, NodeMeshIndices& nodeMeshIndices);
		void UpdateDirichletIndices(Dirichlets& dirichlets, DofIndex& pivot, DofMeshIndices& dofMeshIndices, DirichletMeshIndices& dirichletMeshIndices);
		void UpdateDirichletLoads(const DofMeshIndices& dofMeshIndices);
		void Reorder(IMeshPtr mesh, NumberDof numberDof, NodeMeshIndices& nodeMeshIndices, DofMeshIndices& dofMeshIndices);
		void SortDofMeshIndices(DofMeshIndices& dofMeshIndices);
	} //namespace problems
} //namespace dive

#endif // DIVE_PROBLEMS_BASIC_HPP_