import meshes
import thermal
import solvers
import materials.fluid.oil
import plots.residual
import plots.routines

from prettytable import PrettyTable

def FunctionVelocity(point):
    global speed

    res = thermal.Matrix(3, 1, 0.0)
    res[0, 0] = speed
    return res

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.oil.Create(1, 68, T_ref)
meshFile    = 'beam.msh'

meshes.beam.quadratic = True
meshes.beam.Create(meshFile)
meshThermal = meshes.routines.LoadMesh(1, meshFile)
meshVelocity = meshes.routines.LoadMesh(2, meshFile)
meshes.routines.ApplyMaterial(meshThermal.GetElements(), material)
meshes.routines.ApplyMaterial(meshVelocity.GetElements(), material)

pressure = thermal.CreateValueScalar3D(p_ref)
velocity = thermal.CreateValueMatrix3DCongruent(meshVelocity)

#--------------------------------------------------------------------------------------------------
rho = material.GetDensity(T_ref, p_ref)
k = material.GetThermalConductivity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)

heightElement = meshThermal.GetElementHeightMinium()
lenghtDomain = meshes.beam.x
diffusity = k / (cp * rho)

peclet = 3.5
speed = (2 * peclet * diffusity) / heightElement

dt1 = lenghtDomain**2.0 / diffusity
dt2 = heightElement**2.0 /  diffusity
dt3 = lenghtDomain / (speed)
dt4 = heightElement / (speed)

dt1 = lenghtDomain**2.0 / diffusity
dt2 = heightElement**2.0 /  diffusity

tableSummary = PrettyTable()
tableSummary.field_names = ["Property", "Value", "Unit"]
tableSummary.align["Property"] = "l"
tableSummary.align["Unit"] = "l"
tableSummary.add_row(["Peclet", "{:.2f}".format(peclet), ""])
tableSummary.add_row(["Speed", "{:.2e}".format(speed), ""])
tableSummary.add_row(["Density", "{:.2f}".format(rho), ""])
tableSummary.add_row(["Specific Heat", "{:.2f}".format(cp), ""])
tableSummary.add_row(["Thermal Conductivity", "{:.2f}".format(k), ""])
tableSummary.add_row(["Thermal Diffusity", "{:.2e}".format(diffusity), ""])
tableSummary.add_row(["Domain Lenght", "{:.2f}".format(lenghtDomain), "[m]"])
tableSummary.add_row(["Element Height", "{:.2e}".format(heightElement), "[m]"])
tableSummary.add_row(["Diffusion Domain Time", "{:.2g}".format(dt1), "[s]"])
tableSummary.add_row(["Diffusion Element Time", "{:.2g}".format(dt2), "[s]"])
tableSummary.add_row(["Convection Domain Time", "{:.2g}".format(dt3), "[s]"])
tableSummary.add_row(["Convection Element Time", "{:.2g}".format(dt4), "[s]"])
print(tableSummary)
#--------------------------------------------------------------------------------------------------

meshes.routines.ApplyField(meshVelocity, dof = 3, function = FunctionVelocity)

tolerance = heightElement/10.0
nodesLeft = thermal.FilterNodesByCoordinate(meshThermal.GetNodes(), basis, thermal.axis_x, 0.0, tolerance)
nodesRight = thermal.FilterNodesByCoordinate(meshThermal.GetNodes(), basis, thermal.axis_x, meshes.beam.x, tolerance)

nodesCurve = thermal.FilterNodesByCoordinate(meshThermal.GetNodes(), basis, thermal.axis_y, 0.0, tolerance)
nodesCurve = thermal.FilterNodesByCoordinate(nodesCurve, basis, thermal.axis_z, 0.0, tolerance)

thermal.CreateProblem(1, timer, meshThermal, pressure, velocity)
thermal.ApplyDirichlet(nodesLeft, 100.0)
thermal.ApplyDirichlet(nodesRight, 0.0)
thermal.Initialize()

K = thermal.Stiffness()
C = thermal.Convection()
S = thermal.Stabilization()
y = thermal.Energy()
dt = 0.01

y[1], monitor = solvers.Iterative(K[1] + C[1] - dt * S[1], -(K[0] + C[0] - dt *S[0])*y[0])

thermal.UpdateMeshValues(y)
plots.Curve(nodesCurve)