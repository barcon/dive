import dive
import meshes
import materials
import thermal

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = dive.CreateBasisCartesian(1)
timer       = dive.CreateTimerStationary(1, 0.0)
material    = materials.Fluids['Unity'](1)
meshFile    = 'cube.msh'
status      = 0
mesh        = meshes.LoadMesh(1, meshFile)

temperature = dive.CreateValueScalar3D(T_ref)
pressure    = dive.CreateValueScalar3D(p_ref)
problem     = thermal.solver.CreateProblemThermal(1, timer, mesh, pressure, None, material)

thermal.solver.Initialize()
point = dive.Vector(3)

print(problem.Mass())
print(problem.Stiffness())
print(mesh.GetElements())
print(mesh.GetElements()[0].u(point))
print(mesh.GetElements()[0].du(point))
print(mesh.GetElements()[0].N(point))
print(mesh.GetElements()[0].dN(point))