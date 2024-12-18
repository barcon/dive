#ifndef DIVE_ROUTINES_HPP_
#define DIVE_ROUTINES_HPP_

#include "dive_types.hpp"
#include "dive_elements.hpp"
#include "dive_meshes.hpp"

namespace dive
{
	String ListNodes(const Nodes& nodes);
	String ListElements(const Elements& elements);
	String ListVolumes(const Elements& elements);
	String ListDofMeshIndices(const DofMeshIndices& dofMeshIndices);
	String ListNodeMeshIndices(const NodeMeshIndices& nodeMeshIndices);
	String ListDirichletMeshIndices(const DirichletMeshIndices& dirichletMeshIndices);
}

#endif /* DIVE_ROUTINES_HPP_ */