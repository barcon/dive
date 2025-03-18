import materials.fluid.oil
import materials.solid.steel

import fluid.momentum
import fluid.pressure
#import structural
#import thermal

T_oil_bath  = 323.15    #[K]      = 50 [°C]
F_axial     = 1000.0    #[kN]
D_outer     = 2000.0
D_inner     = 1000.0

basis   = fluid.CreateBasisCartesian(1)
timer   = fluid.CreateTimerStepped(1, 0.0, 20000.0, 10.0)
oil     = materials.fluid.oil.Create(1, ISO = 46, temperature = T_oil_bath)
steel   = materials.solid.steel.Create(2)

meshSegment = 'gleiter_segment.msh'
meshRunner  = 'gleiter_runner.msh'
meshFluid   = 'gleiter_fluid.msh'

#temperature = fluid.CreateValueScalar3DCongruent(meshTemperature)
#pressure    = fluid.CreateValueScalar3DCongruent(meshPressure)
#velocity    = fluid.CreateValueMatrix3DCongruent(meshVelocity)
