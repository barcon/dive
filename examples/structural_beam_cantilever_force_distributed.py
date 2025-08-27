import meshes
import structural
import solvers
import plots
import plots.beam
import materials.solid.steel

T_ref = 293.15      #[K]      = 20 [°C]
p_ref = 101325.1    #[N/m²]   =  1 [atm]
basis = structural.CreateBasisCartesian(1)
timer = structural.CreateTimerStationary(1, 0.0)
temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)
gravity = -9.8      #[m/s²]

steel = materials.solid.steel.Create(1)
density = steel.GetDensity(T_ref, p_ref)

meshes.Initialize()
meshes.CreateBeam(1.0, 0.1, 0.1, 21, 2, 2, True)

beam = meshes.GetMeshForPhysicalGroup(meshTag = 1, numberDof = 3, physicalGroup = "beam")
fixed = meshes.GetNodesForPhysicalGroup(mesh = beam, physicalGroup = "fixed")
loadVolume = meshes.GetElementsForPhysicalGroup(mesh = beam, physicalGroup = "loadVolume")
weight = structural.CreateValueVector3DScalars([0.0, density * gravity, 0.0])

meshes.ApplyMaterial(beam.GetElements(), steel)
meshes.Finalize()

structural.CreateProblem(1, beam, temperature, pressure)
structural.ApplyDirichlet(fixed, 0.0)
structural.ApplyLoadDistributedVolume(loadVolume, weight)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
y = structural.PartitionVector(structural.GetProblem().Displacement())
f = structural.PartitionVector(structural.GetProblem().LoadDistributedVolume())

monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0] + f[1])
structural.UpdateMeshValues(y)

#plots.residual.Show(monitor)
nodesPlot = structural.FilterNodesByCoordinate(beam.GetNodes(), basis, structural.axis_y, 0.0, 0.001)
nodesPlot = structural.FilterNodesByCoordinate(nodesPlot, basis, structural.axis_z, 0.0, 0.001)
plots.beam.Cantilever(nodesPlot, 1)