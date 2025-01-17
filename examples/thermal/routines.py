import solvers
from dive import *

problem = None

M = None
M21 = None
M22 = None

K = None
K21 = None
K22 = None

C = None
C21 = None
C22 = None

S = None
S21 = None
S22 = None

f = None
g = None

def CreateProblem(tag, timer, mesh, pressure, velocity):
    global problem

    problem = CreateProblemThermal(tag)
    problem.SetTimer(timer)
    problem.SetMesh(mesh)
    problem.SetPressure(pressure)
    problem.SetVelocity(velocity) 
    
    return problem

def Initialize(): 
    global problem

    problem.Initialize()
    
    return

def UpdateMeshValues(y): 
    global problem

    problem.UpdateMeshValues(y)
    
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

def Diffusion():
    global K 
    global K21
    global K22 

    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()
    
    K = problem.Stiffness()
    K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
    K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    y0 = problem.Energy()
    y0_1 = y0.Region(0, pivot - 1)  
    y0_2 = y0.Region(pivot, totalDof - 1)  

    y0_2, monitor = solvers.Iterative(K22, - K21 * y0_1)

    y0.Region(0, pivot - 1, y0_1)
    y0.Region(pivot, totalDof - 1, y0_2)

    return y0, monitor

def SolverStationaryConvection():
    global temperature

    global K 
    global K21
    global K22 

    global C
    global C21
    global C22 

    totalDof = temperature.totalDof
    pivot = temperature.pivot

    K = temperature.problem.Stiffness()  
    K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
    K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    C = temperature.problem.Convection()
    C21 = C.Region(pivot, 0, totalDof - 1, pivot - 1)
    C22 = C.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    y0 = temperature.problem.Energy()
    dy0 = dive.Vector(totalDof, 0.0) 

    y0_1 = y0.Region(0, pivot - 1)  
    y0_2 = y0.Region(pivot, totalDof - 1)  
    
    dy0_1 = dy0.Region(0, pivot - 1)  
    dy0_2 = dy0.Region(pivot, totalDof - 1)  
    
    dive.IterativeBiCGStab(dy0_2, C22 + K22, -(C21 + K21) * y0_1, 0.000001)

    dy0.Region(0, pivot - 1, dy0_1)
    dy0.Region(pivot, totalDof - 1, dy0_2)

    temperature.problem.UpdateMeshValues(y0 + dy0)

    return

def SolverStationaryStabilized():
    global temperature

    global K 
    global K21
    global K22 

    global C
    global C21
    global C22 

    global S 
    global S21
    global S22 
  
    dt = 0.1
    totalDof = temperature.totalDof
    pivot = temperature.pivot
    
    K = temperature.problem.Stiffness()  
    K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
    K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    C = temperature.problem.Convection()
    C21 = C.Region(pivot, 0, totalDof - 1, pivot - 1)
    C22 = C.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    S = temperature.problem.Stabilization()
    S21 = S.Region(pivot, 0, totalDof - 1, pivot - 1)
    S22 = S.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    y0 = temperature.problem.Energy()
    dy0 = dive.Vector(totalDof, 0.0) 

    y0_1 = y0.Region(0, pivot - 1)  
    y0_2 = y0.Region(pivot, totalDof - 1)  
    
    dy0_1 = dy0.Region(0, pivot - 1)  
    dy0_2 = dy0.Region(pivot, totalDof - 1)  
    
    dive.IterativeBiCGStab(dy0_2, C22 + K22 - dt * S22, -(C21 + K21 - dt * S21) * y0_1, 0.000001)

    dy0.Region(0, pivot - 1, dy0_1)
    dy0.Region(pivot, totalDof - 1, dy0_2)

    temperature.problem.UpdateMeshValues(y0 + dy0)

    return

def SolverTransient():
    global temperature

    totalDof = temperature.totalDof
    pivot = temperature.pivot

    t =  temperature.problem.GetTimer().GetInitialTime()
    dt =  temperature.problem.GetTimer().GetStepSize()    

    y0 = temperature.problem.e()
    dy0 = dive.Vector(totalDof, 0.0)

    while t != temperature.problem.GetTimer().GetEndTime():
        print("Solving time step = {0}".format(t))

        dy0 = Derivative(y0, t)        
        y1 = EulerExplicit(y0, dy0, dt)
        y0 = y1

        temperature.problem.UpdateMeshValues(y1)
        temperature.problem.GetTimer().SetNextStep()

        t =  temperature.problem.GetTimer().GetCurrentTime()
    return

def Derivative(y, t):
    global temperature

    global M 
    global M21 
    global M22 
    
    global K
    global K21
    global K22

    totalDof = temperature.totalDof
    pivot = temperature.pivot

    if temperature.problem.GetTimer().GetStepNumber() == 0:
        M = temperature.problem.M()
        M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)
        M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)

        K = temperature.problem.K()
        K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
        K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    y_1 = y.Region(0, pivot - 1)  
    y_2 = y.Region(pivot, totalDof - 1)  

    dy_dt = dive.Vector(totalDof, 0.0)
    dy_dt_1 = dy_dt.Region(0, pivot - 1)  
    dy_dt_2 = dy_dt.Region(pivot, totalDof - 1)  

    dive.IterativBiCGStab(dy_dt_2, M22, - (K21 * y_1 + K22 * y_2), 0.00001)

    dy_dt.Region(0, pivot - 1, dy_dt_1)
    dy_dt.Region(pivot, totalDof - 1, dy_dt_2)

    return dy_dt