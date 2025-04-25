import meshes
import materials.solid.steel
import structural

T_ref = 293.15    #[K]      = 20 [°C]
p_ref = 101325.1  #[N/m²]   =  1 [atm]
basis = structural.CreateBasisCartesian(1)
timer = structural.CreateTimerStationary(1, 0.0)
steel = materials.solid.steel.Create(1)

meshes.beam.Create('beam.msh')
mesh = meshes.routines.LoadMesh(1, 'beam.msh', dof = 3)
meshes.routines.ApplyMaterial(mesh.GetElements(), steel)

temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)

nodesLeft = structural.FilterNodesByCoordinate(mesh.GetNodes(), basis, structural.axis_x, 0.0, 0.001)
nodesRight = structural.FilterNodesByCoordinate(mesh.GetNodes(), basis, structural.axis_x, meshes.beam.x, 0.001)

structural.CreateProblem(1, timer, mesh, pressure, None)
structural.ApplyDirichlet(nodesLeft, 0.0)
structural.Initialize()

forceVector = structural.Vector(3)
forceVector[0] = 0.0
forceVector[1] = 0.0
forceVector[2] = -1.0
force = structural.CreateValueVector3D(forceVector)

for node in nodesRight:
    load = structural.CreateLoadNode(node, force)    
    structural.AddLoad(load)

print(structural.LoadNode())