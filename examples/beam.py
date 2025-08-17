import materials.solid.unit
import structural
import meshes

T_ref       = 313.15      #[K]      = 40 [°C]
p_ref       = 101325.1    #[N/m²]   =  1 [atm]
basis       = structural.CreateBasisCartesian(1)
timer       = structural.CreateTimerStationary(1, 0.0)
temperature = structural.CreateValueScalar3D(T_ref)
pressure    = structural.CreateValueScalar3D(p_ref)

material = materials.solid.unit.Create(1)
meshFile = 'cube.msh'

meshes.Initialize(meshFile)
meshes.CreateBeam()
meshes.Entities()
meshes.Finalize()