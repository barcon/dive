import dive
import materials
import meshes
import thermal

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = dive.CreateBasisCartesian(1)
timer       = dive.CreateTimerStationary(1, 0.0)
material    = materials.CreateFluidOil(1, 68, T_ref)
meshFile    = 'cavity.msh'

def ApplyBoundaryConditions(problem, temperature1, temperature2):
    basis = dive.CreateBasisCartesian(1)
    nodes = problem.GetMesh().GetNodes()
    
    l = meshes.cavity.x
    h = meshes.cavity.y
    w = meshes.cavity.z

    nodesTop = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_y, h, 0.001)
    for node in nodesTop:
        dirichlet = dive.CreateDirichletByValue(node, 0, temperature1)
        problem.AddDirichlet(dirichlet)

    nodesBottom = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_y, 0.0, 0.001)
    for node in nodesBottom:
        dirichlet = dive.CreateDirichletByValue(node, 0, temperature2)
        problem.AddDirichlet(dirichlet)

    nodesLeft = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, 0.0, 0.001)
    for node in nodesLeft:
        dirichlet = dive.CreateDirichletByValue(node, 0, temperature2)
        problem.AddDirichlet(dirichlet)

    nodesRight = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, l, 0.001)
    for node in nodesRight:
        dirichlet = dive.CreateDirichletByValue(node, 0, temperature2)
        problem.AddDirichlet(dirichlet)   

def FunctionVelocity(point):
    global speed

    res = dive.Matrix(3, 1, 0.0)
    res[0, 0] = speed
    return res

meshes.cavity.quadratic = True
meshes.Create(meshFile)

mesh     = meshes.LoadMesh(1, meshFile)
pressure = dive.CreateValueScalar3D(p_ref)
problem  = thermal.CreateProblemThermal(1, timer, mesh, pressure, None, material)

quit()
ApplyBoundaryConditions(problem, 100.0, 0.0)

thermal.solver.Initialize()

thermal.solver.Start()
meshes.plot.Field(mesh.GetNodes())