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
node2 = structural.CreateNode(2, 0.0, 0.0, 1.0)

spring = structural.CreateElementCombined(1)
spring.SetNode(0, node1)
spring.SetNode(1, node2)
spring.SetStiffness(structural.CreateValueScalar(stiffness))
spring.SetDamping(structural.CreateValueScalar(damping))

body = structural.CreateElementMass(2)
body.SetNode(0, node2)
body.SetMass(structural.CreateValueScalar(mass))

mesh = structural.CreateMesh(1)
mesh.AddNode(node1, status, True)
mesh.AddNode(node2, status, True)
mesh.AddElement(spring, status)
mesh.AddElement(body, status)

structural.CreateProblem(1, mesh, temperature, pressure)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

totalDof = structural.GetProblem().GetTotalDof()
pivot = structural.GetProblem().GetPivot()

#M = structural.PartitionMatrix(structural.GetProblem().Mass())
#K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
#C = structural.PartitionMatrix(structural.GetProblem().Damping())

print(totalDof)
print(pivot)
#print(structural.GetProblem().Mass())
print(structural.GetProblem().Stiffness())
quit()