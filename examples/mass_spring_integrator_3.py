import structural
import solvers
import plots.oscillator
import math

def Harmonic(t: float) -> float:
    amplitude = 1.0
    omega = 1.0

    #return amplitude
    return amplitude * math.cos(omega * t)

T_ref   = 313.15      #[K]      = 40 [°C]
p_ref   = 101325.1    #[N/m²]   =  1 [atm]
mass = 1.0
stiffness = 100.0
damping = 0.5
omega = math.sqrt(stiffness / mass)
period = 2.0 * math.pi / omega
status  = 0
basis   = structural.CreateBasisCartesian(1)
timer   = structural.CreateTimerStepped(1, 0.0, 50.0 * period, 0.001)
time = []
position = []
velocity = []

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
force.SetScalar(0, structural.CreateValueScalar3DTimeFunction(Harmonic))

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
    global M
    global K
    global u
    global f
    
    u = structural.PartitionVector(structural.GetProblem().Displacement())
    f = structural.PartitionVector(structural.GetProblem().LoadNode(time))

    return [M[3], -(K[2] * u[0] + K[3] * u[1]) + f[1]]

def ODE2(time):
    global D
    global v

    return [D[3], v[1]]

while(timer.GetCurrent() < timer.GetEnd()):
    t.append(timer.GetCurrent())
    y.append(u[1][0])
    dydt.append(v[1][0])

    v[1] = solvers.ForwardMethod(timer, v[1], ODE1)
    u[1] = solvers.ForwardMethod(timer, u[1], ODE2)
    structural.UpdateMeshValues(u) 

    timer.SetNextStep()

plots.oscillator.Show(t, y)