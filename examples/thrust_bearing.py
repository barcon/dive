import materials.fluid.oil_vg46
import materials.solid.steel
import bearing.segment.sector_parallel
import bearing.segment.mesh.sector_parallel

T_oil_bath  = 313.15    #[K]      = 40 [°C]
p_oil_bath  = 101325.1  #[N/m²]   =  1 [atm]

materials.fluid.oil_vg46.configuration.viscosityConstant = False
oil     = materials.fluid.oil_vg46.Create(1, T_oil_bath, p_oil_bath)
steel   = materials.solid.steel.Create(2)

segment = bearing.segment.sector_parallel.Create()
segment.SetChamferNone()
segment.diameter = 2860.0
segment.size = 675.0
segment.number = 12
segment.groove = 145.0
segment.thickness = 150.0

meshFileSegment = 'segment.msh'
meshSegment = bearing.segment.mesh.sector_parallel.Print(segment)