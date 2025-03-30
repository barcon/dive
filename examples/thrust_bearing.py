import materials.fluid.oil_vg46
import materials.solid.steel

import fluid.momentum
import fluid.pressure
#import structural
#import thermal

T_oil_bath  = 313.15    #[K]      = 50 [°C]
p_oil_bath  = 101325.1  #[N/m²]   =  1 [atm]

F_axial     = 1000.0    #[kN]
D_outer     = 2000.0
D_inner     = 1000.0

basis   = fluid.CreateBasisCartesian(1)
timer   = fluid.CreateTimerStepped(1, 0.0, 20000.0, 10.0)

materials.fluid.oil_vg46.configuration.ISO = 46.0
materials.fluid.oil_vg46.configuration.viscosityConstant = False

oil     = materials.fluid.oil_vg46.Create(1, T_oil_bath, p_oil_bath)
steel   = materials.solid.steel.Create(2)

meshFluid   = 'gleiter_fluid.msh'

#temperature = fluid.CreateValueScalar3DCongruent(meshTemperature)
#pressure    = fluid.CreateValueScalar3DCongruent(meshPressure)
#velocity    = fluid.CreateValueMatrix3DCongruent(meshVelocity)