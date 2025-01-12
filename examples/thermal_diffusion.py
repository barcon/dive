import thermal
import meshes
import materials.fluid

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
material    = materials.fluid.CreateFluidOil(1, 68, T_ref)
meshFile    = 'cavity.msh'

def ApplyBoundaryConditions(problem, temperature1, temperature2):
    basis = thermal.CreateBasisCartesian(1)
    nodes = problem.GetMesh().GetNodes()
    
    l = meshes.cavity.x
    h = meshes.cavity.y
    w = meshes.cavity.z

    nodesTop = thermal.FilterNodesByCoordinate(nodes, basis, thermal.axis_y, h, 0.001)
    for node in nodesTop:
        dirichlet = thermal.CreateDirichletByValue(node, 0, temperature1)
        problem.AddDirichlet(dirichlet)

    nodesBottom = thermal.FilterNodesByCoordinate(nodes, basis, thermal.axis_y, 0.0, 0.001)
    for node in nodesBottom:
        dirichlet = thermal.CreateDirichletByValue(node, 0, temperature2)
        problem.AddDirichlet(dirichlet)

    nodesLeft = thermal.FilterNodesByCoordinate(nodes, basis, thermal.axis_x, 0.0, 0.001)
    for node in nodesLeft:
        dirichlet = thermal.CreateDirichletByValue(node, 0, temperature2)
        problem.AddDirichlet(dirichlet)

    nodesRight = thermal.FilterNodesByCoordinate(nodes, basis, thermal.axis_x, l, 0.001)
    for node in nodesRight:
        dirichlet = thermal.CreateDirichletByValue(node, 0, temperature2)
        problem.AddDirichlet(dirichlet)   

meshes.cavity.quadratic = False
meshes.cavity.Create(meshFile)

mesh = meshes.routines.LoadMesh(1, meshFile)
pressure = thermal.CreateValueScalar3D(p_ref)
problem  = thermal.CreateProblem(1, timer, mesh, pressure, None, material)

ApplyBoundaryConditions(problem, 100.0, 0.0)

thermal.Initialize()
y, status = thermal.SolverStationaryDiffusion()
