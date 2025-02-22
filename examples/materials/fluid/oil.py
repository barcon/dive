import dive

def Density_Oil_VDI2204(temperature):
    """
    Calculate density [kg/m³] depending on the temperature [K]
    """    
    rho_oil = 872.0     #[kg/m³]
    T_oil   = 288.15    #[K]
    return rho_oil*(1.0 - 0.00065 * (temperature - T_oil))

def SpecificHeat_Oil_VDI2204(temperature):
    """
    Calculate specific heat [J/(kg.K)] depending on the temperature [K]
    """      
    rho_oil = 872.0     #[kg/m³]
    T_ref = 273.15

    if (Density_Oil_VDI2204(temperature) > 896.0):
        return 3856.0 - 2.345 * rho_oil + 4.605 * (temperature - T_ref)
    else:
        return 2910.0 - 1.29 * rho_oil + 4.605 * (temperature - T_ref) 
    
def Viscosity_Oil_VDI2204(ISO, temperature):
    """
    Calculate dynamic viscosity [Pa.s] depending on the temperature [K]
    """     
    T_ref = 273.15
    nx = 1.8E-4
    rho = Density_Oil_VDI2204(temperature)
    return nx * (rho * ISO / (nx * 1.0E6))**((159.56 / ((temperature - T_ref) + 95.0)) - 0.181913)

def ThermalConductivity_Oil(temperature):
    """
    Calculate thermal conductivity [W/(m.K)]
    """     
    return 0.129

def Create(tag, ISO, temperature):
    """
    Create material with a tag at a fixed temperature [K]
    """    
    material            = dive.CreateMaterialFluid(tag)
    valueClass          = dive.CreateValueString('class', 'Fluid', 'Class')
    valueGroup          = dive.CreateValueString('Mineral Hydraulic Oil', 'Group', 'group')
    valueDescription    = dive.CreateValueString('Oil ISO VG ' + str(ISO), 'Description', 'description')
    valueName           = dive.CreateValueString('ISO VG ' + str(ISO), 'Name', 'name')

    valueDensity        = dive.CreateValueScalar2D(Density_Oil_VDI2204(temperature), 'Density', 'rho')
    valueSpecificHeat   = dive.CreateValueScalar2D(SpecificHeat_Oil_VDI2204(temperature), 'Specific Heat', 'cp')
    valueConductivity   = dive.CreateValueScalar2D(ThermalConductivity_Oil(temperature), 'Thermal Conductivity', 'k')    
    valueViscosity      = dive.CreateValueScalar2D(Viscosity_Oil_VDI2204(ISO, temperature), 'Dynamic Viscosity', 'mu')    
 
    material.SetClass(valueClass)
    material.SetGroup(valueGroup)
    material.SetDescription(valueDescription)
    material.SetName(valueName)
    material.SetDensity(valueDensity)
    material.SetSpecificHeat(valueSpecificHeat)
    material.SetThermalConductivity(valueConductivity)
    material.SetDynamicViscosity(valueViscosity)    

    return material

#Fluids = {'Oil_ISO_VG68': Fluid_Oil_VG68, 'Unity': Fluid_Unity}

#z = 17
#step= 10.0
#temperatures = [(T_ref + i * step) for i in range(0, z, 1)] 
#pressures = [p_ref for i in range(0, z, 1)]
#viscosities = [Viscosity_Oil_VDI2204(ISO, temperatures[i]) for i in range(0, z, 1)]    
#nodes = dive.vecNodes()

#for i in range (0, z, 1):
#    node = dive.CreateNode(i, temperatures[i], pressures[i], 0.0, viscosities[i])
#    nodes.append(node)

#valueViscosity   = dive.CreateValueScalar2DInterpolation(nodes, dive.interpolation_idw, 'Dynamic Viscosity', 'mu')