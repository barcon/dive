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

#meshes.CreateBeam(1.0, 0.1, 0.1, 21, 3, 3, False)
structural.GmshInitialize()
structural.GmshOpenFile("beam.msh")

beam = structural.GmshGetMeshForPhysicalGroup(1, 3, "beam")
fixed = structural.GmshGetNodesForPhysicalGroup(beam, "fixed")
loadEdge = structural.GmshGetEdgesForPhysicalGroup(beam, "loadEdge")
force = structural.CreateValueVector3D([0.0, 400000.0, 0.0])
plot = structural.GmshGetNodesForPhysicalGroup(beam, "plot")

structural.GmshFinalize()

structural.CreateProblem(1, beam, temperature, pressure)
structural.ApplyMaterial(beam, steel)
structural.ApplyDirichlet(fixed, 0.0)
structural.ApplyLoadDistributedEdge(loadEdge, force)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
y = structural.PartitionVector(structural.GetProblem().Displacement())
f = structural.PartitionVector(structural.GetProblem().LoadDistributedEdge())

monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0] + f[1])
structural.UpdateMeshValues(y)

#plots.residual.Show(monitor)
plots.beam.Cantilever(plot, 1)