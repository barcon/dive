import os
os.add_dll_directory(r"C:\devel\dive\examples")

import structural
import solvers
import plots.oscillator
import math

def Harmonic(t: float, x: float, y: float, z: float) -> float:
    amplitude = 10.0
    omega = 1.0

    return amplitude
#    return amplitude * math.cos(omega * t)

mass = 1.0
stiffness = 100.0
damping = 0.5
omega = math.sqrt(stiffness / mass)
period = 2.0 * math.pi / omega
status  = 0

basis   = structural.CreateBasisCartesian(1)
timer   = structural.CreateTimerStepped(1, 0.0, 50.0 * period, 0.01)
time = []
position = []
velocity = []

T_ref   = 313.15      #[K]      = 40 [°C]
p_ref   = 101325.1    #[N/m²]   =  1 [atm]
temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)

node1 = structural.CreateNode(1, 0.0, 0.0, 0.0)
node2 = structural.CreateNode(2, 1.0, 0.0, 0.0)

spring = structural.CreateElementSpring(1)
spring.SetNode(0, node1)
spring.SetNode(1, node2)
spring.SetStiffness(structural.CreateValueScalar(stiffness))

body = structural.CreateElementMass(3)
body.SetNode(0, node2)
body.SetMass(structural.CreateValueScalar(mass))

mesh = structural.CreateMesh(1)
mesh.AddNode(node1, status, True)
mesh.AddNode(node2, status, True)
mesh.AddElement(spring, status)
mesh.AddElement(body, status)

forceScalar = structural.CreateValueScalar3DTimeFunction(Harmonic)
force = structural.CreateValueVector3DScalarsTime(3)
force.SetScalar(0, forceScalar)
#force.SetScalar(0, structural.CreateValueScalar3DTime(10.0))

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
C = structural.PartitionMatrix(structural.Ellpack(totalDof, totalDof))
K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
u = structural.PartitionVector(structural.GetProblem().Displacement())
v = structural.PartitionVector(structural.Vector(totalDof, 0.0))
f = structural.PartitionVector(structural.GetProblem().LoadNode(timer.GetCurrent()))

C[3][0, 0] = damping

def ODE1(time, u, v):
    global M
    global C
    global K
    global f
  
    #print(body.GetNode(0).GetPoint())

    f[1][0] = forceScalar.GetValue(time, body.GetNode(0).GetPoint())
    print(f[1][0])
    #f[1][0] = scalar1.GetValue(time, body.GetNode(0).GetPoint())
    #f[1][0] = scalar2.GetValue(time, body.GetNode(0).GetPoint())
    #f[1][0] = Harmonic(time, 0, 0, 0)
    #f[1][0] = 10.0
    #f = structural.PartitionVector(structural.GetProblem().LoadNode(time))

    return [M[3], -(C[3]*v + K[3]*u) + f[1]]

def ODE2(time, v):
    global D

    return [D[3], v]

M_Dense = structural.Matrix(1, 1)
M_Dense[0, 0] = M[3][0, 0]

D_Dense = structural.Matrix(1, 1)
D_Dense[0, 0] = D[3][0, 0]


while(timer.GetCurrent() < timer.GetEnd()):
    time.append(timer.GetCurrent())
    position.append(u[1][0])
    velocity.append(v[1][0])

    dt = timer.GetStepSize()
    dvdt = structural.Vector(v[1])
    dudt = structural.Vector(u[1])

    f = structural.PartitionVector(structural.GetProblem().LoadNode(timer.GetCurrent()))
    #print(f[1])

    #monitor = solvers.IterativeBiCGStab(M[3], dvdt, -(C[3]*v[1] + K[3]*u[1]) + f[1])
    dvdt = structural.Solve(M_Dense, -(C[3]*v[1] + K[3]*u[1]) + f[1])
    v[1] = v[1] + dt * dvdt
     
    dudt = structural.Solve(D_Dense, v[1])
    u[1] = u[1] + dt * dudt

    structural.UpdateMeshValues(u)
    timer.SetNextStep()

plots.oscillator.Show(time, position, velocity)