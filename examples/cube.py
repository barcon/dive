import materials.solid.steel
import structural
import meshes

T_oil_bath  = 313.15    #[K]      = 40 [°C]
p_oil_bath  = 101325.1  #[N/m²]   =  1 [atm]

steel = materials.solid.steel.Create(1)
meshFile = 'cube.msh'

meshes.cube.Create(meshFile)
mesh = meshes.routines.LoadMesh(1, meshFile)
meshes.routines.ApplyMaterial(mesh.GetElements(), steel)

print(mesh.GetNodes())
print(mesh.GetElements())
for element in mesh.GetElements():
    elementHexa = structural.CastToElementHexa(element)
    print(elementHexa.Volume())

quit()