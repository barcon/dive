%inline
%{
	#include "basis_types.hpp"
	#include "basis_cartesian.hpp"		
	#include "basis_cylindrical.hpp"		
	
	typedef std::shared_ptr<basis::IBasis> IBasisPtr;
	typedef std::shared_ptr<const basis::IBasis> ConstIBasisPtr;

	typedef std::shared_ptr<basis::Cartesian> CartesianPtr;
	typedef std::shared_ptr<const basis::Cartesian> ConstCartesianPtr;

	typedef std::shared_ptr<basis::Cylindrical> CylindricalPtr;
	typedef std::shared_ptr<const basis::Cylindrical> ConstCylindricalPtr;
%}

%shared_ptr(basis::IBasis);
%shared_ptr(basis::Cartesian);
%shared_ptr(basis::Cylindrical);

%template(vecBasis) std::vector<std::shared_ptr<basis::IBasis>>;

%include "..\..\basis\src\basis_types.hpp"
%include "..\..\basis\src\basis_cartesian.hpp"
%include "..\..\basis\src\basis_cylindrical.hpp"