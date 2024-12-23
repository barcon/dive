import fluid.dive
import fluid.cavity.config
import fluid.solver

problem = None
totalDof = 0
pivot = 0

C = None
C21 = None
C22 = None

K = None
K21 = None
K22 = None

S = None
S21 = None
S22 = None

f = None
g = None

q = None
dq = None
dqq = None

def CreateProblem(tag, timer, meshVelocity, stateTemperature, statePressure, stateVelocity):
    global problem 
    
    problem = fluid.dive.CreateProblemFluid(tag)
    problem.SetTimer(timer)
    problem.SetMesh(meshVelocity)
    problem.SetTemperature(stateTemperature)
    problem.SetPressure(statePressure)
    problem.SetVelocity(stateVelocity)
    
    return

def CalculateVelocity():
    Re = fluid.cavity.config.Reynolds
    rho = fluid.solver.material.GetDensity(fluid.cavity.config.T_ref, fluid.cavity.config.p_ref)
    mu = fluid.solver.material.GetDynamicViscosity(fluid.cavity.config.T_ref, fluid.cavity.config.p_ref)
    L = fluid.cavity.mesh.CharacteristicLength()

    velocity = Re * mu / (rho * L)

    print("Reynolds = ", "{:.2e}".format(Re))
    print("Characteristic Length = ", "{:.2f}".format(L))
    print("Density = ", "{:.2f}".format(rho))
    print("Velocity = ", "{:.4f}".format(velocity))
    print("Dyn. Viscosity = ", "{:.4f}".format(mu))

    return velocity

def ApplyBoundaryConditions():
    velocity = CalculateVelocity()   
    basis = fluid.dive.CreateBasisCartesian(1)
    nodes = problem.GetMesh().GetNodes()

    nodesTop = fluid.dive.FilterNodesByCoordinate(nodes, basis, fluid.dive.axis_y, 1.0, 0.01)
    for node in nodesTop:
        dirichlet0 = fluid.dive.CreateDirichletByValue(node, 0, velocity)
        dirichlet1 = fluid.dive.CreateDirichletByValue(node, 1, 0.0)
        dirichlet2 = fluid.dive.CreateDirichletByValue(node, 2, 0.0)
        problem.AddDirichlet(dirichlet0)
        problem.AddDirichlet(dirichlet1)
        problem.AddDirichlet(dirichlet2)

    nodesWall = fluid.dive.FilterNodesByCoordinate(nodes, basis, fluid.dive.axis_x, 0.0, 0.01) + fluid.dive.FilterNodesByCoordinate(nodes, basis, fluid.dive.axis_x, 1.0, 0.01) + fluid.dive.FilterNodesByCoordinate(nodes, basis, fluid.dive.axis_y, 0.0, 0.01)
    for node in nodesWall:
        dirichlet0 = fluid.dive.CreateDirichletByValue(node, 0, 0.0)
        dirichlet1 = fluid.dive.CreateDirichletByValue(node, 1, 0.0)
        dirichlet2 = fluid.dive.CreateDirichletByValue(node, 2, 0.0)
        problem.AddDirichlet(dirichlet0)
        problem.AddDirichlet(dirichlet1)
        problem.AddDirichlet(dirichlet2)
 
    nodesSide = fluid.dive.FilterNodesByCoordinate(nodes, basis, fluid.dive.axis_z, 0.0, 0.01) + fluid.dive.FilterNodesByCoordinate(nodes, basis, fluid.dive.axis_z, 0.1, 0.01)
    for node in nodesSide:
        dirichlet2 = fluid.dive.CreateDirichletByValue(node, 2, 0.0)
        problem.AddDirichlet(dirichlet2)

    return    

def Initialize(): 
    global totalDof
    global pivot

    problem.Initialize()
    totalDof = problem.GetTotalDof()
    pivot = problem.GetPivot() 

    return

def Derivative(q, t):
    global M 
    global M21 
    global M22 

    global C
    global C21
    global C22

    global K
    global K21
    global K22

    global S
    global S21
    global S22    
    
    global f    
    global g

    dt = problem.GetTimer().GetStepSize()

    if problem.GetTimer().GetStepNumber() == 0:
        M = problem.M()
        M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)
        M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)

        K = problem.K()
        K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
        K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)    

    C = problem.C()
    C21 = C.Region(pivot, 0, totalDof - 1, pivot - 1)
    C22 = C.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    S = problem.S()
    S21 = S.Region(pivot, 0, totalDof - 1, pivot - 1)
    S22 = S.Region(pivot, pivot, totalDof - 1, totalDof - 1)    

    f = problem.f() 
    f1 = f.Region(0, pivot - 1)  
    f2 = f.Region(pivot, totalDof - 1)  

    g = problem.g() 
    g1 = g.Region(0, pivot - 1)  
    g2 = g.Region(pivot, totalDof - 1)  

    q1 = q.Region(0, pivot - 1)
    q2 = q.Region(pivot, totalDof - 1)

    dq_dt = fluid.dive.Vector(totalDof, 0.0)
    dq1_dt = dq_dt.Region(0, pivot - 1)  
    dq2_dt = dq_dt.Region(pivot, totalDof - 1) 
    
    fluid.dive.IterativBiCGStab(dq2_dt, M22, -((C22 + K22 - dt * S22) * q2 + (C21 + K21 - dt * S21) * q1 - f2 - dt * g2), 1.0e-5)
 
    dq_dt.Region(0, pivot - 1, dq1_dt)
    dq_dt.Region(pivot, totalDof - 1, dq2_dt)

    return dq_dt

def Stationary(t):
    global C
    global C21
    global C22

    global K
    global K21
    global K22

    global S
    global S21
    global S22    
    
    global q
    global f    
    global g

    dt = 0.1

    K = problem.K()
    K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
    K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)  

    C = problem.C()
    C21 = C.Region(pivot, 0, totalDof - 1, pivot - 1)
    C22 = C.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    S = problem.S()
    S21 = S.Region(pivot, 0, totalDof - 1, pivot - 1)
    S22 = S.Region(pivot, pivot, totalDof - 1, totalDof - 1)    

    f = problem.f() 
    f1 = f.Region(0, pivot - 1)  
    f2 = f.Region(pivot, totalDof - 1)  

    g = problem.g() 
    g1 = g.Region(0, pivot - 1)  
    g2 = g.Region(pivot, totalDof - 1)  

    q = problem.q()
    q1 = q.Region(0, pivot - 1)
    q2 = q.Region(pivot, totalDof - 1)

    fluid.dive.IterativBiCGStab(q2, (C22 + K22 - dt * S22), - (C21 + K21 - dt * S21) * q1 + f2 + dt * g2, 1.0e-5)
 
    q.Region(0, pivot - 1, q1)
    q.Region(pivot, totalDof - 1, q2)

    return q

def Correction(p, dp, t):
    dt = problem.GetTimer().GetStepSize()
    theta1 = fluid.solver.theta1
    theta2 = fluid.solver.theta2

    G = fluid.cavity.pressure.G
    P = problem.P()
    
    dqq_dt = fluid.dive.Vector(totalDof, 0.0)    

    fluid.dive.IterativBiCGStab(dqq_dt, M, -dt * (G.Transpose() * (p + theta2 * dp) + dt / 2.0 * P * p), 1.0e-5)

    return dqq_dt  
