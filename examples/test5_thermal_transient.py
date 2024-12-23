import dive
import materials
import meshes
import thermal

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = dive.CreateBasisCartesian(1)
timer       = dive.CreateTimerStationary(1, 0.0)
material    = materials.Fluids['Oil_ISO_VG68'](1, T_ref)
meshFile    = 'cavity.msh'
speed       = 0.01

def ApplyBoundaryConditions(problem, temperature1, temperature2):
    basis = dive.CreateBasisCartesian(1)
    nodes = problem.GetMesh().GetNodes()
    x = meshes.cavity.l
    y = meshes.cavity.h
    z = meshes.cavity.w

    nodesLeft = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, 0.0, 0.01)
    for node in nodesLeft:
        dirichlet = dive.CreateDirichletByValue(node, 0, temperature1)
        problem.AddDirichlet(dirichlet)

    nodesRight = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, x, 0.01)
    for node in nodesRight:
        dirichlet = dive.CreateDirichletByValue(node, 0, temperature2)
        problem.AddDirichlet(dirichlet)   

def FunctionVelocity(point):
    global speed

    res = dive.Matrix(3, 1, 0.0)
    res[0, 0] = speed
    return res

meshes.cavity.l = 2.0
meshes.cavity.h = 1.0
meshes.cavity.w = 0.1
meshes.cavity.nx = 20
meshes.cavity.ny = 20
meshes.cavity.nz = 2
meshes.cavity.quadratic = True
meshes.Create(meshFile)

meshThermal  = meshes.LoadMesh(1, meshFile)
temperature = dive.CreateValueScalar3D(T_ref)
pressure    = dive.CreateValueScalar3D(p_ref)

problem = thermal.solver.CreateProblemThermal(1, timer, meshThermal, pressure, None, material)
ApplyBoundaryConditions(problem, 100.0, 0.0)

thermal.solver.Initialize()
thermal.solver.SolverTransient()

meshes.plot.Field(meshThermal.GetNodes())

"""
def FunctionVelocity(x, y, z):
    return [0.0, 0.0, 0.0]

def FunctionThermal(x, y, z):
    x0 = 0.5
    y0 = 0.5
    sx = 0.2
    sy = 0.2
    A  = 10.0 

    return [A * math.exp(-((x - x0)**2.0 / (2.0 * sx**2.0) + (y - y0)**2.0 / (2.0 * sy**2.0)))]
"""