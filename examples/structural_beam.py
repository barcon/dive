import meshes
import materials.solid.steel
import structural

T_ref  = 293.15    #[K]      = 20 [°C]
p_ref  = 101325.1  #[N/m²]   =  1 [atm]

steel   = materials.solid.steel.Create(1)

meshes.beam.Create('beam.msh')
mesh = meshes.routines.LoadMesh(1, 'beam.msh', dof = 3)
meshes.routines.ApplyMaterial(mesh.GetElements(), steel)

temperature = structural.CreateValueScalar3D(T_ref, "Temperature", "T_ref")
pressure = structural.CreateValueScalar3D(p_ref, "Temperature", "p_ref")

nodesLeft = structural.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, 0.0, 0.001)
nodesRight = structural.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, meshes.cavity.x, 0.001)

thermal.CreateProblem(1, timer, mesh, pressure, None)
thermal.ApplyDirichlet(nodesLeft, 0.0)
thermal.ApplyDirichlet(nodesRight, 100.0)
thermal.Initialize()