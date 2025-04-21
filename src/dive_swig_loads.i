%inline
%{
	#include "dive_loads.hpp"
	#include "dive_loads_dirichlet.hpp"
	#include "dive_loads_distributed_volume.hpp"
	#include "dive_loads_distributed_face.hpp"
	#include "dive_loads_distributed_edge.hpp"
	#include "dive_loads_node.hpp"

	typedef std::shared_ptr<dive::loads::IDirichlet> IDirichletPtr;
	typedef std::shared_ptr<const dive::loads::IDirichlet> ConstIDirichletPtr;

	typedef std::shared_ptr<dive::loads::ILoad> ILoadPtr;
	typedef std::shared_ptr<const dive::loads::ILoad> ConstILoadPtr;
	
	typedef std::shared_ptr<dive::loads::ILoadDistributedVolume> ILoadDistributedVolumePtr;
	typedef std::shared_ptr<const dive::loads::ILoadDistributedVolume> ConstILoadDistributedVolumePtr;

	typedef std::shared_ptr<dive::loads::ILoadDistributedFace> ILoadDistributedFacePtr;
	typedef std::shared_ptr<const dive::loads::ILoadDistributedFace> ConstILoadDistributedFacePtr;

	typedef std::shared_ptr<dive::loads::ILoadDistributedEdge> ILoadDistributedEdgePtr;
	typedef std::shared_ptr<const dive::loads::ILoadDistributedEdge> ConstILoadDistributedEdgePtr;

	typedef std::shared_ptr<dive::loads::ILoadNode> ILoadNodePtr;
	typedef std::shared_ptr<const dive::loads::ILoadNode> ConstILoadNodePtr;

	typedef std::shared_ptr<dive::loads::Dirichlet> DirichletPtr;
	typedef std::shared_ptr<const dive::loads::Dirichlet> ConstDirichletPtr;	
	
	typedef std::shared_ptr<dive::loads::LoadDistributedVolume> LoadDistributedVolumePtr;
	typedef std::shared_ptr<const dive::loads::LoadDistributedVolume> ConstLoadDistributedVolumePtr;	
	
	typedef std::shared_ptr<dive::loads::LoadDistributedFace> LoadDistributedFacePtr;
	typedef std::shared_ptr<const dive::loads::LoadDistributedFace> ConstLoadDistributedFacePtr;

	typedef std::shared_ptr<dive::loads::LoadDistributedEdge> LoadDistributedEdgePtr;
	typedef std::shared_ptr<const dive::loads::LoadDistributedEdge> ConstLoadDistributedEdgePtr;
	
	typedef std::shared_ptr<dive::loads::LoadNode> LoadNodePtr;
	typedef std::shared_ptr<const dive::loads::LoadNode> ConstLoadNodePtr;	
		
	typedef std::vector<dive::loads::IDirichletPtr> Dirichlets;
	typedef std::vector<dive::loads::ILoadPtr> Loads;	

%}

%shared_ptr(dive::loads::IDirichlet);
%shared_ptr(dive::loads::ILoad);
%shared_ptr(dive::loads::ILoadDistributedVolume);
%shared_ptr(dive::loads::ILoadDistributedFace);
%shared_ptr(dive::loads::ILoadDistributedEdge);
%shared_ptr(dive::loads::ILoadNode);

%shared_ptr(dive::loads::Dirichlet);
%shared_ptr(dive::loads::LoadDistributedVolume);
%shared_ptr(dive::loads::LoadDistributedFace);
%shared_ptr(dive::loads::LoadDistributedEdge);
%shared_ptr(dive::loads::LoadNode);

%template(vecDirichlets) std::vector<std::shared_ptr<dive::loads::IDirichlet>>;
%template(vecLoads) std::vector<std::shared_ptr<dive::loads::ILoad>>;

%include "dive_loads.hpp"
%include "dive_loads_dirichlet.hpp"
%include "dive_loads_distributed_volume.hpp"
%include "dive_loads_distributed_face.hpp"
%include "dive_loads_distributed_edge.hpp"
%include "dive_loads_node.hpp"