import dive
import os

def LoadMesh(tag, fileName, dof = 1):
    error = 0
    extension = os.path.splitext(fileName)[1].upper()

    if(extension == '.MSH'):
        mesh, error  = dive.LoadGmsh(tag, fileName, dof, error)
    elif(extension == '.CGNS'):
        mesh, error = dive.LoadCGNS(tag, fileName, dof, error)
    else:
        return None   
    
    return mesh

def SetNumberDof(elements, dof):
    for element in elements:
        element.SetNumberDof(dof)
    return

def ApplyMaterial(elements, material):
    for element in elements:
        element.SetMaterial(material)
    return

def ApplyField(mesh, dof, function):   
    SetNumberDof(mesh.GetElements(), dof)

    for node in mesh.GetNodes():
        point = node.GetPoint()
        value = function(point)
        node.SetValue(value)
    return