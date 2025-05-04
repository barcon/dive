%module dive

%{
	#define SWIG_PYTHON_EXTRA_NATIVE_CONTAINERS 
%}

%include "typemaps.i"
%include "cpointer.i"
%include "std_shared_ptr.i"
%include "std_string.i"
%include "std_set.i"
%include "std_vector.i"

namespace utils
{
	typedef ::String std::string;
}

namespace interpolation
{
	typedef ::Nodes std::vector<dive::nodes::INodePtr>;
}

%include "dive_swig_eilig.i"
%include "dive_swig_bases.i"
%include "dive_swig_timers.i"
%include "dive_swig_values.i"
%include "dive_swig_quadrature.i"
%include "dive_swig_interpolation.i"
%include "dive_swig_material.i"
%include "dive_swig_routines.i"
%include "dive_swig_nodes.i"
%include "dive_swig_elements.i"
%include "dive_swig_loads.i"
%include "dive_swig_meshes.i"
%include "dive_swig_weak_forms.i"
%include "dive_swig_problems.i"
%include "dive_swig_selection.i"

%inline
%{
	using Axis = eilig::Axis;
	using Scalar = eilig::Scalar;
	using Vector = eilig::Vector;
	using Matrix = eilig::Matrix;
	using Sparse = eilig::Ellpack;
	
	using Status = long long int;
	using Tag = std::size_t;
	using Type = std::size_t;

	using Dimension = std::size_t;
	using Index = std::size_t;
	using DofIndex = Index;
	using NodeIndex = Index;
	using ElementIndex = Index;
	using FaceIndex = Index;
	using EdgeIndex = Index;
	using DirichletIndex = Index;
	using DimensionIndex = Index;
	using CacheIndex = Index;

	using Number = std::size_t;
	using NumberDof = Number;
	using NumberNodes = Number;
	using NumberFaces = Number;
	using NumberEdges = Number;
	using NumberElements = Number;	
	using NumberDimensions = Number;	
	
	using Indices = std::vector<size_t>;	
	using String = std::string;
%}