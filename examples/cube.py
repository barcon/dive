import materials.solid.unit
import structural
import meshes

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = structural.CreateBasisCartesian(1)
timer       = structural.CreateTimerStationary(1, 0.0)
temperature = structural.CreateValueScalar3D(T_ref)
pressure    = structural.CreateValueScalar3D(p_ref)

material = materials.solid.unit.Create(1)
meshFile = 'cube.msh'

meshes.cube.Create(meshFile)
mesh = meshes.routines.LoadMesh(1, meshFile)
meshes.routines.ApplyMaterial(mesh.GetElements(), material)

structural.CreateProblem(1, mesh, temperature, pressure)
structural.Initialize()

print(structural.GetProblem().Mass())
print(structural.GetProblem().Stiffness())
