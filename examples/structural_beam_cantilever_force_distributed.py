import meshes
import structural
import solvers
import plots
import plots.beam
import materials.solid.steel
import matplotlib.pyplot as plt

T_ref = 293.15      #[K]      = 20 [°C]
p_ref = 101325.1    #[N/m²]   =  1 [atm]
basis = structural.CreateBasisCartesian(1)
timer = structural.CreateTimerStationary(1, 0.0)
temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)
gravity = -9.8      #[m/s²]

steel = materials.solid.steel.Create(1)
density = steel.GetDensity(T_ref, p_ref)

#meshes.CreateBeam(1.0, 0.1, 0.1, 21, 5, 5, False)
structural.GmshInitialize()
structural.GmshOpenFile("beam.msh")

beam = structural.GmshGetMeshForPhysicalGroup(1, 3, "beam")
fixed = structural.GmshGetNodesForPhysicalGroup(beam, "fixed")
loadVolume = structural.GmshGetElementsForPhysicalGroup(beam, "loadVolume")
weight = structural.CreateValueVector3DScalars([0.0, density * gravity, 0.0])
plot = structural.GmshGetNodesForPhysicalGroup(beam, "plot")

meshes.ApplyMaterial(beam.GetElements(), steel)
structural.GmshFinalize()

structural.CreateProblem(1, beam, temperature, pressure)
structural.ApplyDirichlet(fixed, 0.0)
structural.ApplyLoadDistributedVolume(loadVolume, weight)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
y = structural.PartitionVector(structural.GetProblem().Displacement())
f = structural.PartitionVector(structural.GetProblem().LoadDistributedVolume())
monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0] + f[1])
structural.UpdateMeshValues(y)

#plots.residual.Show(monitor)
plots.beam.Cantilever(plot, 1)

"""

K = structural.GetProblem().Stiffness()

rows = K.GetRows()
cols = K.GetCols()
width = K.GetWidth()
count = K.GetCount()
numberDof = structural.GetProblem().GetNumberDof()
numberNodes = len(beam.GetNodes())
numberElements = len(beam.GetElements())
memory = width * rows * numberDof * 8 / (10**6)
sum = 0
distribution = []

for i in range(0, len(count)):
    sum += count[i]
    distribution.append(count[i])

sparsity = 1.0 - sum / (rows * cols)

print("Nodes = ", numberNodes)
print("Elements = ", numberElements)
print("Rows = ", rows)
print("Cols = ", cols)
print("Width = ", width)
print("Sparsity (%) = ", 100. * sparsity)
print("Memory (MB) = ", memory)

plt.plot(distribution)
plt.show()

"""