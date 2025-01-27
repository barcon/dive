import meshes
import plots.beam
import thermal
import solvers
import materials.fluid
import plots.residual
import plots.field

from prettytable import PrettyTable

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStepped(1, 0.0, 5e+5, 1e+3)
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
tableSummary.add_row(["Specific Heat", "{:.2f}".format(cp), ""])
tableSummary.add_row(["Thermal Conductivity", "{:.2f}".format(k), ""])
tableSummary.add_row(["Thermal Diffusity", "{:.2e}".format(diffusity), ""])
tableSummary.add_row(["Domain Lenght", "{:.2f}".format(lenghtDomain), "[m]"])
tableSummary.add_row(["Element Height", "{:.2e}".format(heightElement), "[m]"])
tableSummary.add_row(["Diffusion Domain Time", "{:.2g}".format(dt1), "[s]"])
tableSummary.add_row(["Diffusion Element Time", "{:.2g}".format(dt2), "[s]"])
print(tableSummary)
#--------------------------------------------------------------------------------------------------

nodesLeft = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, 0.0, 0.01)
nodesRight = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, meshes.beam.x, 0.01)

nodesCurve = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_y, 0.0, 0.001)
nodesCurve = thermal.FilterNodesByCoordinate(nodesCurve, basis, thermal.axis_z, 0.0, 0.001)

nodesField = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_z, 0.0, 0.001)

thermal.CreateProblem(1, timer, mesh, pressure, None)
thermal.ApplyDirichlet(nodesLeft, 100.0)
thermal.ApplyDirichlet(nodesRight, 100.0)
thermal.Initialize()

dt = timer.GetStepSize()
M21, M22 = thermal.Mass()
K21, K22 = thermal.Diffusion()
y1, y1_1, y1_2 = thermal.Energy()
dy, dy_1, dy_2 = thermal.EnergyDerivative()

while(timer.GetCurrentTime() != timer.GetEndTime()):
    y0 = thermal.Energy()
    y0_1, y0_2 = thermal.Split(y0)
    
    dy_2, monitor  = solvers.Iterative(M22, -K21 * y0_1 - K22 * y0_2)

    y1 = solvers.EulerExplicit(y0, dy, dt)
        
    y0_1 = y1_1
    y0_2 = y1_2

    thermal.UpdateMeshValues(y1)

    timer.SetNextStep()

plots.field.Show(nodesField)