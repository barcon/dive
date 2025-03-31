import materials.fluid.water
import matplotlib.pyplot as plt

T_ref  = 273.15    #[K]      =  0 [°C]
p_ref  = 101325.1  #[N/m²]   =  1 [atm]

materials.fluid.water.configuration.densityConstant = False
materials.fluid.water.configuration.specificHeatConstant = False
materials.fluid.water.configuration.viscosityConstant = False
oil = materials.fluid.water.Create(1, T_ref, p_ref)

temperatures = [x for x in range(0, 101)]
pressure = p_ref
densities = []
specificHeats = []
viscosities = []

for temperature in temperatures:
    densities.append(oil.GetDensity(temperature + T_ref, pressure))
    specificHeats.append(oil.GetSpecificHeat(temperature + T_ref, pressure))
    viscosities.append(1000.0 * oil.GetDynamicViscosity(temperature + T_ref, pressure))

plt.subplot(1, 3, 1)
x, f = zip(*sorted(zip(temperatures, viscosities)))
plt.plot(x, f)  
plt.xlim(0.0, 100.0)
plt.xlabel("Temperature [°C]")
plt.grid(visible = True, which = "both")
plt.ylim(0.1, 10.0)
plt.ylabel("Dynamic Viscosity [mPa.s]")
plt.yscale('log',base=10) 

plt.subplot(1, 3, 2)
x, f = zip(*sorted(zip(temperatures, densities)))
plt.plot(x, f)  
plt.xlim(0.0, 100.0)
plt.xlabel("Temperature [°C]")
plt.grid(visible = True, which = "both")
plt.ylim(900.0, 1100.0)
plt.ylabel("Density [kg/m³]")

plt.subplot(1, 3, 3)
x, f = zip(*sorted(zip(temperatures, specificHeats)))
plt.plot(x, f)  
plt.xlim(0.0, 100.0)
plt.xlabel("Temperature [°C]")
plt.grid(visible = True, which = "both")
plt.ylim(4150.0, 4250.0)
plt.ylabel("specific heat [J/(kg.K)]")

plt.show()