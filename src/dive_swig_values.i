%typemap(in) double (*function_Pointer_D)() {
    $1 = (double (*)())PyLong_AsVoidPtr($input);
}

%typemap(in) double (*function_Pointer_D_D)(double) {
    $1 = (double (*)(double))PyLong_AsVoidPtr($input);
}

%typemap(in) double (*function_Pointer_D_DD)(double, double) {
    $1 = (double (*)(double, double))PyLong_AsVoidPtr($input);
}

%typemap(in) double (*function_Pointer_D_DDD)(double, double, double) {
    $1 = (double (*)(double, double, double))PyLong_AsVoidPtr($input);
}

%typemap(in) double (*function_Pointer_D_DDDD)(double, double, double, double) {
    $1 = (double (*)(double, double, double, double))PyLong_AsVoidPtr($input);
}

%inline
%{
	#include "values_types.hpp"
	#include "values_bool.hpp"
	#include "values_string.hpp"	
	#include "values_scalar.hpp"
	#include "values_scalar_time.hpp"
	#include "values_scalar_time_function.hpp"
	#include "values_scalar_function.hpp"
	#include "values_scalar_interpolation.hpp"
	#include "values_vector.hpp"
	#include "values_vector_scalars.hpp"
	#include "values_vector_scalars_time.hpp"
	#include "values_matrix.hpp"
	#include "values_matrix_interpolation.hpp"

	#include "dive_values_scalar_congruent.hpp"
	#include "dive_values_matrix_congruent.hpp"
	#include "dive_values_vector_congruent.hpp"

	typedef std::shared_ptr<values::IValue> IValuePtr;
	typedef std::shared_ptr<values::IBool> IBoolPtr;
	typedef std::shared_ptr<values::IString> IStringPtr;
	
	typedef std::shared_ptr<values::IScalar> IScalarPtr;
	typedef std::shared_ptr<values::IScalar1D> IScalar1DPtr;
	typedef std::shared_ptr<values::IScalar2D> IScalar2DPtr;
	typedef std::shared_ptr<values::IScalar3D> IScalar3DPtr;
	
	typedef std::shared_ptr<values::IVector> IVector;
	typedef std::shared_ptr<values::IVector1D> IVector1DPtr;
	typedef std::shared_ptr<values::IVector2D> IVector2DPtr;
	typedef std::shared_ptr<values::IVector3D> IVector3DPtr;

	typedef std::shared_ptr<values::IMatrix> IMatrix;
	typedef std::shared_ptr<values::IMatrix1D> IMatrix1DPtr;
	typedef std::shared_ptr<values::IMatrix2D> IMatrix2DPtr;
	typedef std::shared_ptr<values::IMatrix3D> IMatrix3DPtr;

	typedef std::shared_ptr<values::IScalarTime> IScalarTimePtr;
	typedef std::shared_ptr<values::IScalar1DTime> IScalar1DTimePtr;
	typedef std::shared_ptr<values::IScalar2DTime> IScalar2DTimePtr;
	typedef std::shared_ptr<values::IScalar3DTime> IScalar3DTimePtr;
	
	typedef std::shared_ptr<values::IVectorTime> IVectorTime;
	typedef std::shared_ptr<values::IVector1DTime> IVector1DTimePtr;
	typedef std::shared_ptr<values::IVector2DTime> IVector2DTimePtr;
	typedef std::shared_ptr<values::IVector3DTime> IVector3DTimePtr;
	
	typedef std::shared_ptr<values::IMatrixTime> IMatrixTime;
	typedef std::shared_ptr<values::IMatrix1DTime> IMatrix1DTimePtr;
	typedef std::shared_ptr<values::IMatrix2DTime> IMatrix2DTimePtr;
	typedef std::shared_ptr<values::IMatrix3DTime> IMatrix3DTimePtr;
	
	typedef std::shared_ptr<values::ValueBool> ValueBoolPtr;
	typedef std::shared_ptr<values::ValueString> ValueStringPtr;
	
	typedef std::shared_ptr<values::ValueScalar> ValueScalarPtr;
	typedef std::shared_ptr<values::ValueScalarTime> ValueScalarTimePtr;
	typedef std::shared_ptr<values::ValueScalarTimeFunction> ValueScalarTimeFunctionPtr;	
	typedef std::shared_ptr<values::ValueScalarFunction> ValueScalarFunctionPtr;	
	typedef std::shared_ptr<values::ValueScalar1D> ValueScalar1DPtr;
	typedef std::shared_ptr<values::ValueScalar1DTime> ValueScalar1DTimePtr;
	typedef std::shared_ptr<values::ValueScalar1DTimeFunction> ValueScalar1DTimeFunctionPtr;	
	typedef std::shared_ptr<values::ValueScalar1DFunction> ValueScalar1DFunctionPtr;	
	typedef std::shared_ptr<values::ValueScalar2D> ValueScalar2DPtr;
	typedef std::shared_ptr<values::ValueScalar2DTime> ValueScalar2DTimePtr;
	typedef std::shared_ptr<values::ValueScalar2DTimeFunction> ValueScalar2DTimeFunctionPtr;
	typedef std::shared_ptr<values::ValueScalar2DFunction> ValueScalar2DFunctionPtr;
	typedef std::shared_ptr<values::ValueScalar3D> ValueScalar3DPtr;
	typedef std::shared_ptr<values::ValueScalar3DTime> ValueScalar3DTimePtr;
	typedef std::shared_ptr<values::ValueScalar3DTimeFunction> ValueScalar3DTimeFunctionPtr;
	typedef std::shared_ptr<values::ValueScalar3DFunction> ValueScalar3DFunctionPtr;
	typedef std::shared_ptr<values::ValueScalar3DInterpolation> ValueScalar3DInterpolationPtr;
	typedef std::shared_ptr<values::ValueScalar3DCongruent> ValueScalar3DCongruentPtr;	
	
	typedef std::shared_ptr<values::ValueVector> ValueVectorPtr;
	typedef std::shared_ptr<values::ValueVectorScalars> ValueVectorScalarsPtr;
	typedef std::shared_ptr<values::ValueVectorScalarsTime> ValueVectorScalarsTimePtr;
	typedef std::shared_ptr<values::ValueVector1D> ValueVector1DPtr;
	typedef std::shared_ptr<values::ValueVector1DScalars> ValueVector1DScalarsPtr;
	typedef std::shared_ptr<values::ValueVector1DScalarsTime> ValueVector1DScalarsTimePtr;
	typedef std::shared_ptr<values::ValueVector2D> ValueVector2DPtr;
	typedef std::shared_ptr<values::ValueVector2DScalars> ValueVector2DScalarsPtr;
	typedef std::shared_ptr<values::ValueVector2DScalarsTime> ValueVector2DScalarsTimePtr;
	typedef std::shared_ptr<values::ValueVector3D> ValueVector3DPtr;
	typedef std::shared_ptr<values::ValueVector3DScalars> ValueVector3DScalarsPtr;
	typedef std::shared_ptr<values::ValueVector3DScalarsTime> ValueVector3DScalarsTimePtr;
	typedef std::shared_ptr<values::ValueVector3DCongruent> ValueVector3DCongruentPtr;
	
	typedef std::shared_ptr<values::ValueMatrix> ValueMatrixPtr;
	typedef std::shared_ptr<values::ValueMatrix1D> ValueMatrix1DPtr;
	typedef std::shared_ptr<values::ValueMatrix2D> ValueMatrix2DPtr;
	typedef std::shared_ptr<values::ValueMatrix3D> ValueMatrix3DPtr;
	typedef std::shared_ptr<values::ValueMatrix3DInterpolation> ValueMatrix3DInterpolationPtr;
	typedef std::shared_ptr<values::ValueMatrix3DCongruent> ValueMatrix3DCongruentPtr;	
%}

