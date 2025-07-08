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
position1 = []
position2 = []
velocity1 = []
velocity2 = []

T_ref   = 313.15      #[K]      = 40 [°C]
p_ref   = 101325.1    #[N/m²]   =  1 [atm]
temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)

node1 = structural.CreateNode(1, 0.0, 0.0, 0.0)
node2 = structural.CreateNode(2, 1.0, 0.0, 0.0)
node3 = structural.CreateNode(3, 2.0, 0.0, 0.0)

spring1 = structural.CreateElementCombined(1)
spring1.SetNode(0, node1)
spring1.SetNode(1, node2)
spring1.SetStiffness(structural.CreateValueScalar(stiffness))
spring1.SetDamping(structural.CreateValueScalar(damping))

spring2 = structural.CreateElementCombined(2)
spring2.SetNode(0, node2)
spring2.SetNode(1, node3)
spring2.SetStiffness(structural.CreateValueScalar(stiffness))
spring2.SetDamping(structural.CreateValueScalar(damping))

body1 = structural.CreateElementMass(3)
body1.SetNode(0, node2)
body1.SetMass(structural.CreateValueScalar(mass))

body2 = structural.CreateElementMass(4)
body2.SetNode(0, node3)
body2.SetMass(structural.CreateValueScalar(5 * mass))

mesh = structural.CreateMesh(1)
mesh.AddNode(node1, status, True)
mesh.AddNode(node2, status, True)
mesh.AddNode(node3, status, True)
mesh.AddElement(spring1, status)
mesh.AddElement(spring2, status)
mesh.AddElement(body1, status)
mesh.AddElement(body2, status)

force = structural.CreateValueVector3DScalarsTime(3)
force.SetScalar(0, structural.CreateValueScalar3DTimeFunction(Harmonic))

structural.CreateProblem(1, mesh, temperature, pressure)
structural.ApplyDirichlet([node1], 0.0, dof = 0)
structural.ApplyDirichlet([node1], 0.0, dof = 1)
structural.ApplyDirichlet([node1], 0.0, dof = 2)

structural.ApplyDirichlet([node2], 0.0, dof = 1)
structural.ApplyDirichlet([node2], 0.0, dof = 2)

structural.ApplyDirichlet([node3], 0.0, dof = 1)
structural.ApplyDirichlet([node3], 0.0, dof = 2)
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

dt = timer.GetStepSize()
dvdt = structural.Vector(v[1])
dudt = structural.Vector(u[1])
permutation = structural.CreateIndices()

while(timer.GetCurrent() < timer.GetEnd()):
    time.append(timer.GetCurrent())
    position1.append(u[1][0])
    position2.append(u[1][1])
    velocity1.append(v[1][0])
    velocity2.append(v[1][1])

    a = structural.Vector(totalDof, 10.0)
    f = structural.PartitionVector(a)

    #f[1][0] = Harmonic(timer.GetCurrent(), 0, 0, 0)

    #structural.DecomposeLUP(LU, MD, permutation)
    #structural.DirectLUP(LU, dvdt, -(CD*v[1] + KD*u[1]) + f[1], permutation)
    #monitor = solvers.IterativeBiCGStab(M[3], dvdt, -(C[3]*v[1] + K[3]*u[1]) + f[1])
    monitor = solvers.IterativeCG(M[3], dvdt, -(C[3]*v[1] + K[3]*u[1]) + f[1])
    v[1] = v[1] + dt * dvdt
     
    #structural.DecomposeLUP(LU, DD, permutation)
    #structural.DirectLUP(LU, dudt, v[1], permutation)     
    #monitor = solvers.IterativeBiCGStab(D[3], dudt, v[1])
    monitor = solvers.IterativeCG(D[3], dudt, v[1])
    u[1] = u[1] + dt * dudt

    structural.UpdateMeshValues(u)
    
    timer.SetNextStep()

plots.oscillator.Show(time, position1, velocity1)
plots.oscillator.Show(time, position2, velocity2)