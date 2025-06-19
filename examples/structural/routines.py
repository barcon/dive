from dive import *

problem = None

def CreateProblem(tag, timer, mesh, temperature, pressure):
    global problem

    problem = CreateProblemStructural(tag)
    problem.SetTimer(timer)
    problem.SetMesh(mesh)
    problem.SetTemperature(temperature)
    problem.SetPressure(pressure)
    
    return problem

def GetProblem():
    return problem

def Initialize(): 
    global problem
    problem.Initialize()
    
    return

def UpdateMeshValues(y): 
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y0 = problem.Displacement()

    y0.Region(0, pivot - 1, y[0])
    y0.Region(pivot, totalDof - 1, y[1])

    problem.UpdateMeshValues(y0)
    
    return

def ApplyDirichlet(nodes, value, dof = None):
    global problem

    for node in nodes:
        if (dof == None):
            numberDof = node.GetNumberDof()
            for i in range(0, numberDof):
                dirichlet = CreateBoundaryCondition(node, i, CreateValueScalar3D(value))
                problem.AddDirichlet(dirichlet)
        else:
            dirichlet = CreateBoundaryCondition(node, dof, CreateValueScalar3D(value))
            problem.AddDirichlet(dirichlet)
    return

def ApplyVelocity(nodes, value, dof = None):
    global problem

    for node in nodes:
        if (dof == None):
            numberDof = node.GetNumberDof()
            for i in range(0, numberDof):
                velocity = CreateBoundaryCondition(node, i, CreateValueScalar3D(value))
                problem.AddVelocity(velocity)
        else:
            velocity = CreateBoundaryCondition(node, dof, CreateValueScalar3D(value))
            problem.AddVelocity(velocity)
    return

def PartitionVector(vector):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    v0 = vector.Region(0, pivot - 1)  
    v1 = vector.Region(pivot, totalDof - 1) 

    return [v0, v1]

def PartitionMatrix(matrix):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
        
    m00 = matrix.Region(0, 0, pivot - 1, pivot - 1)
    m01 = matrix.Region(0, pivot, pivot - 1, totalDof - 1)        
    m10 = matrix.Region(pivot, 0, totalDof - 1, pivot - 1)
    m11 = matrix.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    return [m00, m01, m10, m11]  

def ApplyLoadNode(nodes, load):
    global problem

    for node in nodes:
        problem.AddLoad(CreateLoadNode(node, load))
    
    return

def ApplyLoadDistributedVolume(elements, load):
    global problem

    for element in elements:
        problem.AddLoad(CreateLoadDistributedVolume(element, load))
    
    return

def ApplyLoadDistributedFace(elements, load, face):
    global problem

    for element in elements:
        problem.AddLoad(CreateLoadDistributedFace(element, face, load))
    
    return

def ApplyLoadDistributedEdge(elements, load, edge):
    global problem

    for element in elements:
        problem.AddLoad(CreateLoadDistributedEdge(element, edge, load))
    
    return