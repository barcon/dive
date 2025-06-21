import structural
import solvers
import plots
import math

def Harmonic(t, x, y, z):
    omega = 1.0
    amplitude = 20.0

    return amplitude * math.cos(omega * t)

def Zero(t, x, y, z):
    return 0.0

T_ref   = 313.15      #[K]      = 40 [°C]
p_ref   = 101325.1    #[N/m²]   =  1 [atm]
basis   = structural.CreateBasisCartesian(1)
timer   = structural.CreateTimerStepped(1, 0.0, 10.0, 0.01)
status  = 0

temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)

node1 = structural.CreateNode(1, 0.0, 0.0, 0.0)
node2 = structural.CreateNode(2, 1.0, 0.0, 0.0)

spring = structural.CreateElementSpring(1)
spring.SetNode(0, node1)
spring.SetNode(1, node2)
spring.SetStiffness(structural.CreateValueScalar(100.0))

mass = structural.CreateElementMass(3)
mass.SetNode(0, node2)
mass.SetMass(structural.CreateValueScalar(1.0))

mesh = structural.CreateMesh(1)
mesh.AddNode(node1, status, True)
mesh.AddNode(node2, status, True)
mesh.AddElement(spring, status)
mesh.AddElement(mass, status)

force = structural.CreateValueVector3DScalarsTime(3)
force.SetScalar(0, structural.CreateValueScalar3DFunctionTime(Harmonic))
force.SetScalar(1, structural.CreateValueScalar3DFunctionTime(Zero))
force.SetScalar(2, structural.CreateValueScalar3DFunctionTime(Zero))

structural.CreateProblem(1, timer, mesh, temperature, pressure)
structural.ApplyDirichlet([node1], 0.0)
structural.ApplyDirichlet([node2], 0.0, dof = 1)
structural.ApplyDirichlet([node2], 0.0, dof = 2)
structural.ApplyLoadNode([node2], force)
structural.Initialize()

#--------------------------------------------------------------------------------------------------
quit()
M = structural.PartitionMatrix(structural.GetProblem().Mass())
K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
f = structural.PartitionVector(structural.GetProblem().LoadNode())
u = structural.PartitionVector(structural.GetProblem().Displacement())
v = structural.PartitionVector(structural.GetProblem().Velocity())

def ODE1():
    global M
    global K
    global u
    global f

    return [M[3], -(K[2] * u[0] + K[3] * u[1]) + f[1]]

def ODE2():
    global v

    totalDof = structural.GetProblem().GetTotalDof()
    pivot = structural.GetProblem().GetPivot()
    
    D = structural.PartitionMatrix(structural.Ellpack(totalDof, totalDof, 1.0).Diagonal())

    return [D[3], v[1]]

while(timer.GetCurrent() != timer.GetEnd()):

    print("{:.3f}".format(timer.GetCurrent()), "{:.3f}".format(u[1](0)))

    v[1] = solvers.ForwardMethod(timer, v[1], ODE1)
    u[1] = solvers.ForwardMethod(timer, u[1], ODE2)
    structural.UpdateMeshValues(u)    
    
    timer.SetNextStep()
