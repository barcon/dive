import meshes
import thermal
import solvers
import materials.fluid
import plots.residual
import plots.field

from prettytable import PrettyTable

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStepped(1, 0.0, 1e+5, 1e+2)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.fluid.CreateFluidOil(1, 68, T_ref)
meshFile    = 'beam.msh'

meshes.beam.quadratic = False
meshes.beam.Create(meshFile)
mesh = meshes.routines.LoadMesh(1, meshFile)
meshes.routines.ApplyMaterial(mesh.GetElements(), material)

rho = material.GetDensity(T_ref, p_ref)
k = material.GetThermalConductivity(T_ref, p_ref)
cp = material.GetSpecificHeat(T_ref, p_ref)

#--------------------------------------------------------------------------------------------------
heightElement = mesh.GetElementHeightMinium()
lenghtDomain = meshes.cavity.x
diffusity = k / (cp * rho)

dt1 = lenghtDomain**2.0 / diffusity
dt2 = heightElement**2.0 /  diffusity

tableSummary = PrettyTable()
tableSummary.field_names = ["Property", "Value", "Unit"]
tableSummary.align["Property"] = "l"
tableSummary.align["Unit"] = "l"
tableSummary.add_row(["Density", "{:.2f}".format(rho), ""])
tableSummary.add_row(["Thermal Conductivity", "{:.2e}".format(k), ""])
tableSummary.add_row(["Thermal Diffusity", "{:.2e}".format(diffusity), ""])
tableSummary.add_row(["Domain Lenght", "{:.2f}".format(lenghtDomain), ""])
tableSummary.add_row(["Element Height", "{:.2e}".format(heightElement), ""])
tableSummary.add_row(["Diffusion Domain Time", "{:.2g}".format(dt1), "[s]"])
tableSummary.add_row(["Diffusion Element Time", "{:.2g}".format(dt2), "[s]"])
print(tableSummary)
#--------------------------------------------------------------------------------------------------

nodesLeft = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, 0.0, 0.001)
nodesRight = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, meshes.cavity.x, 0.001)

nodesPlot = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_y, 0.0, 0.001)
nodesPlot = thermal.FilterNodesByCoordinate(nodesPlot, basis, thermal.axis_z, 0.0, 0.001)

thermal.CreateProblem(1, timer, mesh, pressure, None)
thermal.ApplyDirichlet(nodesLeft, 100.0)
thermal.ApplyDirichlet(nodesRight, 100.0)
thermal.Initialize()

dt = timer.GetStepSize()
M21, M22 = thermal.Mass()
K21, K22 = thermal.Diffusion()
y0_1, y0_2 = thermal.Energy()
y1_1, y1_2 = thermal.Energy()
dy_1, dy_2 = thermal.EnergyDerivative()

while(timer.GetCurrentTime() != timer.GetEndTime()):
    for i in range(0, 10):
        dy_2, monitor  = solvers.Iterative(M22, -M21 * dy_1 + K21 * y1_1 + K22 * y1_2)

        y1_1 = dt * dy_1 + y0_1
        y1_2 = dt * dy_2 + y0_2

    y0_1 = y1_1
    y0_2 = y1_2

    timer.SetNextStep()

thermal.UpdateMeshValues(y1_1, y1_2)

#plots.residual.Show(monitor)
plots.field.AddCurve(nodesPlot)