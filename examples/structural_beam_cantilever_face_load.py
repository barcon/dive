import meshes
import structural
import solvers
import plots
import plots.beam
import materials.solid.steel

T_ref = 293.15    #[K]      = 20 [°C]
p_ref = 101325.1  #[N/m²]   =  1 [atm]
basis = structural.CreateBasisCartesian(1)
timer = structural.CreateTimerStationary(1, 0.0)
steel = materials.solid.steel.Create(1)

meshes.beam.Create('beam.msh')
mesh = meshes.routines.LoadMesh(1, 'beam.msh', dof = 3)
meshes.routines.ApplyMaterial(mesh.GetElements(), steel)

temperature = structural.CreateValueScalar3D(T_ref)
pressure = structural.CreateValueScalar3D(p_ref)

nodesLeft = structural.FilterNodesByCoordinate(mesh.GetNodes(), basis, structural.axis_x, 0.0, 0.001)
nodesRight = structural.FilterNodesByCoordinate(mesh.GetNodes(), basis, structural.axis_x, meshes.beam.x, 0.001)
elementsRight = structural.FilterElementsByNodes(nodesRight)

pressureFace = structural.CreateValueVector3DScalars(3)
pressureFace.SetScalar(1, structural.CreateValueScalar3D(1000000000.0))

structural.CreateProblem(1, mesh, temperature, pressure)
structural.ApplyDirichlet(nodesLeft, 0.0)
structural.ApplyLoadDistributedFace(elementsRight, 0, pressureFace)
structural.Initialize()

#--------------------------------------------------------------------------------------------------

K = structural.PartitionMatrix(structural.GetProblem().Stiffness())
y = structural.PartitionVector(structural.GetProblem().Displacement())
f = structural.PartitionVector(structural.GetProblem().LoadDistributedFace())

monitor = solvers.IterativeBiCGStab(K[3], y[1], -K[2] * y[0] + f[1])
structural.UpdateMeshValues(y)

#plots.residual.Show(monitor)
nodesPlot = structural.FilterNodesByCoordinate(mesh.GetNodes(), basis, structural.axis_y, 0.0, 0.001)
nodesPlot = structural.FilterNodesByCoordinate(nodesPlot, basis, structural.axis_z, 0.0, 0.001)
plots.beam.Cantilever(nodesPlot, 1)