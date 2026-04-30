#ifndef DIVE_MESH_GMSH_HPP_
#define DIVE_MESH_GMSH_HPP_

#include "dive_mesh_basic.hpp"

namespace dive
{
	namespace mesh
	{
		using PhysicalGroup = std::pair<int, int>;
		using PhysicalGroups = std::vector<PhysicalGroup>;

		void GmshInitialize();
		void GmshFinalize();

		PhysicalGroups GmshGetPhysicalGroups();
		PhysicalGroup GmshGetPhysicalGroupByName(const String& groupName);
		String GmshGetPhysicalName(int dim, int tag);
		Nodes GmshGetNodesForPhysicalGroup(IMeshPtr mesh, const String& groupName);
		EdgePairs GmshGetEdgesForPhysicalGroup(IMeshPtr mesh, const String& groupName);
		FacePairs GmshGetFacesForPhysicalGroup(IMeshPtr mesh, const String& groupName);
		Elements GmshGetElementsForPhysicalGroup(IMeshPtr mesh, const String& groupName);
		IMeshPtr GmshGetMeshForPhysicalGroup(Tag meshTag, NumberCoordinates numberCoordinates, const String& groupName);

	} //namespace mesh
} //namespace dive

#endif /* DIVE_MESH_GMSH_HPP_ */