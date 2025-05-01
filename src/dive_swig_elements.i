%inline
%{
	#include "dive_elements.hpp"
	#include "dive_elements_hexa.hpp"

	typedef std::shared_ptr<dive::elements::IShape> IShapePtr;
	typedef std::shared_ptr<const dive::elements::IShape> ConstIShapePtr;
	
	typedef std::shared_ptr<dive::elements::IElement> IElementPtr;
	typedef std::shared_ptr<const dive::elements::IElement> ConstIElementPtr;
	
	typedef std::shared_ptr<dive::elements::IElementMass> IElementMassPtr;
	typedef std::shared_ptr<const dive::elements::IElementMass> ConstIElementMassPtr;	

	typedef std::shared_ptr<dive::elements::IElementSpring> IElementSpringPtr;
	typedef std::shared_ptr<const dive::elements::IElementSpring> ConstIElementSpringPtr;	
	
	typedef std::shared_ptr<dive::elements::IElement1D> IElement1DPtr;
	typedef std::shared_ptr<const dive::elements::IElement1D> ConstIElement1DPtr;	

	typedef std::shared_ptr<dive::elements::IElement2D> IElement2DPtr;
	typedef std::shared_ptr<const dive::elements::IElement2D> ConstIElement2DPtr;
	
		typedef std::shared_ptr<dive::elements::IElement3D> IElement3DPtr;
	typedef std::shared_ptr<const dive::elements::IElement3D> ConstIElement3DPtr;

	typedef std::shared_ptr<dive::elements::ElementHexa> ElementHexaPtr;
	typedef std::shared_ptr<const dive::elements::ElementHexa> ConstElementHexaPtr;
	
	typedef std::vector<dive::elements::IElementPtr> Elements;	

%}

%shared_ptr(dive::elements::IShape);
%shared_ptr(dive::elements::IElement);
%shared_ptr(dive::elements::IElementMass);
%shared_ptr(dive::elements::IElementSpring);
%shared_ptr(dive::elements::IElement1D);
%shared_ptr(dive::elements::IElement2D);
%shared_ptr(dive::elements::IElement3D);
%shared_ptr(dive::elements::ElementHexa);

%template(vecElements) std::vector<std::shared_ptr<dive::elements::IElement>>;

%include "dive_elements.hpp"
%include "dive_elements_hexa.hpp"

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