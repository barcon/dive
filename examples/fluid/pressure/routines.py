from dive import *

problem = None

def CreateProblem(tag, timer, mesh, temperature, velocity):
    global problem

    problem = CreateProblemPressure(tag)
    problem.SetTimer(timer)
    problem.SetMesh(mesh)
    problem.SetTemperature(temperature)
    problem.SetVelocity(velocity) 
    
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

    y0 = problem.Pressure()

    y0.Region(0, pivot - 1, y[0])
    y0.Region(pivot, totalDof - 1, y[1])

    problem.UpdateMeshValues(y0)
    
    return

def ApplyDirichlet(nodes, value, dof = None):
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

def Pressure():
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y = problem.Pressure()

    y0 = y.Region(0, pivot - 1)  
    y1 = y.Region(pivot, totalDof - 1) 

    return [y0, y1]

def PressureDerivative():
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    dy = Vector(totalDof)

    dy0 = dy.Region(0, pivot - 1)  
    dy1 = dy.Region(pivot, totalDof - 1) 

    return [dy0, dy1]

def Stiffness():
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    K = problem.Stiffness()
    K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
    K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    return [K21, K22]

def Mass():
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    M = problem.Mass()
    M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)
    M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    return [M21, M22]

def Crossed(problemMomentum):
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    G = problem.Crossed_Udp(problemMomentum)
    G21 = G.Region(pivot, 0, totalDof - 1, pivot - 1)
    G22 = G.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    return [G21, G22]