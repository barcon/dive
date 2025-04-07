import materials.fluid.VG46
import materials.solid.steel
import bearing.segment.axial
import bearing.segment.mesh

T_oil_bath  = 313.15    #[K]      = 40 [°C]
p_oil_bath  = 101325.1  #[N/m²]   =  1 [atm]

materials.fluid.VG46.configuration.viscosityConstant = False
oil     = materials.fluid.VG46.Create(1, T_oil_bath, p_oil_bath)
steel   = materials.solid.steel.Create(2)

segment = bearing.segment.axial.SectorParallel()
segment.SetChamferParallel(30, 0.2)
segment.bidirectional = False
segment.diameter = 2860.0
segment.size = 675.0
segment.number = 12
segment.groove = 145.0
segment.thickness = 150.0

bearing.segment.mesh.Sector('segment.msh', segment, 0.1)