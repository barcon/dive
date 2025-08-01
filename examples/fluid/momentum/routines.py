from dive import *

problem = None

def CreateProblem(tag, mesh, temperature, pressure):
    global problem

    problem = CreateProblemFluid(tag)
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

def UpdateMeshValuesMomentum(q): 
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y = problem.Momentum()
    y.Region(0, pivot - 1, q[0])
    y.Region(pivot, totalDof - 1, q[1])    
    
    problem.UpdateMeshValuesMomentum(y)
    
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