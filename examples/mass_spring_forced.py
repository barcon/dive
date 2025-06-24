import structural
import solvers
import plots.oscillator
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
timer   = structural.CreateTimerStepped(1, 0.0, 20.0, 0.01)
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

structural.CreateProblem(1, mesh, temperature, pressure)
structural.ApplyDirichlet([node1], 0.0)
structural.ApplyDirichlet([node2], 0.0, dof = 1)
structural.ApplyDirichlet([node2], 0.0, dof = 2)
structural.ApplyLoadNodeTransient([node2], force)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

totalDof = structural.GetProblem().GetTotalDof()
pivot = structural.GetProblem().GetPivot()

D = structural.PartitionMatrix(structural.Ellpack(totalDof, totalDof, 1.0).Diagonal())
M = structural.PartitionMatrix(structural.GetProblem().Mass())
K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
f = structural.PartitionVector(structural.GetProblem().LoadNode(timer.GetCurrent()))
u = structural.PartitionVector(structural.GetProblem().Displacement())
v = structural.PartitionVector(structural.Vector(totalDof, 0.0))

t = []
y = []
dydt = []

def ODE1(time):
    global D
    global M
    global K
    global u
    global v
    global f
    
    global t
    global y
    global dydt

    u = structural.PartitionVector(structural.GetProblem().Displacement())
    f = structural.PartitionVector(structural.GetProblem().LoadNode(time))

    f[1].SetValue(0, Harmonic(time, 0.0, 0.0, 0.0))

    #print(-(K[2] * u[0] + K[3] * u[1]))
    return [M[3], -(K[2] * u[0] + K[3] * u[1]) + f[1]]

def ODE2(time):
    global D
    global v

    #v[1][0] = Harmonic(time, 0.0, 0.0, 0.0)

    return [D[3], v[1]]

while(timer.GetCurrent() < timer.GetEnd()):
    #u[1] = solvers.ForwardMethod(timer, u[1], ODE2)
    
    t.append(timer.GetCurrent())
    y.append(u[1][0])
    dydt.append(v[1][0])

    #print("{:.2f}".format(timer.GetCurrent()), "{:.3f}".format(u[1](0)))

    v[1] = solvers.ForwardMethod(timer, v[1], ODE1)
    u[1] = solvers.ForwardMethod(timer, u[1], ODE2)
    structural.UpdateMeshValues(u) 

    timer.SetNextStep()

plots.oscillator.Show(t, y, y)