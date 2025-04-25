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

def Initialize(): 
    global problem

    problem.Initialize()
    
    return

def UpdateMeshValues(y): 
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y0 = problem.Energy()

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
                dirichlet = CreateDirichletByValue(node, i, value)
                problem.AddDirichlet(dirichlet)
        else:
            dirichlet = CreateDirichletByValue(node, dof, value)
            problem.AddDirichlet(dirichlet)
    return

def Displacement():
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y = problem.Displacement()

    y0 = y.Region(0, pivot - 1)  
    y1 = y.Region(pivot, totalDof - 1) 

    return [y0, y1]

def DisplacementDerivative():
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    dy = Vector(totalDof)

    dy0 = dy.Region(0, pivot - 1)  
    dy1 = dy.Region(pivot, totalDof - 1) 

    return [dy0, dy1]

def Stiffness():
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    K = problem.Stiffness()
    K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
    K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    return [K21, K22]

def Mass():
    global problem    
    
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    M = problem.Mass()
    M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)
    M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    return [M21, M22]

def LoadNode():
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    f = problem.LoadNode()

    f0 = f.Region(0, pivot - 1)  
    f1 = f.Region(pivot, totalDof - 1) 

    #return [f0, f1]
    return f

def AddLoad(load):
    global problem
    problem.AddLoad(load)
    return