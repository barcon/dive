import dive

def Density(temperature, pressure):
    """
    Calculate density [kg/m³] depending on the temperature [K]
    """    
    rho_oil = 872.0     #[kg/m³]
    T_oil   = 288.15    #[K]
    return rho_oil*(1.0 - 0.00065 * (temperature - T_oil))

def SpecificHeat(temperature, pressure):
    """
    Calculate specific heat [J/(kg.K)] depending on the temperature [K]
    """      
    rho_oil = 872.0     #[kg/m³]
    T_ref = 273.15

    if (Density(temperature, pressure) > 896.0):
        return 3856.0 - 2.345 * rho_oil + 4.605 * (temperature - T_ref)
    else:
        return 2910.0 - 1.29 * rho_oil + 4.605 * (temperature - T_ref) 
    
def Viscosity(temperature, pressure):
    """
    Calculate dynamic viscosity [Pa.s] depending on the temperature [K]
    """     
    ISO = 46.0
    T_ref = 273.15
    nx = 1.8E-4
    rho = Density(temperature, pressure)
    
    return nx * (rho * ISO / (nx * 1.0E6))**((159.56 / ((temperature - T_ref) + 95.0)) - 0.181913)

def ThermalConductivity(temperature, pressure):
    """
    Calculate thermal conductivity [W/(m.K)]
    """     
    return 0.129

def Create(tag, temperature, pressure, viscosityConstant = False):
    """
    Create material with a tag at a fixed temperature [K]
    """    
    material            = dive.CreateMaterialFluid(tag)
    valueClass          = dive.CreateValueString('class', 'Fluid', 'Class')
    valueGroup          = dive.CreateValueString('Mineral Hydraulic Oil', 'Group', 'group')
    valueDescription    = dive.CreateValueString('Oil ISO VG 46', 'Description', 'description')
    valueName           = dive.CreateValueString('ISO VG 46', 'Name', 'name')

    valueDensity        = dive.CreateValueScalar2D(Density(temperature, pressure), 'Density', 'rho')
    valueSpecificHeat   = dive.CreateValueScalar2D(SpecificHeat(temperature, pressure), 'Specific Heat', 'cp')
    valueConductivity   = dive.CreateValueScalar2D(ThermalConductivity(temperature, pressure), 'Thermal Conductivity', 'k') 
    
    if (viscosityConstant):
        valueViscosity  = dive.CreateValueScalar2D(Viscosity(temperature, pressure), 'Dynamic Viscosity', 'mu')    
    else:
        valueViscosity  = dive.CreateValueScalar2DFunction(Viscosity, 'Dynamic Viscosity', 'mu')
 
    material.SetClass(valueClass)
    material.SetGroup(valueGroup)
    material.SetDescription(valueDescription)
    material.SetName(valueName)
    material.SetDensity(valueDensity)
    material.SetSpecificHeat(valueSpecificHeat)
    material.SetThermalConductivity(valueConductivity)
    material.SetDynamicViscosity(valueViscosity)    

    return material