import meshes
import thermal
import materials.fluid
import plots.residual
import plots.field

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.fluid.CreateFluidOil(1, 68, T_ref)
meshFile    = 'cavity.msh'

meshes.cavity.quadratic = False
meshes.cavity.Create(meshFile)
mesh = meshes.routines.LoadMesh(1, meshFile)

quit()

thermal.CreateProblem(1, timer, mesh, pressure, None, material)
thermal.ApplyBoundaryConditions(100.0, 0.0)
thermal.Initialize()

y0, monitor = thermal.Diffusion()

thermal.UpdateMeshValues(y0)

plots.residual.Show(monitor)
plots.field.Show(mesh.GetNodes())