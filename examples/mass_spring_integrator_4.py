import os
os.add_dll_directory(r"C:\devel\dive\examples")

import structural
import solvers
import plots.oscillator
import math
import time

mass = 1.0
stiffness = 1.0
damping = 0.5
status  = 0

basis   = structural.CreateBasisCartesian(1)
timer   = structural.CreateTimerStepped(1, 0.0, 100.0, 0.01)
current = []
position = []
velocity = []

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

body = structural.CreateElementMass(3)
body.SetNode(0, node2)
body.SetMass(structural.CreateValueScalar(mass))

mesh = structural.CreateMesh(1)
mesh.AddNode(node1, status, True)
mesh.AddNode(node2, status, True)
mesh.AddNode(node3, status, True)
mesh.AddElement(spring1, status)
mesh.AddElement(spring2, status)
mesh.AddElement(body, status)

force = structural.CreateValueVector3DScalars(3)

structural.CreateProblem(1, mesh, temperature, pressure)
structural.ApplyDirichlet([node1], 0.0)
structural.ApplyDirichlet([node2], 0.0, dof = 2)
structural.ApplyDirichlet([node3], 0.0)
structural.ApplyLoadNode([node2], force)
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
f = structural.PartitionVector(structural.GetProblem().LoadNode())

print(K[3])
quit()

def ODE1(t, u, v):
    global M
    global C
    global K
    global f
  
    force.SetScalar(1, structural.CreateValueScalar3D(-10.0))
    f = structural.PartitionVector(structural.GetProblem().LoadNode())
    K = structural.PartitionMatrix(structural.GetProblem().Stiffness())

 
    return [M[3], -(C[3]*v + K[3]*u) + f[1]]

def ODE2(time, v):
    global D

    return [D[3], v]

while(timer.GetCurrent() < timer.GetEnd()):
    current.append(timer.GetCurrent())
    position.append(u[1][1])
    velocity.append(v[1][1])

    u[1], v[1] = solvers.ForwardMethod2(timer, u[1], v[1], ODE1, ODE2)

    structural.UpdateMeshValues(u)
    timer.SetNextStep()

plots.oscillator.Show(current, position, velocity)