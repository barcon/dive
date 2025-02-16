from dive import *

problem = None

def CreateProblem(tag, timer, mesh, temperature, pressure):
    global problem

    problem = CreateProblemFluid(tag)
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
                dirichlet = CreateDirichletByValue(node, i, value)
                problem.AddDirichlet(dirichlet)
        else:
            dirichlet = CreateDirichletByValue(node, dof, value)
            problem.AddDirichlet(dirichlet)
    return

def Velocity(partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y = problem.Velocity()

    if(partitioned):
        y0 = y.Region(0, pivot - 1)  
        y1 = y.Region(pivot, totalDof - 1) 
        return [y0, y1]  

    return y 

def VelocityDerivative(partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    dy = Vector(totalDof)

    if(partitioned):
        dy0 = dy.Region(0, pivot - 1)  
        dy1 = dy.Region(pivot, totalDof - 1) 
        return [dy0, dy1]  

    return dy 

def Momentum(partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    y = problem.Momentum()

    if(partitioned):
        y0 = y.Region(0, pivot - 1)  
        y1 = y.Region(pivot, totalDof - 1) 
        return [y0, y1]  

    return y 

def MomentumDerivative(partitioned = True):
    global problem

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

def Convection(partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    C = problem.Convection()
    
    if(partitioned):
        C11 = C.Region(0, 0, pivot - 1, pivot - 1)
        C12 = C.Region(0, pivot, pivot - 1, totalDof - 1)        
        C21 = C.Region(pivot, 0, totalDof - 1, pivot - 1)
        C22 = C.Region(pivot, pivot, totalDof - 1, totalDof - 1)
        return [C11, C12, C21, C22]        

    return C

def Stabilization(partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    S = problem.Stabilization()

    if(partitioned):
        S11 = S.Region(0, 0, pivot - 1, pivot - 1)
        S12 = S.Region(0, pivot, pivot - 1, totalDof - 1)           
        S21 = S.Region(pivot, 0, totalDof - 1, pivot - 1)
        S22 = S.Region(pivot, pivot, totalDof - 1, totalDof - 1)
        return [S11, S12, S21, S22]        

    return S

def LoadDistributedCrossed(G, p, partitioned = True):
    global problem

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    fc = G * p

    if(partitioned):
        fc1 = fc.Region(0, pivot - 1)  
        fc2 = fc.Region(pivot, totalDof - 1) 
        return [fc1, fc2]      

    return fc
