import os
os.add_dll_directory(r"C:\devel\dive\examples")

import structural
import solvers
import plots.oscillator
import math

def Harmonic(t: float, x: float, y: float, z: float) -> float:
    amplitude = 10.0
    omega = 1.0

    #return amplitude
    return amplitude * math.cos(omega * t)

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

spring = structural.CreateElementCombined(1)
spring.SetNode(0, node1)
spring.SetNode(1, node2)
spring.SetStiffness(structural.CreateValueScalar(stiffness))
spring.SetDamping(structural.CreateValueScalar(damping))

body = structural.CreateElementMass(3)
body.SetNode(0, node2)
body.SetMass(structural.CreateValueScalar(mass))

mesh = structural.CreateMesh(1)
mesh.AddNode(node1, status, True)
mesh.AddNode(node2, status, True)
mesh.AddElement(spring, status)
mesh.AddElement(body, status)

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

D = structural.PartitionMatrix(structural.Ellpack(totalDof, totalDof, structural.matrix_diagonal))
M = structural.PartitionMatrix(structural.GetProblem().Mass())
K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
C = structural.PartitionMatrix(structural.GetProblem().Damping())
u = structural.PartitionVector(structural.GetProblem().Displacement())
v = structural.PartitionVector(structural.Vector(totalDof, 0.0))
f = structural.PartitionVector(structural.GetProblem().LoadNode(timer.GetCurrent()))

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

MD = structural.Matrix(M[3])
KD = structural.Matrix(K[3])
CD = structural.Matrix(C[3])
DD = structural.Matrix(D[3])
LU = structural.Matrix()

permutation = structural.CreateIndices()
dt = timer.GetStepSize()
dvdt = structural.Vector(v[1])
dudt = structural.Vector(u[1])

while(timer.GetCurrent() < timer.GetEnd()):
    time.append(timer.GetCurrent())
    position.append(u[1][0])
    velocity.append(v[1][0])
 
    #f = structural.PartitionVector(structural.GetProblem().LoadNode(timer.GetCurrent()))
    f[1][0] = Harmonic(timer.GetCurrent(), 0, 0, 0)

    #structural.DecomposeLUP(LU, MD, permutation)
    #structural.DirectLUP(LU, dvdt, -(CD*v[1] + KD*u[1]) + f[1], permutation)
    dvdt.SetValue(0, 0.0)
    monitor = solvers.IterativeBiCGStab(M[3], dvdt, -(C[3]*v[1] + K[3]*u[1]) + f[1])
    v[1] = v[1] + dt * dvdt

    #print(-(C[3]*v[1] + K[3]*u[1]) + f[1])
     
    #structural.DecomposeLUP(LU, DD, permutation)
    #structural.DirectLUP(LU, dudt, v[1], permutation)     
    dudt.SetValue(0, 0.0)
    monitor = solvers.IterativeBiCGStab(D[3], dudt, v[1])
    u[1] = u[1] + dt * dudt

    structural.UpdateMeshValues(u)
    timer.SetNextStep()

plots.oscillator.Show(time, position, velocity)