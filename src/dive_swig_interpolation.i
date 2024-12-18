%inline
%{
	#include "interpolation_types.hpp"
	#include "interpolation_fixed.hpp"
	#include "interpolation_idw.hpp"
	#include "interpolation_nn.hpp"
	#include "interpolation_rbf.hpp"

	typedef std::shared_ptr<interpolation::IInterpolation> IInterpolationPtr;
	typedef std::shared_ptr<const interpolation::IInterpolation> ConstIInterpolationPtr;

	typedef std::shared_ptr<interpolation::InterpolationFixed> InterpolationFixedPtr;
	typedef std::shared_ptr<const interpolation::InterpolationFixed> ConstInterpolationFixedPtr;
	
	typedef std::shared_ptr<interpolation::InterpolationIDW> InterpolationIDWPtr;
	typedef std::shared_ptr<const interpolation::InterpolationIDW> ConstInterpolationIDWPtr;	
	
	typedef std::shared_ptr<interpolation::InterpolationNN> InterpolationNNPtr;
	typedef std::shared_ptr<const interpolation::InterpolationNN> ConstInterpolationNNPtr;		
	
	typedef std::shared_ptr<interpolation::InterpolationRBF> InterpolationRBFPtr;
	typedef std::shared_ptr<const interpolation::InterpolationRBF> ConstInterpolationRBFPtr;			
%}

%shared_ptr(interpolation::IInterpolation);
%shared_ptr(interpolation::InterpolationFixed);
%shared_ptr(interpolation::InterpolationIDW);
%shared_ptr(interpolation::InterpolationNN);
%shared_ptr(interpolation::InterpolationRBF);

%include "..\interpolation\interpolation_types.hpp"
%include "..\interpolation\interpolation_fixed.hpp"
%include "..\interpolation\interpolation_idw.hpp"
%include "..\interpolation\interpolation_nn.hpp"
%include "..\interpolation\interpolation_rbf.hpp"