%inline
%{
	#include "dive_loads.hpp"
	#include "dive_loads_boundary_condition.hpp"
	#include "dive_loads_distributed_volume.hpp"
	#include "dive_loads_distributed_face.hpp"
	#include "dive_loads_distributed_edge.hpp"
	#include "dive_loads_node.hpp"
	#include "dive_loads_node_transient.hpp"

	typedef std::shared_ptr<dive::loads::IBoundaryCondition> IBoundaryConditionPtr;
	typedef std::shared_ptr<const dive::loads::IBoundaryCondition> ConstIBoundaryConditionPtr;

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
	
		typedef std::shared_ptr<dive::loads::ILoadNodeTransient> ILoadNodeTransientPtr;
	typedef std::shared_ptr<const dive::loads::ILoadNodeTransient> ConstILoadNodeTransientPtr;

	typedef std::shared_ptr<dive::loads::BoundaryCondition> BoundaryConditionPtr;
	typedef std::shared_ptr<const dive::loads::BoundaryCondition> ConstBoundaryConditionPtr;	
	
	typedef std::shared_ptr<dive::loads::LoadDistributedVolume> LoadDistributedVolumePtr;
	typedef std::shared_ptr<const dive::loads::LoadDistributedVolume> ConstLoadDistributedVolumePtr;	
	
	typedef std::shared_ptr<dive::loads::LoadDistributedFace> LoadDistributedFacePtr;
	typedef std::shared_ptr<const dive::loads::LoadDistributedFace> ConstLoadDistributedFacePtr;

	typedef std::shared_ptr<dive::loads::LoadDistributedEdge> LoadDistributedEdgePtr;
	typedef std::shared_ptr<const dive::loads::LoadDistributedEdge> ConstLoadDistributedEdgePtr;
	
	typedef std::shared_ptr<dive::loads::LoadNode> LoadNodePtr;
	typedef std::shared_ptr<const dive::loads::LoadNode> ConstLoadNodePtr;	
	
	typedef std::shared_ptr<dive::loads::LoadNodeTransient> LoadNodeTransientPtr;
	typedef std::shared_ptr<const dive::loads::LoadNodeTransient> ConstLoadNodeTransientPtr;	
		
	typedef std::vector<dive::loads::IBoundaryConditionPtr> BoundaryConditions;
	typedef std::vector<dive::loads::ILoadPtr> Loads;
%}

%shared_ptr(dive::loads::IBoundaryCondition);
%shared_ptr(dive::loads::ILoad);
%shared_ptr(dive::loads::ILoadDistributedVolume);
%shared_ptr(dive::loads::ILoadDistributedFace);
%shared_ptr(dive::loads::ILoadDistributedEdge);
%shared_ptr(dive::loads::ILoadNode);
%shared_ptr(dive::loads::ILoadNodeTransient);

%shared_ptr(dive::loads::BoundaryCondition);
%shared_ptr(dive::loads::LoadDistributedVolume);
%shared_ptr(dive::loads::LoadDistributedFace);
%shared_ptr(dive::loads::LoadDistributedEdge);
%shared_ptr(dive::loads::LoadNode);
%shared_ptr(dive::loads::LoadNodeTransient);

%template(vecBoundaryConditions) std::vector<std::shared_ptr<dive::loads::IBoundaryCondition>>;
%template(vecLoads) std::vector<std::shared_ptr<dive::loads::ILoad>>;

%include "dive_loads.hpp"
%include "dive_loads_boundary_condition.hpp"
%include "dive_loads_distributed_volume.hpp"
%include "dive_loads_distributed_face.hpp"
%include "dive_loads_distributed_edge.hpp"
%include "dive_loads_node.hpp"
%include "dive_loads_node_transient.hpp"