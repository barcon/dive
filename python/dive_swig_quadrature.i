%inline
%{
	#include "quadrature_types.hpp"
	#include "quadrature_gauss.hpp"

	typedef std::shared_ptr<quadrature::IGauss> IGaussPtr;
	typedef std::shared_ptr<const quadrature::IGauss> ConstIGaussPtr;
	
	typedef std::shared_ptr<quadrature::GaussHexa> GaussHexaPtr;
	typedef std::shared_ptr<const quadrature::GaussHexa> ConstGaussHexaPtr;

	typedef std::shared_ptr<quadrature::GaussRect> GaussRectPtr;
	typedef std::shared_ptr<const quadrature::GaussRect> ConstGaussRectPtr;
	
	typedef std::shared_ptr<quadrature::GaussLine> GaussLinePtr;
	typedef std::shared_ptr<const quadrature::GaussLine> ConstGaussLinePtr;
%}

%shared_ptr(quadrature::IGauss);
%shared_ptr(quadrature::GaussHexa);
%shared_ptr(quadrature::GaussRect);
%shared_ptr(quadrature::GaussLine);

%include "..\..\quadrature\src\quadrature_types.hpp"
%include "..\..\quadrature\src\quadrature_gauss.hpp"