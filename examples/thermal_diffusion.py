import meshes
import thermal
import solvers
import plots
import materials.fluid.VG46
import matplotlib.pyplot as plt

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.fluid.VG46.Create(1, T_ref, p_ref)

#meshes.CreateCavity(1.0, 1.0, 0.1, 101, 101, 2, False)
thermal.GmshInitialize()
thermal.GmshOpenFile("cavity.msh")

cavity = thermal.GmshGetMeshForPhysicalGroup(1, 1, "cavity")
wall = thermal.GmshGetNodesForPhysicalGroup(cavity, "wall")
hot = thermal.GmshGetNodesForPhysicalGroup(cavity, "hot")
plot = thermal.GmshGetNodesForPhysicalGroup(cavity, "plot")

meshes.ApplyMaterial(cavity.GetElements(), material)
thermal.GmshFinalize()

thermal.CreateProblem(1, cavity, pressure)
thermal.ApplyDirichlet(hot, 100.0)
thermal.ApplyDirichlet(wall, 0.0)
thermal.Initialize()

#--------------------------------------------------------------------------------------------------

totalDof = thermal.GetProblem().GetTotalDof()
pivot = thermal.GetProblem().GetPivot()

K = thermal.GetProblem().Stiffness()

K = thermal.PartitionMatrix(thermal.GetProblem().Stiffness())
y = thermal.PartitionVector(thermal.GetProblem().Energy())

monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0])
thermal.UpdateMeshValues(y)
y = thermal.PartitionVector(thermal.GetProblem().Energy())

#plots.residual.Show(monitor)
plots.HeatMapNorm(plot)

""""
rows = K.GetRows()
cols = K.GetCols()
width = K.GetWidth()
count = K.GetCount()
numberDof = thermal.GetProblem().GetNumberDof()
numberNodes = len(cavity.GetNodes())
numberElements = len(cavity.GetElements())
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
quit()
"""