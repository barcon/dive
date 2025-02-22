import dive

def Density_Water(temperature): 
    """
    Calculate density [kg/m³] depending on the temperature [K]
    """
    t = (temperature - 273.15)
    
    return (999.83952 + 16.945176 * t - 7.9870401E-3 * t**2 - 46.170461E-6 * t**3 + 105.56302E-9 * t**4 - 280.54253E-12 * t**5)/(1.0 + 16.897850E-3 * t)

def SpecificHeat_Water(temperature):
    """
    Calculate specific heat [J/(kg.K)] depending on the temperature [K]
    """    
    t = (temperature - 273.15)
    
    return 4185.5 * (0.996185 + 0.0002874 * ((t + 100.0)/100.0)**5.26 + 0.011160 * 10.0 ** (-0.036 * t))
    
def Viscosity_Water(temperature):
    """
    Calculate dynamic viscosity [Pa.s] depending on the temperature [K]
    """        
    t = (temperature - 273.15)
    mu = 1002.01 / 10.0**6  
    log = (20.0 - t) / (t + 96.0) * (1.2378 - 1.303 * 10.0**-3 * (20.0 - t) + 3.06 * 10.0**-6 * (20.0 - t)**2 + 2.55 * 10.0**-8 * (20.0 - t)**3)
    return mu * 10.0 ** log

def ThermalConductivity_Water(temperature):
    """
    Calculate thermal conductivity [W/(m.K)] depending on the temperature [K]
    """ 
    t = temperature / 298.15

    return 0.6065 * (-1.48445 + 4.12292 * t - 1.63866 * t**2)

def Create(tag, temperature):
    """
    Create material with a tag at a fixed temperature [K]
    """    
    material            = dive.CreateMaterialFluid(tag)
    valueClass          = dive.CreateValueString('class', 'Fluid', 'Class')
    valueGroup          = dive.CreateValueString('Water', 'Group', 'group')
    valueDescription    = dive.CreateValueString('Liquid Water (Air-Free)', 'Description', 'description')
    valueName           = dive.CreateValueString('Liquid Water', 'Name', 'name')

    valueDensity        = dive.CreateValueScalar2D(Density_Water(temperature), 'Density', 'rho')
    valueSpecificHeat   = dive.CreateValueScalar2D(SpecificHeat_Water(temperature), 'Specific Heat', 'cp')
    valueConductivity   = dive.CreateValueScalar2D(ThermalConductivity_Water(temperature), 'Thermal Conductivity', 'k')    
    valueViscosity      = dive.CreateValueScalar2D(Viscosity_Water(temperature), 'Dynamic Viscosity', 'mu')    
 
    material.SetClass(valueClass)
    material.SetGroup(valueGroup)
    material.SetDescription(valueDescription)
    material.SetName(valueName)
    material.SetDensity(valueDensity)
    material.SetSpecificHeat(valueSpecificHeat)
    material.SetThermalConductivity(valueConductivity)
    material.SetDynamicViscosity(valueViscosity)    

    return material