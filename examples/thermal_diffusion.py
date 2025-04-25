import meshes
import thermal
import solvers
import materials.fluid.VG46
import plots

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.fluid.VG46.Create(1, T_ref, p_ref)
meshFile    = 'cavity.msh'

meshes.cavity.quadratic = True
meshes.cavity.Create(meshFile)
mesh = meshes.routines.LoadMesh(1, meshFile)
meshes.routines.ApplyMaterial(mesh.GetElements(), material)
#--------------------------------------------------------------------------------------------------

nodesTop = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_y, meshes.cavity.y, 0.001)
nodesBottom = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_y, 0.0, 0.001)
nodesLeft = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, 0.0, 0.001)
nodesRight = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, meshes.cavity.x, 0.001)

thermal.CreateProblem(1, timer, mesh, pressure, None)
thermal.ApplyDirichlet(nodesTop, 100.0)
thermal.ApplyDirichlet(nodesBottom, 0.0)
thermal.ApplyDirichlet(nodesLeft, 0.0)
thermal.ApplyDirichlet(nodesRight, 100.0)
thermal.Initialize()
#--------------------------------------------------------------------------------------------------

K = thermal.Stiffness()
y = thermal.Energy()

monitor = solvers.Iterative(K[1], y[1], -K[0] * y[0])

thermal.UpdateMeshValues(y)

#plots.residual.HeatMapNorm(monitor)
plots.HeatMapNorm(mesh.GetNodes())