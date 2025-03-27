%inline
%{
	#include "dive_loads.hpp"
	#include "dive_loads_dirichlet.hpp"
	#include "dive_loads_distributed_volume.hpp"
	#include "dive_loads_distributed_face.hpp"

	typedef std::shared_ptr<dive::loads::IDirichlet> IDirichletPtr;
	typedef std::shared_ptr<const dive::loads::IDirichlet> ConstIDirichletPtr;

	typedef std::shared_ptr<dive::loads::Dirichlet> DirichletPtr;
	typedef std::shared_ptr<const dive::loads::Dirichlet> ConstDirichletPtr;	
	
	typedef std::shared_ptr<dive::loads::ILoad> ILoadPtr;
	typedef std::shared_ptr<const dive::loads::ILoad> ConstILoadPtr;

	typedef std::shared_ptr<dive::loads::LoadDistributedVolume> LoadDistributedVolumePtr;
	typedef std::shared_ptr<const dive::loads::LoadDistributedVolume> ConstLoadDistributedVolumePtr;	
	
	typedef std::shared_ptr<dive::loads::LoadDistributedFace> LoadDistributedFacePtr;
	typedef std::shared_ptr<const dive::loads::LoadDistributedFace> ConstLoadDistributedFacePtr;
	
	
	typedef std::vector<dive::loads::IDirichletPtr> Dirichlets;
	typedef std::vector<dive::loads::ILoadPtr> Loads;	

%}

%shared_ptr(dive::loads::IDirichlet);
%shared_ptr(dive::loads::Dirichlet);

%shared_ptr(dive::loads::ILoad);
%shared_ptr(dive::loads::LoadDistributedVolume);
%shared_ptr(dive::loads::LoadDistributedFace);

%template(vecDirichlets) std::vector<std::shared_ptr<dive::loads::IDirichlet>>;
%template(vecLoads) std::vector<std::shared_ptr<dive::loads::ILoad>>;

%include "dive_loads.hpp"
%include "dive_loads_dirichlet.hpp"
%include "dive_loads_distributed_volume.hpp"
%include "dive_loads_distributed_face.hpp"

%pythoncode
%{

#import ctypes

#py_cfunction_loads1 = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
#py_cfunction_loads2 = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_double)
#py_cfunction_loads3 = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double)
#py_cfunction_loads4 = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double)

#def CreateDirichletByFunction(node, dofIndex, function):
#	f = py_cfunction_type1(function)
#	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
#	return _dive.CreateDirichletByFunction(node, dofIndex, f_ptr)

#def CreateLoadDistributedVolume(element, dofIndex, function):
#	f = py_cfunction_type4(function)
#	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
#	return _dive.CreateLoadDistributedVolume(element, dofIndex, f_ptr)
%}