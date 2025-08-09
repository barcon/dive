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
meshFile    = 'cavity.cgns'

meshes.cavity.quadratic = False
meshes.cavity.Create(meshFile)
mesh = meshes.routines.LoadMesh(1, meshFile)
meshes.routines.ApplyMaterial(mesh.GetElements(), material)

#--------------------------------------------------------------------------------------------------

nodesTop = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_y, meshes.cavity.y, 0.001)
nodesBottom = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_y, 0.0, 0.001)
nodesLeft = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, 0.0, 0.001)
nodesRight = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, meshes.cavity.x, 0.001)

thermal.CreateProblem(1, mesh, pressure)
thermal.ApplyDirichlet(nodesTop, 100.0)
thermal.ApplyDirichlet(nodesBottom, 0.0)
thermal.ApplyDirichlet(nodesLeft, 0.0)
thermal.ApplyDirichlet(nodesRight, 0.0)
thermal.Initialize()

#--------------------------------------------------------------------------------------------------

K = thermal.PartitionMatrix(thermal.GetProblem().Stiffness())
y = thermal.PartitionVector(thermal.GetProblem().Energy())

#monitor = solvers.IterativeCG(K[3], y[1], -K[2] * y[0])
monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0])

thermal.UpdateMeshValues(y)

#plots.residual.Show(monitor)
plots.HeatMapNorm(mesh.GetNodes())