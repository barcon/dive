from dive import *

problem = None

def CreateProblem(tag, timer, mesh, pressure):
    global problem

    problem = CreateProblemThermal(tag)
    problem.SetTimer(timer)
    problem.SetMesh(mesh)
    problem.SetPressure(pressure)
    
    return problem

def GetProblem():
    return problem

def Initialize(): 
    global problem
    problem.Initialize()
       
    return

def UpdateMeshValues(q): 
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y = problem.Energy()
    y.Region(0, pivot - 1, q[0])
    y.Region(pivot, totalDof - 1, q[1])    
    
    problem.UpdateMeshValues(y)
    
    return

def ApplyDirichlet(nodes, value, dof = None):
    global problem

    for node in nodes:
        if (dof == None):
            numberDof = node.GetNumberDof()
            for i in range(0, numberDof):
                dirichlet = CreateDirichletByValue(node, i, value)
                problem.AddDirichlet(dirichlet)
        else:
            dirichlet = CreateDirichletByValue(node, dof, value)
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