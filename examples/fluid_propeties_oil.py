import materials.fluid.oil_vg46
import matplotlib.pyplot as plt

T_ref  = 273.15    #[K]      =  0 [°C]
p_ref  = 101325.1  #[N/m²]   =  1 [atm]

materials.fluid.oil_vg46.configuration.densityConstant = False
materials.fluid.oil_vg46.configuration.specificHeatConstant = False
materials.fluid.oil_vg46.configuration.viscosityConstant = False
oil = materials.fluid.oil_vg46.Create(1, T_ref, p_ref)

temperatures = [x for x in range(0, 101)]
pressure = p_ref
densities = []
specificHeats = []
viscosities = []

for temperature in temperatures:
    densities.append(oil.GetDensity(temperature + T_ref, pressure))
    viscosities.append(1000.0 * oil.GetDynamicViscosity(temperature + T_ref, pressure))

x, f = zip(*sorted(zip(temperatures, viscosities)))
plt.plot(x, f)
plt.xlim(0.0, 100.0)
plt.xlabel("Temperature [°C]")
plt.grid(visible = True, which = "both")
plt.ylim(1.0, 1000.0)
plt.ylabel("Dynamic Viscosity [mPa.s]")
plt.yscale('log',base=10) 
plt.show()    

