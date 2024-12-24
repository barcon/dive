import dive
import meshes

def Func1(x, y, z):
    return (-4.0 * x ** 2.0 + 4.0 * x) * (-4.0 * y ** 2.0 + 4.0 * y)
    
timer = dive.CreateTimerStationary(1, 0.0)
basis = dive.CreateBasisCartesian(1)
error = 0

mesh = meshes.LoadMesh(1, 'cavity.msh')
nodes = mesh.GetNodes()

for node in nodes:
    point = node.GetPoint()
    value = Func1(point.GetValue(0), point.GetValue(1), point.GetValue(2))
    node.SetValue(0, value)
    
intepolation = dive.CreateValueScalar3DInterpolation(nodes, dive.interpolation_nn)

print(Func1(0.0, 0.0, 0.0))
print(intepolation.GetValue(0.0, 0.0, 0.0))

print(Func1(0.5, 0.5, 0.5))
print(intepolation.GetValue(0.5, 0.5, 0.5))