import os
os.add_dll_directory(r"C:\devel\dive\examples")

import structural
import solvers
import plots.oscillator
import math
import time

mass = 1.0
stiffness = 100.0
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
node2 = structural.CreateNode(2, 1.0, 1.0, 1.0)
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
force.SetScalar(1, structural.CreateValueScalar3D(10.0))

structural.CreateProblem(1, mesh, temperature, pressure)
structural.ApplyDirichlet([node1], 0.0)
structural.ApplyDirichlet([node2], 0.0, dof = 0)
structural.ApplyDirichlet([node2], 0.0, dof = 2)
structural.ApplyDirichlet([node3], 0.0)
structural.ApplyLoadNode([node2], force)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

totalDof = structural.GetProblem().GetTotalDof()
pivot = structural.GetProblem().GetPivot()

M = structural.PartitionMatrix(structural.GetProblem().Mass())
K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
C = structural.PartitionMatrix(structural.GetProblem().Damping())
y = structural.PartitionVector(structural.GetProblem().Displacement())
f = structural.PartitionVector(structural.GetProblem().LoadNode())

monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0] + f[1])

#print(totalDof)
#print(pivot)
print(y[1])
#print(M[3])
#print(K[3])
#print(structural.GetProblem().Mass())
#output = structural.Matrix()
#spring1.Stiffness(output)
#print(output)
#print(structural.GetProblem().LoadNode())
#print(structural.GetProblem().Stiffness())
