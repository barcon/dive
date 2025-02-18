import meshes
import fluid.momentum
import fluid.pressure
import materials.fluid
import solvers
import plots
import plots.cavity

from prettytable import PrettyTable

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = fluid.CreateBasisCartesian(1)
timer       = fluid.CreateTimerStepped(1, 0.0, 1e+5, 1.0)
pressure    = fluid.CreateValueScalar3D(p_ref)
material    = materials.fluid.CreateFluidOil(1, 68, T_ref)
meshFile    = 'cavity.msh'
speed       = 0.0

meshes.cavity.quadratic = False
meshes.cavity.Create(meshFile)
meshVelocity = meshes.routines.LoadMesh(1, meshFile, dof = 3)
meshPressure = meshes.routines.LoadMesh(2, meshFile, dof = 1)
meshes.routines.ApplyMaterial(meshVelocity.GetElements(), material)
meshes.routines.ApplyMaterial(meshPressure.GetElements(), material)

temperature = fluid.CreateValueScalar3D(T_ref)
pressure = fluid.CreateValueScalar3DCongruent(meshPressure)
velocity = fluid.CreateValueMatrix3DCongruent(meshVelocity)

rho = material.GetDensity(T_ref, p_ref)
mu = material.GetDynamicViscosity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)

#--------------------------------------------------------------------------------------------------
heightElement = meshVelocity.GetElementHeightMinium()
lengthDomain = meshes.cavity.x
kinematicViscosity = mu / rho
reynolds = 400.0
speed =  reynolds * mu / (rho * lengthDomain)

dt = timer.GetStepSize()
dt1 = heightElement**2.0 / kinematicViscosity
dt2 = heightElement / (speed)

tableSummary = PrettyTable()
tableSummary.field_names = ["Property", "Value", "Unit"]
tableSummary.align["Property"] = "l"
tableSummary.align["Unit"] = "l"
tableSummary.add_row(["Density", "{:.2f}".format(rho), "[kg/m³]"])
tableSummary.add_row(["Dynamic Viscosity", "{:.2e}".format(mu), ""])
tableSummary.add_row(["Kinematic Viscosity", "{:.2e}".format(kinematicViscosity), ""])
tableSummary.add_row(["Speed", "{:.2g}".format(speed), "[m/s]"])
tableSummary.add_row(["Reynolds", "{:.2e}".format(reynolds), "[-]"])
tableSummary.add_row(["Domain Length", "{:.2f}".format(lengthDomain), "[m]"])
tableSummary.add_row(["Element Height", "{:.2e}".format(heightElement), "[m]"])
tableSummary.add_row(["Diffusion Time Ratio", "{:.4f}".format(dt / dt1), "[--]"])
tableSummary.add_row(["Convection Time Ratio", "{:.4f}".format(dt / dt2), "[--]"])
print(tableSummary)
#--------------------------------------------------------------------------------------------------

tolerance   = heightElement/10.0
nodesTop    = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_y, meshes.cavity.y, tolerance)
nodesLeft   = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, 0.0, tolerance)
nodesRight  = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, meshes.cavity.x, tolerance)
nodesBottom = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_y, 0.0, tolerance)
nodesFront  = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_z, meshes.cavity.z, tolerance)
nodesBack   = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_z, 0.0, tolerance)
nodesMiddle = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, meshes.cavity.x / 2.0, tolerance)

fluid.momentum.CreateProblem(1, timer, meshVelocity, temperature, pressure)
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

fluid.pressure.CreateProblem(2, timer, meshPressure, temperature, velocity)
fluid.pressure.ApplyDirichlet(nodesCorner, 0.0)
fluid.pressure.ApplyLoadDistributedVolumeDivergence(elementsDivergence)
fluid.pressure.Initialize()

M = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Mass())
K = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Stiffness())
H = fluid.pressure.PartitionMatrix(fluid.pressure.GetProblem().Stiffness())
Gt = fluid.pressure.GetProblem().Crossed(fluid.momentum.GetProblem()).Transpose()

while(True): 
    p = fluid.pressure.PartitionVector(fluid.pressure.GetProblem().Pressure())
    q = fluid.momentum.PartitionVector(fluid.momentum.GetProblem().Momentum())
    dq = fluid.momentum.PartitionVector(fluid.Vector(fluid.momentum.GetProblem().GetTotalDof()))
    dqq = fluid.momentum.PartitionVector(fluid.Vector(fluid.momentum.GetProblem().GetTotalDof()))

    C = fluid.momentum.PartitionMatrix(fluid.momentum.GetProblem().Convection())
    monitor = solvers.Iterative(M[3], dq[1], -dt * (K[2] * q[0] + K[3] * q[1] + C[2] * q[0] + C[3] * q[1]))
    #monitor = solvers.Iterative(M[3], dq[1], -dt * (K[2] * q[0] + K[3] * q[1]))
    q[0] = q[0] + dq[0]
    q[1] = q[1] + dq[1]
    fluid.momentum.UpdateMeshValuesMomentum(q)

    fd = fluid.pressure.PartitionVector(fluid.pressure.GetProblem().LoadDistributedVolumeDivergence(fluid.momentum.GetProblem()))
    monitor = solvers.Iterative(H[3], p[1], -H[2] * p[0] - (1.0 / dt) * (fd[1]))
    fluid.pressure.UpdateMeshValues(p)

    fluid.momentum.UpdateMeshValuesMomentum(dq)
    fc = fluid.momentum.LoadDistributedCrossed(Gt, fluid.pressure.GetProblem().Pressure())
    monitor = solvers.Iterative(M[3], dqq[1], -dt * (fc[1]))

    q[0] = q[0] + dqq[0]
    q[1] = q[1] + dqq[1]

    fluid.momentum.UpdateMeshValuesMomentum(q)

    if(timer.GetCurrentTime() == timer.GetEndTime()):
        break
    else:
        timer.SetNextStep()

#plots.HeatMapNorm(meshPressure.GetNodes())
#plots.HeatMapNorm(meshVelocity.GetNodes())
#plots.Vector(meshVelocity.GetNodes())
plots.cavity.HorizontalVelocity(nodesMiddle, speed)
