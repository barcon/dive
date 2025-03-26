import dive

def Create(tag):
    material            = dive.CreateMaterialSolid(tag)
    valueClass          = dive.CreateValueString('Solid', 'Solid', 'Class')
    valueGroup          = dive.CreateValueString('Steel', 'Group', 'group')
    valueDescription    = dive.CreateValueString('Structural Steel', 'Description', 'description')
    valueName           = dive.CreateValueString('Structural Steel', 'Name', 'name')

    valueDensity            = dive.CreateValueScalar2D(7850.0, 'Density', 'rho')
    valuePoissonRatio       = dive.CreateValueScalar2D(0.30, 'Poisson Ratio', 'nu')
    valueThermalExpansion   = dive.CreateValueScalar2D(0.000012, 'Coefficient Thermal Expansion', 'alpha')
    valueElasticModulus     = dive.CreateValueScalar2D(2.07e+11, 'Elastic Modulus', 'E')
    valueSpecificHeat       = dive.CreateValueScalar2D(490.0, 'Specific Heat', 'cp')
    valueConductivity       = dive.CreateValueScalar2D(51.0, 'Thermal Conductivity', 'k')
    
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

    valueFlag   = dive.CreateValueBool(True, 'Flag', 'flag')
    valueScalar = dive.CreateValueScalar(1.2345, 'Scalar', 'scalar')
    valueVector = dive.CreateValueVector(dive.Vector(3, 1.234), 'Vector', 'vector')
    valueMatrix = dive.CreateValueMatrix(dive.Matrix(3, 3, 1.234), 'Matrix', 'matrix')
    
    material.SetProperty(valueFlag)
    material.SetProperty(valueScalar)
    material.SetProperty(valueVector)
    material.SetProperty(valueMatrix)

    return material