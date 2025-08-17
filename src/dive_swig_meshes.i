%inline
%{

	#include "dive_meshes.hpp"
	#include "dive_meshes_basic.hpp"

	typedef std::shared_ptr<dive::meshes::IMesh> IMeshPtr;
	typedef std::shared_ptr<const dive::meshes::IMesh> ConstIMeshPtr;

	typedef std::shared_ptr<dive::meshes::Mesh> MeshPtr;
	typedef std::shared_ptr<const dive::meshes::Mesh> ConstMeshPtr;
	
	typedef std::vector<dive::meshes::IMeshPtr> Meshes;	
	
	using DofMeshIndex = dive::DofMeshIndex;
	using DofMeshIndices = dive::DofMeshIndices;

	using NodeMeshIndex = dive::NodeMeshIndex;
	using NodeMeshIndices = dive::NodeMeshIndices;
	
	using DirichletMeshIndex = dive::DirichletMeshIndex;
	using DirichletMeshIndices = dive::DirichletMeshIndices;	
	
%}

%shared_ptr(dive::meshes::IMesh);
%shared_ptr(dive::meshes::Mesh);

%template(vecMeshes) std::vector<std::shared_ptr<dive::meshes::IMesh>>;
%template(vecDofMeshIndices) std::vector<dive::DofMeshIndex>;
%template(vecNodeMeshIndices) std::vector<dive::NodeMeshIndex>;
%template(arrNodeMeshIndices) std::vector< std::vector<dive::NodeMeshIndex>>;
%template(vecDirichletMeshIndices) std::vector<dive::DirichletMeshIndex>;

%include "dive_meshes.hpp"
%include "dive_meshes_basic.hpp"

%extend std::vector<std::shared_ptr<nodes::INode>> {
    String __str__() const 
	{
         return dive::ListNodes(*self);
    }
}

%extend std::vector<std::shared_ptr<dive::elements::IElement>> {
    String __str__() const 
	{
         return dive::ListElements(*self);
    }
}

%extend std::vector<dive::DofMeshIndex> {
    String __str__() const 
	{
         return dive::ListDofMeshIndices(*self);
    }
}

%extend std::vector< std::vector<dive::NodeMeshIndex>> {
    String __str__() const 
	{
         return dive::ListNodeMeshIndices(*self);
    }
}

%extend std::vector<dive::DirichletMeshIndex> {
    String __str__() const 
	{
         return dive::ListDirichletMeshIndices(*self);
    }
}