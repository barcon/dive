import structural
import solvers
import plots
import math

def Force0(x, y, z):
    global timer
    t = timer.GetCurrentTime() 
    omega = 1.0
    amplitude = 20.0

    return amplitude * math.sin(omega * t)

def Force1(x, y, z):
    return 0.0

def Force2(x, y, z):
    return 0.0

T_ref   = 313.15      #[K]      = 40 [°C]
p_ref   = 101325.1    #[N/m²]   =  1 [atm]
basis   = structural.CreateBasisCartesian(1)
timer   = structural.CreateTimerStepped(1, 0.0, 10.0, 0.01)
status  = 0

temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)

node1 = structural.CreateNode(1, 0.0, 0.0, 0.0)
node2 = structural.CreateNode(2, 100.0, 0.0, 0.0)

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

F0 = structural.CreateValueScalar3DFunction(Force0)
F1 = structural.CreateValueScalar3DFunction(Force1)
F2 = structural.CreateValueScalar3DFunction(Force2)

force = structural.CreateValueVector3DFunction([F0, F1, F2])
quit()

structural.CreateProblem(1, timer, mesh, temperature, pressure)
structural.ApplyDirichlet([node1], 0.0)
structural.ApplyDirichlet([node2], 0.0, dof = 1)
structural.ApplyDirichlet([node2], 0.0, dof = 2)
structural.ApplyDirichlet([node3], 0.0)
structural.ApplyLoadNode([node2], force)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
y = structural.PartitionVector(structural.GetProblem().Displacement())
f = structural.PartitionVector(structural.GetProblem().LoadNode())

#monitor = solvers.IterativeCG(K[3], y[1], -K[2] * y[0] + f[1])
monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0] + f[1])

structural.UpdateMeshValues(y)

print(y[1])