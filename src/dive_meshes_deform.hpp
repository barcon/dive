#ifndef DIVE_MESHES_DEFORM_HPP_
#define DIVE_MESHES_DEFORM_HPP_

#include "dive_types.hpp"
#include "dive_nodes.hpp"
#include "dive_meshes.hpp"

namespace dive
{
	namespace meshes
	{
		void DeformByInterpolation(MeshPtr mesh, InterpolationPtr interpolation);
	} //namespace meshes
} //namespace dive

#endif /* DIVE_MESHES_DEFORM_HPP_ */