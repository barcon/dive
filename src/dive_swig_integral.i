%inline
%{
	#include "integral_types.hpp"
	#include "integral_gauss.hpp"

	typedef std::shared_ptr<integral::IGauss> IGaussPtr;
	typedef std::shared_ptr<const integral::IGauss> ConstIGaussPtr;
	
	typedef std::shared_ptr<integral::GaussHexa> GaussHexaPtr;
	typedef std::shared_ptr<const integral::GaussHexa> ConstGaussHexaPtr;

	typedef std::shared_ptr<integral::GaussRect> GaussRectPtr;
	typedef std::shared_ptr<const integral::GaussRect> ConstGaussRectPtr;
	
	typedef std::shared_ptr<integral::GaussLine> GaussLinePtr;
	typedef std::shared_ptr<const integral::GaussLine> ConstGaussLinePtr;
%}

%shared_ptr(integral::IGauss);
%shared_ptr(integral::GaussHexa);
%shared_ptr(integral::GaussRect);
%shared_ptr(integral::GaussLine);

%include "..\integral\integral_types.hpp"
%include "..\integral\integral_gauss.hpp"