import dive

def LoadMesh(tag, fileName, dof = 1):
    error = 0
    extension = GetFileExtension(fileName)

    if(extension == 'msh'):
        mesh, error  = dive.LoadGmsh(tag, fileName, 1, error)
    elif (extension == 'inp'):
        mesh, error  = dive.LoadAbaqus(tag, fileName, 1, error)
    else:
        return None   

    SetNumberDof(mesh.GetElements(), dof)
    return mesh

def GetFileExtension(filename):
    
    return extension

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