import materials.fluid.VG46
import materials.solid.steel

import bearing.axial.segment
import bearing.axial.film
import bearing.axial.mesh

import meshes

T_oil_bath  = 313.15    #[K]      = 40 [°C]
p_oil_bath  = 101325.1  #[N/m²]   =  1 [atm]

materials.fluid.VG46.configuration.viscosityConstant = False
oil     = materials.fluid.VG46.Create(1, T_oil_bath, p_oil_bath)
steel   = materials.solid.steel.Create(2)

segment = bearing.axial.segment.SectorParallel()
segment.SetChamferParallel(30, 1.0)
segment.bidirectional = True
segment.diameter = 2860.0
segment.size = 675.0
segment.number = 12
segment.groove = 145.0
segment.thickness = 150.0

film = bearing.axial.film.SectorParallel(segment)
film.initialThickness = 0.1

bearing.axial.mesh.Sector('segment.cgns', segment, bearing.axial.mesh.sectorParameters)
bearing.axial.mesh.Sector('film.cgns', film, bearing.axial.mesh.filmParameters)

meshFilm = meshes.routines.LoadMesh(1, 'film.msh', dof = 3)
meshSegment = meshes.routines.LoadMesh(1, 'segment.msh', dof = 3)
#meshSegment = meshes.routines.DeformByInterpolation(meshSegment, crowning)