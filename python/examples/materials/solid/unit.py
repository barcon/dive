import dive

def Create(tag):
    material                = dive.CreateMaterialSolid(tag)
    valueClass              = dive.CreateValueString('Solid', 'Solid', 'Class')
    valueGroup              = dive.CreateValueString('Unity', 'Group', 'group')
    valueDescription        = dive.CreateValueString('Unity', 'Description', 'description')
    valueName               = dive.CreateValueString('Unity', 'Name', 'name')

    valueDensity            = dive.CreateValueScalar2D(1.0, 'Density', 'rho')
    valuePoissonRatio       = dive.CreateValueScalar2D(1.0, 'Poisson Ratio', 'nu')
    valueThermalExpansion   = dive.CreateValueScalar2D(1.0, 'Coefficient Thermal Expansion', 'alpha')
    valueElasticModulus     = dive.CreateValueScalar2D(1.0, 'Elastic Modulus', 'E')
    valueSpecificHeat       = dive.CreateValueScalar2D(1.0, 'Specific Heat', 'cp')
    valueConductivity       = dive.CreateValueScalar2D(1.0, 'Thermal Conductivity', 'k')
    
    material.SetClass(valueClass)
    material.SetGroup(valueGroup)
    material.SetDescription(valueDescription)
    material.SetName(valueName)
    
    material.SetDensity(valueDensity)
    material.SetPoissonRatio(valuePoissonRatio)
    material.SetThermalExpansion(valueThermalExpansion)
    material.SetElasticModulus(valueElasticModulus)
    material.SetSpecificHeat(valueSpecificHeat)
    material.SetThermalConductivity(valueConductivity)

    return material