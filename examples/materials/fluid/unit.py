import dive

def Create(tag):
    material            = dive.CreateMaterialFluid(tag)
    valueClass          = dive.CreateValueString('Fluid', 'Fluid', 'Class')
    valueGroup          = dive.CreateValueString('Unity', 'Group', 'group')
    valueDescription    = dive.CreateValueString('Unity', 'Description', 'description')
    valueName           = dive.CreateValueString('Unity', 'Name', 'name')

    valueDensity        = dive.CreateValueScalar2D(1.0, 'Density', 'rho')
    valueSpecificHeat   = dive.CreateValueScalar2D(1.0, 'Specific Heat', 'cp')
    valueConductivity   = dive.CreateValueScalar2D(1.0, 'Thermal Conductivity', 'k')    
    valueViscosity      = dive.CreateValueScalar2D(1.0, 'Dynamic Viscosity', 'mu') 
 
    material.SetClass(valueClass)
    material.SetGroup(valueGroup)
    material.SetDescription(valueDescription)
    material.SetName(valueName)
    material.SetDensity(valueDensity)
    material.SetSpecificHeat(valueSpecificHeat)
    material.SetThermalConductivity(valueConductivity)
    material.SetDynamicViscosity(valueViscosity)   

    return material