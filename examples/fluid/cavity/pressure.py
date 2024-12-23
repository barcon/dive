import fluid.dive
import fluid.cavity.config
import fluid.solver

problem = None
totalDof = 0
pivot = 0

M = None
M21 = None
M22 = None

H = None
H21 = None
H22 = None

G = None
G21 = None
G22 = None

f = None
g = None
p = None
dp = None

def CreateProblem(tag, timer, meshPressure, stateTemperature, statePressure, stateVelocity):
    global problem 
    problem = fluid.dive.CreateProblemPressure(tag)
    problem.SetTimer(timer)
    problem.SetMesh(meshPressure)
    problem.SetTemperature(stateTemperature)
    problem.SetPressure(statePressure)
    problem.SetVelocity(stateVelocity)
    
    return

def ApplyBoundaryConditions():
    pressure = fluid.cavity.config.p_ref
    basis = fluid.dive.CreateBasisCartesian(1)
    nodes = problem.GetMesh().GetNodes()

    nodesCorner = fluid.dive.FilterNodesByCoordinate(nodes, basis, fluid.dive.axis_x, 0.0, 0.01)
    nodesCorner = fluid.dive.FilterNodesByCoordinate(nodesCorner, basis, fluid.dive.axis_y, 0.0, 0.01)

    for node in nodesCorner:
        dirichlet0 = fluid.dive.CreateDirichletByValue(node, 0, pressure)
        problem.AddDirichlet(dirichlet0)

    return

def Initialize(): 
    global totalDof
    global pivot

    problem.Initialize()
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot()

    return

def Derivative(q, dq, p, t):
    global M 
    global M21 
    global M22 

    global H
    global H21
    global H22

    global G    
    global G21
    global G22  
    
    global f    
    global g

    dt = problem.GetTimer().GetStepSize()
    theta1 = fluid.solver.theta1
    theta2 = fluid.solver.theta2

    M = problem.M()
    M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)
    M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    if problem.GetTimer().GetStepNumber() == 0:
        H = problem.H()
        H21 = H.Region(pivot, 0, totalDof - 1, pivot - 1)
        H22 = H.Region(pivot, pivot, totalDof - 1, totalDof - 1)    

        G = problem.G(fluid.cavity.momentum.problem)      
        g1 = (G * q).Region(0, pivot - 1)  
        g2 = (G * q).Region(pivot, totalDof - 1)  

    f = problem.f() 
    f1 = f.Region(0, pivot - 1)  
    f2 = f.Region(pivot, totalDof - 1)  

    Mp = (M + dt**2 * theta1 * theta2 * H)
    M21 = Mp.Region(pivot, 0, totalDof - 1, pivot - 1)
    M22 = Mp.Region(pivot, pivot, totalDof - 1, totalDof - 1)
    
    q1 = q.Region(0, pivot - 1)  
    q2 = q.Region(pivot, totalDof - 1)  
    
    dq1 = dq.Region(0, pivot - 1)  
    dq2 = dq.Region(pivot, totalDof - 1)  

    p1 = p.Region(0, pivot - 1)
    p2 = p.Region(pivot, totalDof - 1)

    dp_dt = fluid.dive.Vector(totalDof, 0.0)
    dp1_dt = dp_dt.Region(0, pivot - 1)  
    dp2_dt = dp_dt.Region(pivot, totalDof - 1) 
    
    fluid.dive.IterativBiCGStab(dp2_dt, (M22 + (dt**2) * theta1 * theta2 * H22), (G22 * q2 + theta1 * G22 *dq2 - dt * theta1 * H22 * p2 - f2) + (G21 * q1 + theta1 * G21 *dq1 - dt * theta1 * H22 * p1), 1.0e-5)

    dp_dt.Region(0, pivot - 1, dp1_dt)
    dp_dt.Region(pivot, totalDof - 1, dp2_dt)

    return dp_dt

def Stationary(q, t):
    global H
    global H21
    global H22

    global G    
    global G21
    global G22  
    
    global p
    global f    
    global g

    dt = 0.1
    theta1 = fluid.solver.theta1
    theta2 = fluid.solver.theta2

    H = problem.H()
    H21 = H.Region(pivot, 0, totalDof - 1, pivot - 1)
    H22 = H.Region(pivot, pivot, totalDof - 1, totalDof - 1)    

    G = problem.G(fluid.cavity.momentum.problem)      
    g1 = (G * q).Region(0, pivot - 1)  
    g2 = (G * q).Region(pivot, totalDof - 1)  

    f = problem.f() 
    f1 = f.Region(0, pivot - 1)  
    f2 = f.Region(pivot, totalDof - 1)  
  
    p = problem.p()
    p1 = p.Region(0, pivot - 1)
    p2 = p.Region(pivot, totalDof - 1)

    fluid.dive.IterativBiCGStab(p2, H22, (1.0 / (dt * theta1)) *(g2 - dt * theta1 * H21 * p1), 1.0e-5)

    p.Region(0, pivot - 1, p1)
    p.Region(pivot, totalDof - 1, p2)

    return p