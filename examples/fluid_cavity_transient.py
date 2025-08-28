import meshes
import fluid.momentum
import fluid.pressure
import materials.fluid.water
import solvers
import plots
import plots.cavity

from prettytable import PrettyTable

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = fluid.CreateBasisCartesian(1)
timer       = fluid.CreateTimerStepped(1, 0.0, 10000.0, 1.0)
pressure    = fluid.CreateValueScalar3D(p_ref)
temperature = fluid.CreateValueScalar3D(T_ref)
material    = materials.fluid.water.Create(1, T_ref, p_ref)
sizeDomain  = 1.0

meshes.Initialize()
meshes.CreateCavity(sizeDomain, sizeDomain, 0.1 * sizeDomain, 15, 15, 2, False)
#meshes.Show()

meshVelocity = meshes.GetMeshForPhysicalGroup(meshTag = 1, numberDof = 3, physicalGroup = "cavity")
meshPressure = meshes.GetMeshForPhysicalGroup(meshTag = 1, numberDof = 1, physicalGroup = "cavity")

bcWall = meshes.GetNodesForPhysicalGroup(mesh = meshVelocity, physicalGroup = "wallViscous")
bcVelocity = meshes.GetNodesForPhysicalGroup(mesh = meshVelocity, physicalGroup = "velocity")
bcSymmetryPlane = meshes.GetNodesForPhysicalGroup(mesh = meshVelocity, physicalGroup = "symmetryPlane")
bcPressure = meshes.GetNodesForPhysicalGroup(mesh = meshPressure, physicalGroup = "pressure")

plotVelocity = meshes.GetNodesForPhysicalGroup(mesh = meshVelocity, physicalGroup = "plot")
plotPressure = meshes.GetNodesForPhysicalGroup(mesh = meshPressure, physicalGroup = "plot")

meshes.ApplyMaterial(meshVelocity.GetElements(), material)
meshes.ApplyMaterial(meshPressure.GetElements(), material)
meshes.Finalize()

pressure = fluid.CreateValueScalar3DCongruent(meshPressure)
velocity = fluid.CreateValueMatrix3DCongruent(meshVelocity)

#--------------------------------------------------------------------------------------------------
sizeElement = fluid.GetSizeMinimum(meshVelocity.GetElements())

rho = material.GetDensity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)
mu = material.GetDynamicViscosity(T_ref, p_ref)
kinematicViscosity = mu / rho

reynolds = 1000.0
speed = reynolds * mu /(rho * sizeDomain)

dt = timer.GetStepSize()
dt1 = sizeElement**2.0 / kinematicViscosity
dt2 = sizeElement / (speed)

tableSummary = PrettyTable()
tableSummary.field_names = ["Property", "Value", "Unit"]
tableSummary.align["Property"] = "l"
tableSummary.align["Unit"] = "l"
tableSummary.add_row(["Density", "{:.2f}".format(rho), "[kg/m³]"])
tableSummary.add_row(["Dynamic Viscosity", "{:.2e}".format(mu), ""])
tableSummary.add_row(["Kinematic Viscosity", "{:.2e}".format(kinematicViscosity), ""])
tableSummary.add_row(["Speed", "{:.2g}".format(speed), "[m/s]"])
tableSummary.add_row(["Reynolds", "{:.2e}".format(reynolds), "[-]"])
tableSummary.add_row(["Domain Size", "{:.2f}".format(sizeDomain), "[m]"])
tableSummary.add_row(["Element Size", "{:.2e}".format(sizeElement), "[m]"])
tableSummary.add_row(["Diffusion Time Ratio", "{:.4f}".format(dt / dt1), "[--]"])
tableSummary.add_row(["Convection Time Ratio", "{:.4f}".format(dt / dt2), "[--]"])
print(tableSummary)
#--------------------------------------------------------------------------------------------------

fluid.momentum.CreateProblem(1, meshVelocity, temperature, pressure)
fluid.momentum.ApplyDirichlet(bcVelocity, speed, dof = 0)
fluid.momentum.ApplyDirichlet(bcVelocity, 0.0, dof = 1)
fluid.momentum.ApplyDirichlet(bcVelocity, 0.0, dof = 2)
fluid.momentum.ApplyDirichlet(bcWall, 0.0)
fluid.momentum.ApplyDirichlet(bcSymmetryPlane, 0.0, dof = 2)
fluid.momentum.Initialize()

fluid.pressure.CreateProblem(2, meshPressure, temperature, velocity)
fluid.pressure.ApplyDirichlet(bcPressure, 0.0)
fluid.pressure.Initialize()

#--------------------------------------------------------------------------------------------------

kernels = fluid.CreateKernels("kernels.c", 0, 0)
matrix = fluid.Matrix(kernels, 3, 3)
quit()

M = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Mass())
K = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Stiffness())

H = fluid.pressure.PartitionMatrix(fluid.pressure.GetProblem().Stiffness())
G = fluid.pressure.GetProblem().Crossed(fluid.momentum.GetProblem()).Transpose()
D = fluid.pressure.GetProblem().DistributedVolumeDivergence(fluid.momentum.GetProblem())

while(True): 
    print("Time step = ", timer.GetCurrent())

    p = fluid.pressure.PartitionVector(fluid.pressure.GetProblem().Pressure())
    q0 = fluid.momentum.PartitionVector(fluid.momentum.GetProblem().Momentum())
    q1 = fluid.momentum.PartitionVector(fluid.Vector(fluid.momentum.GetProblem().GetTotalDof(), 0.0))
    dq = fluid.momentum.PartitionVector(fluid.Vector(fluid.momentum.GetProblem().GetTotalDof(), 0.0))
    dqq = fluid.momentum.PartitionVector(fluid.Vector(fluid.momentum.GetProblem().GetTotalDof(), 0.0))

    C = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Convection())
    monitor = solvers.IterativeBiCGStab(M[3], dq[1], -dt * (K[2] * q0[0] + C[2] * q0[0] + K[3] * q0[1] + C[3] * q0[1]))

    q1[0] = q0[0] + dq[0]
    q1[1] = q0[1] + dq[1]
    
    fluid.momentum.UpdateMeshValuesMomentum(q1)

    q = fluid.momentum.GetProblem().Momentum()
    fd = fluid.pressure.PartitionVector(D * q)
    monitor = solvers.IterativeBiCGStab(H[3], p[1], -H[2] * p[0] - (1.0 / dt) * (fd[1]))
    fluid.pressure.UpdateMeshValues(p)
    
    p = fluid.pressure.GetProblem().Pressure()
    fc = fluid.momentum.PartitionVector(G * p)
    monitor = solvers.IterativeBiCGStab(M[3], dqq[1], -dt * (fc[1]))

    q1[0] = q0[0] + dq[0] + dqq[0]
    q1[1] = q0[1] + dq[1] + dqq[1]

    fluid.momentum.UpdateMeshValuesMomentum(q1)

    if(timer.GetCurrent() == timer.GetEnd()):
        break
    else:
        timer.SetNextStep()

plots.HeatMapNorm(plotPressure)
plots.HeatMapNorm(plotVelocity)
plots.Vector(plotVelocity)

tolerance   = sizeElement/10.0
nodesMiddle = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, sizeDomain / 2.0, tolerance)
plots.cavity.HorizontalVelocity(nodesMiddle, speed)