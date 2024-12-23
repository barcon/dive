import dive
import materials
import meshes
import fluid

from prettytable import PrettyTable

import meshes.plot_cavity

def ApplyBoundaryConditionsVelocity(problem, value):
    global heightElement
    tolerance = heightElement / 5.0

    x = meshes.cavity.x
    y = meshes.cavity.y
    z = meshes.cavity.z

    nodes = problem.GetMesh().GetNodes()

    nodesTop = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_y, y, tolerance)
    for node in nodesTop:
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 0, value))   
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 1, 0.0))   
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 2, 0.0))

    nodesLeft = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, 0.0, tolerance)
    for node in nodesLeft:
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 0, 0.0))   
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 1, 0.0))   
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 2, 0.0))          

    nodesRight = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, x, tolerance)
    for node in nodesRight:
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 0, 0.0))   
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 1, 0.0))   
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 2, 0.0))     

    nodesBottom = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_y, 0.0, tolerance)
    for node in nodesBottom:
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 0, 0.0))   
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 1, 0.0))   
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 2, 0.0))         

    nodesFront = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_z, z, tolerance)
    for node in nodesFront:
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 2, 0.0))          

    nodesBack = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_z, 0.0, tolerance)
    for node in nodesBack:
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 2, 0.0)) 

def ApplyBoundaryConditionsPressure(problem, value):
    global heightElement
    tolerance = heightElement / 5.0

    x = meshes.cavity.x
    y = meshes.cavity.y
    z = meshes.cavity.z

    nodes = problem.GetMesh().GetNodes()

    nodesCorner = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, 0, tolerance)
    nodesCorner = dive.FilterNodesByCoordinate(nodesCorner, basis, dive.axis_y, 0, tolerance)
    for node in nodesCorner:
        problem.AddDirichlet(dive.CreateDirichletByValue(node, 0, value))        

    return     

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = dive.CreateBasisCartesian(1)
timer       = dive.CreateTimerStepped(1, 0.0, 10.0, 0.1)
material    = materials.Fluids['Oil_ISO_VG68'](1, T_ref)
meshFile    = 'cavity.msh'
speed       = 0.0
status      = None

meshes.cavity.x = 1.0
meshes.cavity.y = 1.0
meshes.cavity.z = 0.1
meshes.cavity.nx = 21
meshes.cavity.ny = 21
meshes.cavity.nz = 2
#meshes.cavity.quadratic = True
meshes.cavity.quadratic = False
meshes.Create(meshFile)

meshPressure = meshes.LoadMesh(1, meshFile)
meshVelocity = meshes.LoadMesh(2, meshFile)

temperature = dive.CreateValueScalar3D(T_ref)
pressure = dive.CreateValueScalar3DCongruent(meshPressure)
velocity = dive.CreateValueMatrix3DCongruent(meshVelocity)

rho = material.GetDensity(T_ref, p_ref)
mu = material.GetDynamicViscosity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)

heightElement = meshVelocity.GetElementHeightMinium()
lenghtDomain = meshes.cavity.x
diffusity = mu / (cp * rho)
reynolds = 400.0
speed =  reynolds * mu / (rho * lenghtDomain)

dt1 = lenghtDomain**2.0 / diffusity
dt2 = heightElement**2.0 /  diffusity
dt3 = lenghtDomain / (speed)
dt4 = heightElement / (speed)

tableSummary = PrettyTable()
tableSummary.field_names = ["Property", "Value", "Unit"]
tableSummary.align["Property"] = "l"
tableSummary.align["Unit"] = "l"
tableSummary.add_row(["Density", "{:.2f}".format(rho), ""])
tableSummary.add_row(["Viscosity", "{:.2e}".format(mu), ""])
tableSummary.add_row(["Diffusity", "{:.2e}".format(diffusity), ""])
tableSummary.add_row(["Speed", "{:.2g}".format(speed), ""])
tableSummary.add_row(["Reynolds", "{:.2e}".format(reynolds), "[-]"])
tableSummary.add_row(["Domain Lenght", "{:.2f}".format(lenghtDomain), ""])
tableSummary.add_row(["Element Height", "{:.2e}".format(heightElement), ""])
tableSummary.add_row(["Diffusion Domain Time", "{:.2g}".format(dt1), "[s]"])
tableSummary.add_row(["Diffusion Element Time", "{:.2g}".format(dt2), "[s]"])
tableSummary.add_row(["Convection Domain Time", "{:.2g}".format(dt3), "[s]"])
tableSummary.add_row(["Convection Element Time", "{:.2g}".format(dt4), "[s]"])
print(tableSummary)

problemPressure = fluid.solver.CreateProblemPressure(1, timer, meshPressure, temperature, velocity, material)
problemVelocity = fluid.solver.CreateProblemVelocity(2, timer, meshVelocity, temperature, pressure, material)

ApplyBoundaryConditionsVelocity(problemVelocity, speed)
ApplyBoundaryConditionsPressure(problemPressure, p_ref)

fluid.solver.InitializeVelocity()
fluid.solver.InitializePressure()

#quit()
while (timer.GetCurrentTime() != timer.GetEndTime()): 
    #q = fluid.solver.velocity.problem.Momentum()
    #dq = fluid.solver.TransientDiffusion()

    p = fluid.solver.pressure.problem.Pressure()    
    dp = fluid.solver.TransientPressure() 

    #dqq = fluid.solver.TransientCorrection()
    
    fluid.solver.pressure.problem.UpdateMeshValues(p + dp)
    #fluid.solver.velocity.problem.UpdateMeshValuesMomentum(q + dq + dqq)

    timer.SetNextStep()

#meshes.plot.FieldVector(meshVelocity.GetNodes())
#meshes.plot.FieldVectorNorm(meshVelocity.GetNodes())
#meshes.plot.Field(meshVelocity.GetNodes(), 0)
#meshes.plot.Field(meshVelocity.GetNodes(), 1)
meshes.plot.Field(meshPressure.GetNodes())

#nodes = problemVelocity.GetMesh().GetNodes()
#nodesCenter = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, meshes.cavity.x / 2.0, 0.001)
#nodesCenter = dive.FilterNodesByCoordinate(nodesCenter, basis, dive.axis_z, 0.0, 0.05)
#meshes.plot_cavity.HorizontalVelocity(nodesCenter, speed)
#print(nodesCenter)

#nodes = problemPressure.GetMesh().GetNodes()
#nodesCenter = dive.FilterNodesByCoordinate(nodes, basis, dive.axis_x, meshes.cavity.x / 2.0, 0.001)
#nodesCenter = dive.FilterNodesByCoordinate(nodesCenter, basis, dive.axis_z, 0.0, 0.05)
#print(nodesCenter)

#print(meshPressure.GetNodes())
#print(meshVelocity.GetNodes())
#print(meshVelocity.GetElements())
#print(problemVelocity.GetDofMeshIndices())
#print(problemVelocity.GetNodeMeshIndices())
#print(problemVelocity.GetDirichletMeshIndices())