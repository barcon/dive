import dive

from dataclasses import dataclass, field

@dataclass
class Configuration:
    densityConstant : bool = True
    specificHeatConstant : bool = True
    viscosityConstant : bool = True

configuration = Configuration()    

def Density(temperature, pressure): 
    """
    Calculate density [kg/m³] depending on the temperature [K]
    """
    t = (temperature - 273.15)
    
    return (999.83952 + 16.945176 * t - 7.9870401E-3 * t**2 - 46.170461E-6 * t**3 + 105.56302E-9 * t**4 - 280.54253E-12 * t**5)/(1.0 + 16.897850E-3 * t)

def SpecificHeat(temperature, pressure):
    """
    Calculate specific heat [J/(kg.K)] depending on the temperature [K]
    """    
    t = (temperature - 273.15)
    
    return 4185.5 * (0.996185 + 0.0002874 * ((t + 100.0)/100.0)**5.26 + 0.011160 * 10.0 ** (-0.036 * t))
    
def Viscosity(temperature, pressure):
    """
    Calculate dynamic viscosity [Pa.s] depending on the temperature [K]
    """        
    t = (temperature - 273.15)
    mu = 1002.01 / 10.0**6  
    log = (20.0 - t) / (t + 96.0) * (1.2378 - 1.303 * 10.0**-3 * (20.0 - t) + 3.06 * 10.0**-6 * (20.0 - t)**2 + 2.55 * 10.0**-8 * (20.0 - t)**3)
    return mu * 10.0 ** log

def ThermalConductivity(temperature, pressure):
    """
    Calculate thermal conductivity [W/(m.K)] depending on the temperature [K]
    """ 
    t = temperature / 298.15

    return 0.6065 * (-1.48445 + 4.12292 * t - 1.63866 * t**2)

def Create(tag, temperature, pressure):
    """
    Create material with a tag at a fixed temperature [K]
    """    
    material            = dive.CreateMaterialFluid(tag)
    valueClass          = dive.CreateValueString('class', 'Fluid', 'Class')
    valueGroup          = dive.CreateValueString('Water', 'Group', 'group')
    valueDescription    = dive.CreateValueString('Liquid Water (Air-Free)', 'Description', 'description')
    valueName           = dive.CreateValueString('Liquid Water', 'Name', 'name')

    if (configuration.specificHeatConstant):
        valueSpecificHeat = dive.CreateValueScalar2D(SpecificHeat(temperature, pressure), 'Specific Heat', 'cp')
    else:
        valueSpecificHeat = dive.CreateValueScalar2DFunction(SpecificHeat, 'Specific Heat', 'cp') 

    if (configuration.densityConstant):
        valueDensity = dive.CreateValueScalar2D(Density(temperature, pressure), 'Density', 'rho')    
    else:
        valueDensity = dive.CreateValueScalar2DFunction(Density, 'Density', 'rho')   

    if (configuration.viscosityConstant):
        valueViscosity = dive.CreateValueScalar2D(Viscosity(temperature, pressure), 'Dynamic Viscosity', 'mu')    
    else:
        valueViscosity = dive.CreateValueScalar2DFunction(Viscosity, 'Dynamic Viscosity', 'mu')

    valueConductivity   = dive.CreateValueScalar2D(ThermalConductivity(temperature, pressure), 'Thermal Conductivity', 'k') 
   
    material.SetClass(valueClass)
    material.SetGroup(valueGroup)
    material.SetDescription(valueDescription)
    material.SetName(valueName)
    material.SetDensity(valueDensity)
    material.SetSpecificHeat(valueSpecificHeat)
    material.SetThermalConductivity(valueConductivity)
    material.SetDynamicViscosity(valueViscosity)    

    return material