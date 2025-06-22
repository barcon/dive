import meshes
import thermal
import fluid.momentum
import solvers
import plots.residual
import plots.routines
import materials.fluid.VG46

from prettytable import PrettyTable

def FunctionVelocity(point) -> thermal.Matrix:
    global speed
    res = thermal.Matrix(3, 1, 0.0)
    res[0, 0] = speed
    return res

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.fluid.VG46.Create(1, 68, T_ref)
meshFile    = 'beam.msh'

meshes.beam.quadratic = True
meshes.beam.Create(meshFile)

meshThermal = meshes.routines.LoadMesh(1, meshFile, dof = 1)
meshMomentum = meshes.routines.LoadMesh(2, meshFile, dof = 3)
meshes.routines.ApplyMaterial(meshThermal.GetElements(), material)
meshes.routines.ApplyMaterial(meshMomentum.GetElements(), material)

pressure = thermal.CreateValueScalar3D(p_ref)
temperature = thermal.CreateValueScalar3D(T_ref)

#--------------------------------------------------------------------------------------------------
rho = material.GetDensity(T_ref, p_ref)
k = material.GetThermalConductivity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)

heightElement = thermal.GetSizeMinimum(meshThermal.GetElements())
lenghtDomain = meshes.beam.x
diffusity = k / (cp * rho)

peclet = 5.0
speed = (2.0 * peclet * diffusity) / heightElement

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

tolerance = heightElement/10.0
nodesLeft = thermal.FilterNodesByCoordinate(meshThermal.GetNodes(), basis, thermal.axis_x, 0.0, tolerance)
nodesRight = thermal.FilterNodesByCoordinate(meshThermal.GetNodes(), basis, thermal.axis_x, meshes.beam.x, tolerance)

nodesCurve = thermal.FilterNodesByCoordinate(meshThermal.GetNodes(), basis, thermal.axis_y, 0.0, tolerance)
nodesCurve = thermal.FilterNodesByCoordinate(nodesCurve, basis, thermal.axis_z, 0.0, tolerance)

thermal.CreateProblem(1, meshThermal, pressure)
thermal.ApplyDirichlet(nodesLeft, 1.0)
thermal.ApplyDirichlet(nodesRight, 0.0)
thermal.Initialize()

fluid.momentum.CreateProblem(1, meshMomentum, temperature, pressure)
meshes.routines.ApplyField(meshMomentum, dof = 3, function = FunctionVelocity)
fluid.momentum.Initialize()
#--------------------------------------------------------------------------------------------------

K = thermal.PartitionMatrix(thermal.GetProblem().Stiffness())
C = thermal.PartitionMatrix(thermal.GetProblem().Convection(fluid.momentum.GetProblem()))
y = thermal.PartitionVector(thermal.GetProblem().Energy())

monitor = solvers.IterativeBiCGStab(K[3] + C[3], y[1], -(K[2] + C[2]) * y[0])

thermal.UpdateMeshValues(y)
plots.Curve(nodesCurve)