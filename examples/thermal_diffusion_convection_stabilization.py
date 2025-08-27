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
    res = thermal.Matrix(3, 1)
    res[0, 0] = speed
    return res

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStationary(1, 0.0)
pressure = thermal.CreateValueScalar3D(p_ref)
temperature = thermal.CreateValueScalar3D(T_ref)
lenghtDomain = 1.0
material    = materials.fluid.VG46.Create(1, T_ref, p_ref)

meshes.Initialize()
meshes.CreateBeam(lenghtDomain, 0.1, 0.1, 21, 2, 2, True)
#meshes.Show()

meshThermal = meshes.GetMeshForPhysicalGroup(meshTag = 1, numberDof = 1, physicalGroup = "beam")
meshMomentum = meshes.GetMeshForPhysicalGroup(meshTag = 2, numberDof = 1, physicalGroup = "beam")

wall = meshes.GetNodesForPhysicalGroup(mesh = meshThermal, physicalGroup = "wall")
inlet = meshes.GetNodesForPhysicalGroup(mesh = meshThermal, physicalGroup = "inlet")
plot = meshes.GetNodesForPhysicalGroup(mesh = meshThermal, physicalGroup = "plot")

meshes.ApplyMaterial(meshThermal.GetElements(), material)
meshes.ApplyMaterial(meshMomentum.GetElements(), material)
meshes.Finalize()

#--------------------------------------------------------------------------------------------------
rho = material.GetDensity(T_ref, p_ref)
k = material.GetThermalConductivity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)

heightElement = thermal.GetSizeMinimum(meshThermal.GetElements())
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

thermal.CreateProblem(1, meshThermal, pressure)
thermal.ApplyDirichlet(inlet, 1.0)
thermal.ApplyDirichlet(wall, 0.0)
thermal.Initialize()

fluid.momentum.CreateProblem(1, meshMomentum, temperature, pressure)
meshes.ApplyField(meshMomentum, dof = 3, function = FunctionVelocity)
fluid.momentum.Initialize()

#--------------------------------------------------------------------------------------------------

K = thermal.PartitionMatrix(thermal.GetProblem().Stiffness())
C = thermal.PartitionMatrix(thermal.GetProblem().Convection(fluid.momentum.GetProblem()))
S = thermal.PartitionMatrix(thermal.GetProblem().Stabilization(fluid.momentum.GetProblem()))
y = thermal.PartitionVector(thermal.GetProblem().Energy())
dt = 0.05

monitor = solvers.IterativeBiCGStab(K[3] + C[3] - dt * S[3], y[1], -(K[2] + C[2] - dt * S[2]) * y[0])

thermal.UpdateMeshValues(y)
plots.Curve(plot)