import dive
import materials
import meshes
import thermal

def ApplyBoundaryConditions(problem, temperature1, temperature2):
    nodes = problem.GetMesh().GetNodes()
    l = meshes.cavity.l
    h = meshes.cavity.h
    w = meshes.cavity.w

    nodesLeft = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, 0.0, 0.01)
    for node in nodesLeft:
        dirichlet = dive.CreateDirichletByValue(node, 0, temperature1)
        problem.AddDirichlet(dirichlet)

    nodesRight = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, l, 0.01)
    for node in nodesRight:
        dirichlet = dive.CreateDirichletByValue(node, 0, temperature2)
        problem.AddDirichlet(dirichlet)   

def FunctionVelocity(point):
    global speed

    res = dive.Matrix(3, 1, 0.0)
    res[0, 0] = speed
    return res

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = dive.CreateBasisCartesian(1)
timer       = dive.CreateTimerStationary(1, 0.0)
material    = materials.Fluids['Oil_ISO_VG68'](1, T_ref)
meshFile    = 'cavity.msh'
speed       = 0.0

meshes.cavity.l = 1.0
meshes.cavity.h = 0.1
meshes.cavity.w = 0.1
meshes.cavity.nx = 21
meshes.cavity.ny = 2
meshes.cavity.nz = 2
meshes.cavity.quadratic = True
meshes.Create(meshFile)

meshThermal = meshes.LoadMesh(1, meshFile)
meshVelocity= meshes.LoadMesh(2, meshFile)

pressure = dive.CreateValueScalar3D(p_ref)
velocity = dive.CreateValueMatrix3DCongruent(meshVelocity)

rho = material.GetDensity(T_ref, p_ref)
h = meshThermal.GetElementHeightMinium()
k = material.GetThermalConductivity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)
D = k / (cp * rho)
Pe = 3.0
speed = (2 * Pe * D) / h
lenght = meshes.cavity.l

dt1 = lenght**2.0 / D
dt2 = h**2.0 /  D
dt3 = lenght / (speed)
dt4 = h / (speed)

print("Speed = ",  speed)
print("Domain Lenght = ", lenght)
print("Element Lenght = ", h)
print("Diffusity = ", D)
print("Peclet = ", Pe)

print("Characteristic Time (Diffusion Domain)  = ", "{:.5f}".format(dt1))
print("Characteristic Time (Diffusion Element) = ", "{:.5f}".format(dt2))
print("Characteristic Time (Convection Domain)  = ", "{:.5f}".format(dt3))
print("Characteristic Time (Convection Element) = ", "{:.5f}".format(dt4))

meshes.routines.ApplyField(meshVelocity, dof = 3, function = FunctionVelocity)

problem = thermal.solver.CreateProblemThermal(1, timer, meshThermal, pressure, velocity, material)
ApplyBoundaryConditions(problem, 1.0, 0.0)

thermal.solver.Initialize()
thermal.solver.SolverStationaryConvection()
#thermal.solver.SolverStationaryStabilized()

nodesPlot = dive.FilterNodesByCoordinate(meshThermal.GetNodes(), basis, dive.axis_y, 0.0, 0.01)
nodesPlot = dive.FilterNodesByCoordinate(nodesPlot, basis, dive.axis_z, 0.0, 0.01)
meshes.plot.AddCurve(nodesPlot)
meshes.plot.plt.show()