import meshes
import structural
import solvers
import plots
import plots.beam
import materials.solid.steel

T_ref = 293.15    #[K]      = 20 [°C]
p_ref = 101325.1  #[N/m²]   =  1 [atm]
basis = structural.CreateBasisCartesian(1)
timer = structural.CreateTimerStationary(1, 0.0)
temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)
steel = materials.solid.steel.Create(1)
meshFile = 'beam.msh'

meshes.Initialize(meshFile)
meshes.CreateBeam()

mesh = meshes.GetMeshForPhysicalGroup(meshTag = 1, numberDof = 3, physicalGroup = "problem")
fixed = meshes.GetNodesForPhysicalGroup(mesh = mesh, physicalGroup = "fixed")
loadNodes = meshes.GetNodesForPhysicalGroup(mesh = mesh, physicalGroup = "loadNodes")

meshes.routines.ApplyMaterial(mesh.GetElements(), steel)
meshes.Finalize()

force = structural.CreateValueVector3DScalars(3)
force.SetScalar(1, structural.CreateValueScalar3D(100000))

structural.CreateProblem(1, mesh, temperature, pressure)
structural.ApplyDirichlet(fixed, 0.0)
structural.ApplyLoadNode(loadNodes, force)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
y = structural.PartitionVector(structural.GetProblem().Displacement())
f = structural.PartitionVector(structural.GetProblem().LoadNode())

monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0] + f[1])
structural.UpdateMeshValues(y)

nodesPlot = structural.FilterNodesByCoordinate(mesh.GetNodes(), basis, structural.axis_y, 0.0, 0.001)
nodesPlot = structural.FilterNodesByCoordinate(nodesPlot, basis, structural.axis_z, 0.0, 0.001)

#plots.residual.Show(monitor)
plots.beam.Cantilever(nodesPlot, 1)