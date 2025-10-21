%inline
%{
	#include "dive_weakforms.hpp"

	typedef std::shared_ptr<dive::weakforms::IWeakFormElement> IWeakFormElementPtr;
	typedef std::shared_ptr<const dive::weakforms::IWeakFormElement> ConstIWeakFormElementPtr;
	
	typedef std::shared_ptr<dive::weakforms::IWeakFormLoad> IWeakFormLoadPtr;
	typedef std::shared_ptr<const dive::weakforms::IWeakFormLoad> ConstIWeakFormLoadPtr;	
%}

%shared_ptr(dive::weakforms::IWeakFormElement);
%shared_ptr(dive::weakforms::IWeakFormLoad);

%include "dive_weakforms.hpp"