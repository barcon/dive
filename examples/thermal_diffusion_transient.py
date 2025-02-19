import meshes
import thermal
import solvers
import materials.fluid.oil
import plots
import math

from prettytable import PrettyTable

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = thermal.CreateBasisCartesian(1)
timer       = thermal.CreateTimerStepped(1, 0.0, 1e+6, 1e+2)
pressure    = thermal.CreateValueScalar3D(p_ref)
material    = materials.oil.Create(1, 68, T_ref)
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
lengthDomain = meshes.cavity.x
diffusity = k / (cp * rho)

dt1 = lengthDomain**2.0 / diffusity
dt2 = heightElement**2.0 /  diffusity

tableSummary = PrettyTable()
tableSummary.field_names = ["Property", "Value", "Unit"]
tableSummary.align["Property"] = "l"
tableSummary.align["Unit"] = "l"
tableSummary.add_row(["Density", "{:.2f}".format(rho), ""])
tableSummary.add_row(["Specific Heat", "{:.2f}".format(cp), ""])
tableSummary.add_row(["Thermal Conductivity", "{:.2f}".format(k), ""])
tableSummary.add_row(["Thermal Diffusity", "{:.2e}".format(diffusity), ""])
tableSummary.add_row(["Domain Lenght", "{:.2f}".format(lengthDomain), "[m]"])
tableSummary.add_row(["Element Size", "{:.2e}".format(heightElement), "[m]"])
tableSummary.add_row(["Diffusion Domain Time", "{:.2g}".format(dt1), "[s]"])
tableSummary.add_row(["Diffusion Element Time", "{:.2g}".format(dt2), "[s]"])
print(tableSummary)
#--------------------------------------------------------------------------------------------------

tolerance = heightElement/10.0
nodesLeft = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, 0.0, tolerance)
nodesRight = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_x, meshes.beam.x, tolerance)

nodesCurve = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_y, 0.0, tolerance)
nodesCurve = thermal.FilterNodesByCoordinate(nodesCurve, basis, thermal.axis_z, 0.0, tolerance)

nodesField = thermal.FilterNodesByCoordinate(mesh.GetNodes(), basis, thermal.axis_z, 0.0, tolerance)

thermal.CreateProblem(1, timer, mesh, pressure, None)
thermal.ApplyDirichlet(nodesLeft, 100.0)
thermal.ApplyDirichlet(nodesRight, 100.0)
thermal.Initialize()

M21, M22 = thermal.Mass()
K21, K22 = thermal.Stiffness()

def Diffusion(t, y):
    global M21
    global M22
    global K21
    global K22

    return M22, -K21 * y[0] - K22 * y[1]

while(True): 
    y0 = thermal.Energy()
    #y1 = solvers.ForwardMethod(timer, y0, Diffusion)
    #y1 = solvers.BackwardMethod(timer, y0, Diffusion)
    y1 = solvers.CrankNicolsonMethod(timer, y0, Diffusion)

    thermal.UpdateMeshValues(y1)

    if(timer.GetCurrentTime() == timer.GetEndTime()):
        break        
    else:
        timer.SetNextStep()  

plots.Curve(nodesCurve)