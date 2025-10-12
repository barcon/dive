%inline
%{
	#include "material_types.hpp"
	#include "material_base.hpp"
	#include "material_fluid.hpp"
	#include "material_solid.hpp"

	typedef std::shared_ptr<material::IBase> IBasePtr;
	typedef std::shared_ptr<const material::IBase> ConstIBasePtr;
	
	typedef std::shared_ptr<material::IMaterial> IMaterialPtr;
	typedef std::shared_ptr<const material::IMaterial> ConstIMaterialPtr;
	
	typedef std::shared_ptr<material::IMaterialFluid> IMaterialFluidPtr;
	typedef std::shared_ptr<const material::IMaterialFluid> ConstIMaterialFluidPtr;	
	
	typedef std::shared_ptr<material::IMaterialSolid> IMaterialSolidPtr;
	typedef std::shared_ptr<const material::IMaterialSolid> ConstIMaterialSolidPtr;	
	
	typedef std::shared_ptr<material::MaterialSolid> MaterialSolidPtr;
	typedef std::shared_ptr<const material::MaterialSolid> ConstMaterialSolidPtr;	
	
	typedef std::shared_ptr<material::MaterialFluid> MaterialFluidPtr;
	typedef std::shared_ptr<const material::MaterialFluid> ConstMaterialFluidPtr;		
%}

%shared_ptr(material::IBase);
%shared_ptr(material::IMaterial);
%shared_ptr(material::IMaterialFluid);
%shared_ptr(material::IMaterialSolid);

%shared_ptr(material::MaterialFluid);
%shared_ptr(material::MaterialSolid);

%include "..\..\material\src\material_types.hpp"
%include "..\..\material\src\material_base.hpp"
%include "..\..\material\src\material_fluid.hpp"
%include "..\..\material\src\material_solid.hpp"