%shared_ptr(values::IValue);
%shared_ptr(values::IBool);
%shared_ptr(values::IString);

%shared_ptr(values::IScalar);
%shared_ptr(values::IScalar1D);
%shared_ptr(values::IScalar2D);
%shared_ptr(values::IScalar3D);

%shared_ptr(values::IVector);
%shared_ptr(values::IVector1D);
%shared_ptr(values::IVector2D);
%shared_ptr(values::IVector3D);

%shared_ptr(values::IMatrix);
%shared_ptr(values::IMatrix1D);
%shared_ptr(values::IMatrix2D);
%shared_ptr(values::IMatrix3D);

%shared_ptr(values::IScalarTime);
%shared_ptr(values::IScalar1DTime);
%shared_ptr(values::IScalar2DTime);
%shared_ptr(values::IScalar3DTime);

%shared_ptr(values::IVectorTime);
%shared_ptr(values::IVector1DTime);
%shared_ptr(values::IVector2DTime);
%shared_ptr(values::IVector3DTime);

%shared_ptr(values::IMatrixTime);
%shared_ptr(values::IMatrix1DTime);
%shared_ptr(values::IMatrix2DTime);
%shared_ptr(values::IMatrix3DTime);

%shared_ptr(values::ValueBool);
%shared_ptr(values::ValueString);

