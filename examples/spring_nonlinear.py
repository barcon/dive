import os
os.add_dll_directory(r"C:\devel\dive\examples")

import structural
import solvers
import plots.oscillator
import math
import time

def Stiffness():
    global node1
    global node2
    global stiffness

    pos1 = structural.Vector(node1.GetValue(), 0)
    pos2 = structural.Vector(node2.GetValue(), 0)
    delta = structural.NormP2(pos2 - pos1)

    if(delta < 1.0):
        return 10.0 * stiffness
    else:
        return stiffness

stiffness = 100.0
status  = 0

basis   = structural.CreateBasisCartesian(1)
timer   = structural.CreateTimerStepped(1, 0.0, 100.0, 0.01)
current = []
position = []

T_ref   = 313.15      #[K]      = 40 [°C]
p_ref   = 101325.1    #[N/m²]   =  1 [atm]
temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)

node1 = structural.CreateNode(1, 0.0, 0.0, 0.0)
node2 = structural.CreateNode(2, 1.0, 0.0, 0.0)

spring1 = structural.CreateElementCombined(1)
spring1.SetNode(0, node1)
spring1.SetNode(1, node2)
spring1.SetStiffness(structural.CreateValueScalarFunction(Stiffness))

mesh = structural.CreateMesh(1)
mesh.AddNode(node1, status, True)
mesh.AddNode(node2, status, True)
mesh.AddElement(spring1, status)

structural.CreateProblem(1, mesh, temperature, pressure)
structural.ApplyDirichlet([node1], 0.0)
structural.ApplyDirichlet([node2], 0.0, dof = 1)
structural.ApplyDirichlet([node2], 0.0, dof = 2)
structural.Initialize()

u = structural.PartitionVector(structural.GetProblem().Displacement())

u[1][0] = 0.1
structural.UpdateMeshValues(u)
K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
print(K[3])

u[1][0] = 1.0
structural.UpdateMeshValues(u)
K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
print(K[3])