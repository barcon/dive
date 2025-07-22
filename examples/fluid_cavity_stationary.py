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
timer       = fluid.CreateTimerStationary(1, 0.0)
pressure    = fluid.CreateValueScalar3D(p_ref)
material    = materials.fluid.water.Create(1, T_ref, p_ref)
meshFile    = 'cavity.msh'

meshes.cavity.quadratic = False
meshes.cavity.Create(meshFile)
meshVelocity = meshes.routines.LoadMesh(1, meshFile, dof = 3)
meshPressure = meshes.routines.LoadMesh(2, meshFile, dof = 1)
meshes.routines.ApplyMaterial(meshVelocity.GetElements(), material)
meshes.routines.ApplyMaterial(meshPressure.GetElements(), material)

temperature = fluid.CreateValueScalar3D(T_ref)
pressure = fluid.CreateValueScalar3DCongruent(meshPressure)
velocity = fluid.CreateValueMatrix3DCongruent(meshVelocity)

#--------------------------------------------------------------------------------------------------
sizeElement = fluid.GetSizeMinimum(meshVelocity.GetElements())
sizeDomain = meshes.cavity.x

rho = material.GetDensity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)
mu = material.GetDynamicViscosity(T_ref, p_ref)
kinematicViscosity = mu / rho

reynolds = 100.0
speed = reynolds * mu /(rho * sizeDomain)
#material.SetDynamicViscosity(fluid.CreateValueScalar2D(mu, 'Dynamic Viscosity', 'mu')) 

dt = 0.5 * sizeElement ** 2
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

tolerance   = sizeElement/10.0
nodesTop    = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_y, meshes.cavity.y, tolerance)
nodesLeft   = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, 0.0, tolerance)
nodesRight  = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, meshes.cavity.x, tolerance)
nodesBottom = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_y, 0.0, tolerance)
nodesFront  = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_z, meshes.cavity.z, tolerance)
nodesBack   = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_z, 0.0, tolerance)
nodesMiddle = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, meshes.cavity.x / 2.0, tolerance)

fluid.momentum.CreateProblem(1, meshVelocity, temperature, pressure)
fluid.momentum.ApplyDirichlet(nodesTop, speed, dof = 0)
fluid.momentum.ApplyDirichlet(nodesTop, 0.0, dof = 1)
fluid.momentum.ApplyDirichlet(nodesTop, 0.0, dof = 2)
fluid.momentum.ApplyDirichlet(nodesLeft, 0.0)
fluid.momentum.ApplyDirichlet(nodesRight, 0.0)
fluid.momentum.ApplyDirichlet(nodesBottom, 0.0)
fluid.momentum.ApplyDirichlet(nodesFront, 0.0, dof = 2)
fluid.momentum.ApplyDirichlet(nodesBack, 0.0, dof = 2)
fluid.momentum.Initialize()

nodesCorner = fluid.FilterNodesByCoordinate(meshPressure.GetNodes(), basis, fluid.axis_x, 0.0, tolerance)
nodesCorner = fluid.FilterNodesByCoordinate(nodesCorner, basis, fluid.axis_y, 0.0, tolerance)
elementsDivergence = meshPressure.GetElements()

fluid.pressure.CreateProblem(2, meshPressure, temperature, velocity)
fluid.pressure.ApplyDirichlet(nodesCorner, 0.0)
fluid.pressure.Initialize()

#--------------------------------------------------------------------------------------------------

M = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Mass())
K = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Stiffness())
C = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Convection())
S = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Stabilization())

H = fluid.pressure.PartitionMatrix(fluid.pressure.GetProblem().Stiffness())
G = fluid.pressure.GetProblem().Crossed(fluid.momentum.GetProblem()).Transpose()
D = fluid.pressure.GetProblem().DistributedVolumeDivergence(fluid.momentum.GetProblem())

omega = 0.5

for i in range(0, 100):
    print("Iteration = ", i)
    p = fluid.pressure.PartitionVector(fluid.pressure.GetProblem().Pressure())
    q0 = fluid.momentum.PartitionVector(fluid.momentum.GetProblem().Momentum())
    q1 = fluid.momentum.PartitionVector(fluid.momentum.GetProblem().Momentum())
    q2 = fluid.momentum.PartitionVector(fluid.momentum.GetProblem().Momentum())
    q3 = fluid.momentum.PartitionVector(fluid.Vector(fluid.momentum.GetProblem().GetTotalDof(), 0.0))
    dq = fluid.momentum.PartitionVector(fluid.Vector(fluid.momentum.GetProblem().GetTotalDof(), 0.0))
    dqq = fluid.momentum.PartitionVector(fluid.Vector(fluid.momentum.GetProblem().GetTotalDof(), 0.0))

    #C = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Convection())
    #S = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Stabilization())
    #monitor = solvers.Iterative(K[3] + C[3] - dt * S[3], dq[1], -(K[2] * q0[0] + K[3] * q0[1] + C[2] * q0[0] + C[3] * q0[1] - dt * (S[2] * q0[0] + S[3] * q0[1])))
    monitor = solvers.IterativeBiCGStab(K[3] + C[3], q1[1], -(K[2] * q0[0] + C[2] * q0[0]))
    #monitor = solvers.IterativeBiCGStab(K[3], q1[1], -(K[2] * q0[0]))

    q2[0] = omega * q1[0] + (1.0 - omega) * q0[0]
    q2[1] = omega * q1[1] + (1.0 - omega) * q0[1]

    #norm = fluid.NormP2(A * dq1[1] - b)
    #print("Iteration = ", i, " Residual = ", norm)

    fluid.momentum.UpdateMeshValuesMomentum(q2)
    
    q = fluid.momentum.GetProblem().Momentum()
    fd = fluid.pressure.PartitionVector(D * q)
    monitor = solvers.IterativeBiCGStab(H[3], p[1], -H[2] * p[0] - (1.0 / dt) * (fd[1]))
    fluid.pressure.UpdateMeshValues(p)
    
    p = fluid.pressure.GetProblem().Pressure()
    fc = fluid.momentum.PartitionVector(G * p)
    monitor = solvers.IterativeBiCGStab(M[3], dqq[1], -dt * (fc[1]))

    q3[0] = q2[0] + dqq[0]
    q3[1] = q2[1] + dqq[1]

    fluid.momentum.UpdateMeshValuesMomentum(q3)

plots.HeatMapNorm(meshPressure.GetNodes())
plots.HeatMapNorm(meshVelocity.GetNodes())

plots.Vector(meshVelocity.GetNodes())
plots.cavity.HorizontalVelocity(nodesMiddle, speed)