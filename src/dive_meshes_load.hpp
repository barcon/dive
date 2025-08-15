#ifndef DIVE_MESHES_LOAD_HPP_
#define DIVE_MESHES_LOAD_HPP_

#include "dive_meshes_basic.hpp"

namespace dive
{
	namespace meshes
	{
		IMeshPtr LoadInp(Tag meshTag, String fileName, NumberDof numberDof, Status& status);
	} //namespace meshes
} //namespace dive

#endif /* DIVE_MESHES_LOAD_HPP_ */