%inline
%{
	#include "dive_elements.hpp"
	#include "dive_elements_hexa.hpp"
	#include "dive_elements_mass.hpp"
	#include "dive_elements_combined.hpp"

	typedef std::shared_ptr<dive::elements::IElement> IElementPtr;
	typedef std::shared_ptr<const dive::elements::IElement> ConstIElementPtr;

	typedef std::shared_ptr<dive::elements::IElementMapped> IElementMappedPtr;
	typedef std::shared_ptr<const dive::elements::IElementMapped> ConstIElementMappedPtr;

	typedef std::shared_ptr<dive::elements::IElementBeam> IElementBeamPtr;
	typedef std::shared_ptr<const dive::elements::IElementBeam> ConstIElementBeamPtr;

	typedef std::shared_ptr<dive::elements::IElementShell> IElementShellPtr;
	typedef std::shared_ptr<const dive::elements::IElementShell> ConstIElementShellPtr;

	typedef std::shared_ptr<dive::elements::IElementSolid> IElementSolidPtr;
	typedef std::shared_ptr<const dive::elements::IElementSolid> ConstIElementSolidPtr;

	typedef std::shared_ptr<dive::elements::ElementHexa> ElementHexaPtr;
	typedef std::shared_ptr<const dive::elements::ElementHexa> ConstElementHexaPtr;

	typedef std::shared_ptr<dive::elements::ElementMass> ElementMassPtr;
	typedef std::shared_ptr<const dive::elements::ElementMass> ConstElementMassPtr;

	typedef std::shared_ptr<dive::elements::ElementCombined> ElementCombinedPtr;
	typedef std::shared_ptr<const dive::elements::ElementCombined> ConstElementCombinedPtr;
	
	typedef std::vector<dive::elements::IElementPtr> Elements;

	typedef struct dive::elements::FacePair FacePair;
	typedef struct dive::elements::EdgePair EdgePair;
%}

%shared_ptr(dive::elements::IElement);
%shared_ptr(dive::elements::IElementMapped);
%shared_ptr(dive::elements::IElementShape);
%shared_ptr(dive::elements::IElementBeam);
%shared_ptr(dive::elements::IElementShell);
%shared_ptr(dive::elements::IElementSolid);

%shared_ptr(dive::elements::ElementHexa);
%shared_ptr(dive::elements::ElementMass);
%shared_ptr(dive::elements::ElementCombined);

%template(vecElements) std::vector<std::shared_ptr<dive::elements::IElement>>;
%template(vecFacePairs) std::vector<struct dive::elements::FacePair>;
%template(vecEdgePairs) std::vector<struct dive::elements::EdgePair>;

%include "dive_elements.hpp"
%include "dive_elements_hexa.hpp"
%include "dive_elements_mass.hpp"
%include "dive_elements_combined.hpp"

%pythoncode
%{

def AddVecElements(self, elements):
    for element in elements:
        self.append(element)
    return self

def CompVecElements(self, other):
    return self.GetTag() == other.GetTag()

def HashVecElements(self):
    return hash(self.GetTag())

vecElements.__add__ = AddVecElements
vecElements.__eq__ = CompVecElements
vecElements.__hash__ = HashVecElements

%}