%shared_ptr(values::ValueScalar);
%shared_ptr(values::ValueScalarTime);
%shared_ptr(values::ValueScalarTimeFunction);
%shared_ptr(values::ValueScalarFunction);
%shared_ptr(values::ValueScalar1D);
%shared_ptr(values::ValueScalar1DTime);
%shared_ptr(values::ValueScalar1DTimeFunction);
%shared_ptr(values::ValueScalar1DFunction);
%shared_ptr(values::ValueScalar2D);
%shared_ptr(values::ValueScalar2DTime);
%shared_ptr(values::ValueScalar2DTimeFunction);
%shared_ptr(values::ValueScalar2DFunction);
%shared_ptr(values::ValueScalar3D);
%shared_ptr(values::ValueScalar3DTime);
%shared_ptr(values::ValueScalar3DTimeFunction);
%shared_ptr(values::ValueScalar3DFunction);
%shared_ptr(values::ValueScalar3DInterpolation);
%shared_ptr(values::ValueScalar3DCongruent);

%shared_ptr(values::ValueVector);
%shared_ptr(values::ValueVectorScalars);
%shared_ptr(values::ValueVectorScalarsTime);
%shared_ptr(values::ValueVector1D);
%shared_ptr(values::ValueVector1DScalars);
%shared_ptr(values::ValueVector1DScalarsTime);
%shared_ptr(values::ValueVector2D);
%shared_ptr(values::ValueVector2DScalars);
%shared_ptr(values::ValueVector2DScalarsTime);
%shared_ptr(values::ValueVector3D);
%shared_ptr(values::ValueVector3DScalars);
%shared_ptr(values::ValueVector3DScalarsTime);
%shared_ptr(values::ValueVector3DCongruent);

%shared_ptr(values::ValueMatrix);
%shared_ptr(values::ValueMatrix1D);
%shared_ptr(values::ValueMatrix2D);
%shared_ptr(values::ValueMatrix3D);
%shared_ptr(values::ValueMatrix3DInterpolation);
%shared_ptr(values::ValueMatrix3DCongruent);

%include "..\..\values\src\values_types.hpp"
%include "..\..\values\src\values_bool.hpp"
%include "..\..\values\src\values_string.hpp"
%include "..\..\values\src\values_scalar.hpp"
%include "..\..\values\src\values_scalar_time.hpp"
%include "..\..\values\src\values_scalar_time_function.hpp"
%include "..\..\values\src\values_scalar_function.hpp"
%include "..\..\values\src\values_scalar_interpolation.hpp"
%include "..\..\values\src\values_vector.hpp"
%include "..\..\values\src\values_vector_scalars.hpp"
%include "..\..\values\src\values_vector_scalars_time.hpp"
%include "..\..\values\src\values_matrix.hpp"
%include "..\..\values\src\values_matrix_interpolation.hpp"
%include "dive_values_scalar_congruent.hpp"
%include "dive_values_vector_congruent.hpp"
%include "dive_values_matrix_congruent.hpp"

%pythoncode
%{

import ctypes

py_function_pointer_D = ctypes.CFUNCTYPE(ctypes.c_double)
py_function_pointer_D_D = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double)
py_function_pointer_D_DD = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_double)
py_function_pointer_D_DDD = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double)
py_function_pointer_D_DDDD = ctypes.CFUNCTYPE(ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double)

def CreateValueScalarFunction(function, name = "", key = ""):
	f = py_function_pointer_D(function)
	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
	return _dive.CreateValueScalarFunction(f_ptr, name, key)

def CreateValueScalar1DFunction(function, name = "", key = ""):
	f = py_function_pointer_D_D(function)
	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
	return _dive.CreateValueScalar1DFunction(f_ptr, name, key)

def CreateValueScalar2DFunction(function, name = "", key = ""):
	f = py_function_pointer_D_DD(function)
	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
	return _dive.CreateValueScalar2DFunction(f_ptr, name, key)

def CreateValueScalar3DFunction(function, name = "", key = ""):
	f = py_function_pointer_D_DDD(function)
	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
	return _dive.CreateValueScalar3DFunction(f_ptr, name, key)

#------------------------------------------------------------------------------
	
def CreateValueScalarTimeFunction(function, name = "", key = ""):
	f = py_function_pointer_D_D(function)
	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
	return _dive.CreateValueScalarTimeFunction(f_ptr, name, key)

def CreateValueScalar1DTimeFunction(function, name = "", key = ""):
	f = py_function_pointer_D_DD(function)
	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
	return _dive.CreateValueScalar1DTimeFunction(f_ptr, name, key)

def CreateValueScalar2DTimeFunction(function, name = "", key = ""):
	f = py_function_pointer_D_DDD(function)
	f_ptr = ctypes.cast(f, ctypes.c_void_p).value
	
	return _dive.CreateValueScalar2DTimeFunction(f_ptr, name, key)

def CreateValueScalar3DTimeFunction(function, name = "", key = ""):
	f = py_function_pointer_D_DDDD(function)
	f_ptr = ctypes.cast(f, ctypes.c_void_p).value	
	
	return _dive.CreateValueScalar3DTimeFunction(f_ptr, name, key)
%}