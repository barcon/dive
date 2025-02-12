import fluid.momentum
import fluid.pressure
import meshes
import fluid
import solvers
import materials.fluid
import plots

from prettytable import PrettyTable

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = fluid.CreateBasisCartesian(1)
timer       = fluid.CreateTimerStepped(1, 0.0, 1e+3, 1.0)
pressure    = fluid.CreateValueScalar3D(p_ref)
material    = materials.fluid.CreateFluidOil(1, 68, T_ref)
meshFile    = 'cavity.msh'
speed       = 0.0

meshes.cavity.quadratic = False
meshes.cavity.Create(meshFile)
meshVelocity = meshes.routines.LoadMesh(1, meshFile)
meshPressure = meshes.routines.LoadMesh(2, meshFile)
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
lenghtDomain = meshes.cavity.x
diffusity = mu / (cp * rho)
reynolds = 10.0
speed =  reynolds * mu / (rho * lenghtDomain)

dt1 = lenghtDomain**2.0 / diffusity
dt2 = heightElement**2.0 /  diffusity
dt3 = lenghtDomain / (speed)
dt4 = heightElement / (speed)

tableSummary = PrettyTable()
tableSummary.field_names = ["Property", "Value", "Unit"]
tableSummary.align["Property"] = "l"
tableSummary.align["Unit"] = "l"
tableSummary.add_row(["Density", "{:.2f}".format(rho), "[kg/m³]"])
tableSummary.add_row(["Viscosity", "{:.2e}".format(mu), ""])
tableSummary.add_row(["Diffusity", "{:.2e}".format(diffusity), ""])
tableSummary.add_row(["Speed", "{:.2g}".format(speed), "[m/s]"])
tableSummary.add_row(["Reynolds", "{:.2e}".format(reynolds), "[-]"])
tableSummary.add_row(["Domain Lenght", "{:.2f}".format(lenghtDomain), "[m]"])
tableSummary.add_row(["Element Height", "{:.2e}".format(heightElement), "[m]"])
tableSummary.add_row(["Diffusion Domain Time", "{:.2e}".format(dt1), "[s]"])
tableSummary.add_row(["Diffusion Element Time", "{:.2e}".format(dt2), "[s]"])
tableSummary.add_row(["Convection Domain Time", "{:.2e}".format(dt3), "[s]"])
tableSummary.add_row(["Convection Element Time", "{:.2e}".format(dt4), "[s]"])
print(tableSummary)
#--------------------------------------------------------------------------------------------------

tolerance = heightElement/10.0
nodesTop = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_y, meshes.cavity.y, tolerance)
nodesLeft = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, 0.0, tolerance)
nodesRight = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_x, meshes.cavity.x, tolerance)
nodesBottom = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_y, 0.0, tolerance)
nodesFront = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_z, meshes.cavity.z, tolerance)
nodesBack = fluid.FilterNodesByCoordinate(meshVelocity.GetNodes(), basis, fluid.axis_z, 0.0, tolerance)

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

dt = timer.GetStepSize()

M = fluid.momentum.Mass()
K = fluid.momentum.Stiffness()
q = fluid.momentum.Momentum()
dq = fluid.momentum.MomentumDerivative()
dqq = fluid.momentum.MomentumDerivative()

H = fluid.pressure.Stiffness()
p = fluid.pressure.Pressure()

dq[1], monitor = solvers.Iterative(M[1], - dt * (K[0]) * q[0])
fluid.momentum.UpdateMeshValuesMomentum(dq)

fd = fluid.pressure.LoadDistributedVolumeDivergence(fluid.momentum.GetProblem())
p[1], monitor = solvers.Iterative(H[1], -H[0] * p[0] - (1.0 / dt) * (fd[1]))
fluid.pressure.UpdateMeshValues(p)

q[0] = q[0] + dq[0] + dqq[0]
q[1] = q[1] + dq[1] + dqq[1]

#fq = fluid.momentum.LoadDistributedVolumeCorrection1()
#fp = fluid.momentum.LoadDistributedVolumeCorrection2(fluid.pressure.GetProblem())
#dqq[1], monitor = solvers.Iterative(M[1], fq[1] + dt * fp[1])
fluid.momentum.UpdateMeshValuesMomentum(q)

plots.HeatMapNorm(meshPressure.GetNodes())
plots.HeatMapNorm(meshVelocity.GetNodes())
plots.Vector(meshVelocity.GetNodes())

#print(meshPressure.GetNodes())
#print(meshVelocity.GetNodes())
#print(meshVelocity.GetElements())
#print(problemVelocity.GetDofMeshIndices())
#print(problemVelocity.GetNodeMeshIndices())
#print(problemVelocity.GetDirichletMeshIndices())