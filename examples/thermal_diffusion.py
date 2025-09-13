import meshes
import thermal
import solvers
import plots
import materials.fluid.VG46

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.fluid.VG46.Create(1, T_ref, p_ref)

meshes.Initialize()
meshes.CreateCavity(1.0, 1.0, 0.1, 101, 101, 5, False)
#meshes.Show()

cavity = meshes.GetMeshForPhysicalGroup(meshTag = 1, numberDof = 3, physicalGroup = "cavity")
wall = meshes.GetNodesForPhysicalGroup(mesh = cavity, physicalGroup = "wall")
hot = meshes.GetNodesForPhysicalGroup(mesh = cavity, physicalGroup = "hot")
plot = meshes.GetNodesForPhysicalGroup(mesh = cavity, physicalGroup = "plot")

meshes.ApplyMaterial(cavity.GetElements(), material)
meshes.Finalize()

thermal.CreateProblem(1, cavity, pressure)
thermal.ApplyDirichlet(hot, 100.0)
thermal.ApplyDirichlet(wall, 0.0)
thermal.Initialize()

#--------------------------------------------------------------------------------------------------

totalDof = thermal.GetProblem().GetTotalDof()
pivot = thermal.GetProblem().GetPivot()

K = thermal.GetProblem().StiffnessParallel()
quit()

K = thermal.PartitionMatrix(thermal.GetProblem().Stiffness())
y = thermal.PartitionVector(thermal.GetProblem().Energy())

monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0])
thermal.UpdateMeshValues(y)
y = thermal.PartitionVector(thermal.GetProblem().Energy())

#plots.residual.Show(monitor)
plots.HeatMapNorm(plot)