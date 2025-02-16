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

def ApplyLoadDistributedVolumeDivergence(elements):
    for element in elements:
        load = CreateLoadDistributedVolume(element, None)
        problem.AddLoad(load)
    return

def Pressure(partitioned = True):
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y = problem.Pressure()

    if(partitioned):
        y0 = y.Region(0, pivot - 1)  
        y1 = y.Region(pivot, totalDof - 1) 
        return [y0, y1]  

    return y

def PressureDerivative(partitioned = True):
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    dy = Vector(totalDof)

    if(partitioned):
        dy0 = dy.Region(0, pivot - 1)  
        dy1 = dy.Region(pivot, totalDof - 1) 
        return [dy0, dy1]  

    return dy

def Mass(partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    M = problem.Mass()

    if(partitioned):
        M11 = M.Region(0, 0, pivot - 1, pivot - 1)
        M12 = M.Region(0, pivot, pivot - 1, totalDof - 1)
        M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)
        M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)
        return [M11, M12, M21, M22]        

    return M

def Stiffness(partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    K = problem.Stiffness()

    if(partitioned):
        K11 = K.Region(0, 0, pivot - 1, pivot - 1)
        K12 = K.Region(0, pivot, pivot - 1, totalDof - 1)
        K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
        K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)
        return [K11, K12, K21, K22]        

    return K

def Crossed(problemMomentum, partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    G = problem.Crossed(problemMomentum)

    if(partitioned):
        G11 = G.Region(0, 0, pivot - 1, pivot - 1)
        G12 = G.Region(0, pivot, pivot - 1, totalDof - 1)        
        G21 = G.Region(pivot, 0, totalDof - 1, pivot - 1)
        G22 = G.Region(pivot, pivot, totalDof - 1, totalDof - 1)
        return [G11, G12, G21, G22]        

    return G

def LoadDistributedVolumeDivergence(problemMomentum, partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    fd = problem.LoadDistributedVolumeDivergence(problemMomentum)
    
    if(partitioned):
        fd1 = fd.Region(0, pivot - 1)  
        fd2 = fd.Region(pivot, totalDof - 1) 
        return [fd1, fd2]      

    return fd