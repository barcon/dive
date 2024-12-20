%inline
%{
	#include "values_types.hpp"
	#include "values_base.hpp"
	#include "values_bool.hpp"
	#include "values_scalar.hpp"
	#include "values_scalar_function.hpp"
	#include "values_scalar_interpolation.hpp"
	#include "values_vector.hpp"
	#include "values_matrix_interpolation.hpp"
	#include "values_string.hpp"

	#include "dive_values_scalar_congruent.hpp"
	#include "dive_values_matrix_congruent.hpp"
	#include "dive_values_vector_congruent.hpp"

	typedef std::shared_ptr<values::IValue> IValuePtr;
	typedef std::shared_ptr<values::IBool> IBoolPtr;
	typedef std::shared_ptr<values::IScalar> IScalarPtr;
	typedef std::shared_ptr<values::IScalar1D> IScalar1DPtr;
	typedef std::shared_ptr<values::IScalar2D> IScalar2DPtr;
	typedef std::shared_ptr<values::IScalar3D> IScalar3DPtr;
	typedef std::shared_ptr<values::IVector3D> IVector3DPtr;
	typedef std::shared_ptr<values::IMatrix3D> IMatrix3DPtr;
	typedef std::shared_ptr<values::IString> IStringPtr;
	
	typedef std::shared_ptr<values::ValueBool> ValueBoolPtr;
	typedef std::shared_ptr<values::ValueScalar> ValueScalarPtr;
	typedef std::shared_ptr<values::ValueScalar> ValueScalarPtr;
	typedef std::shared_ptr<values::ValueScalar1D> ValueScalar1DPtr;
	typedef std::shared_ptr<values::ValueScalar1DFunction> ValueScalar1DFunctionPtr;	
	typedef std::shared_ptr<values::ValueScalar2D> ValueScalar2DPtr;
	typedef std::shared_ptr<values::ValueScalar2DFunction> ValueScalar2DFunctionPtr;	
	typedef std::shared_ptr<values::ValueScalar3D> ValueScalar3DPtr;
	typedef std::shared_ptr<values::ValueScalar3DFunction> ValueScalar3DFunctionPtr;
	typedef std::shared_ptr<values::ValueScalar3DInterpolation> ValueScalar3DInterpolationPtr;
	typedef std::shared_ptr<values::ValueScalar3DCongruent> ValueScalar3DCongruentPtr;	
	typedef std::shared_ptr<values::ValueVector3D> ValueVector3DPtr;
	typedef std::shared_ptr<values::ValueVector3DCongruent> ValueVector3DCongruentPtr;
	typedef std::shared_ptr<values::ValueMatrix3DInterpolation> ValueMatrix3DInterpolationPtr;
	typedef std::shared_ptr<values::ValueMatrix3DCongruent> ValueMatrix3DCongruentPtr;	
	typedef std::shared_ptr<values::ValueString> ValueStringPtr;
%}

%shared_ptr(values::IValue);
%shared_ptr(values::IBool);
%shared_ptr(values::IScalar);
%shared_ptr(values::IScalar1D);
%shared_ptr(values::IScalar2D);
%shared_ptr(values::IScalar3D);
%shared_ptr(values::IVector3D);
%shared_ptr(values::IMatrix3D);
%shared_ptr(values::IString);

%shared_ptr(values::ValueBool);

%shared_ptr(values::ValueScalar);
%shared_ptr(values::ValueScalar1D);
%shared_ptr(values::ValueScalar1DFunction);
%shared_ptr(values::ValueScalar2D);
%shared_ptr(values::ValueScalar2DFunction);
%shared_ptr(values::ValueScalar3D);
%shared_ptr(values::ValueScalar3DFunction);
%shared_ptr(values::ValueScalar3DInterpolation);
%shared_ptr(values::ValueScalar3DCongruent);

%shared_ptr(values::ValueVector3D);
%shared_ptr(values::ValueVector3DCongruent);

%shared_ptr(values::ValueMatrix3DInterpolation);
%shared_ptr(values::ValueMatrix3DCongruent);

%shared_ptr(values::ValueString);

%include "..\..\values\src\values_types.hpp"
%include "..\..\values\src\values_base.hpp"
%include "..\..\values\src\values_bool.hpp"
%include "..\..\values\src\values_scalar.hpp"
%include "..\..\values\src\values_scalar_function.hpp"
%include "..\..\values\src\values_scalar_interpolation.hpp"
%include "..\..\values\src\values_vector.hpp"
%include "..\..\values\src\values_matrix_interpolation.hpp"
%include "..\..\values\src\values_string.hpp"
%include "dive_values_scalar_congruent.hpp"
%include "dive_values_vector_congruent.hpp"
%include "dive_values_matrix_congruent.hpp"