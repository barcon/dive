import dive

T_ref   = 313.15      #[K]      = 40 [°C]
p_ref   = 101325.1    #[N/m²]   =  1 [atm]
basis   = dive.CreateBasisCartesian(1)
timer   = dive.CreateTimerStepped(1, 0.0, 10.0, 0.01)
status  = None

node1 = dive.CreateNode(1, 0.0, 0.0, 0.0)
node2 = dive.CreateNode(2, 1.0, 0.0, 0.0)

#spring = dive.CreateElementSpring(1)
#spring.SetNode(0, node1)
#spring.SetNode(1, node2)
#spring.SetStiffness(dive.CreateValueScalar1D(1.0))

#mass = dive.CreateElementMass(2)
#mass.SetNode(0, node2)
#mass.SetMass(dive.CreateValueScalar(1.0))

mesh = dive.CreateMesh(1)
mesh.AddNode(node1, True)
#mesh.AddNode(node2, status)
#status = mesh.AddElement(spring, status)
#status = mesh.AddElement(mass, status)

print(mesh.GetTag())
#print(mesh.GetElements())