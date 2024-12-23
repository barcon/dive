import dive
import meshes

from dataclasses import dataclass

@dataclass
class Pressure:
    problem = None 
    material = None 
    numberDof = 0
    totalDof = 0
    pivot = 0

@dataclass
class Velocity:
    problem = None
    material = None
    numberDof = 0    
    totalDof = 0
    pivot = 0

@dataclass
class Config:
    type = None
    imax = None
    tolerance = None
    monitoring = False

pressure = None
velocity = None
config = None

M = None
M21 = None
M22 = None

C = None
C21 = None
C22 = None

K = None
K21 = None
K22 = None

S = None
S21 = None
S22 = None

Z = None
Z21 = None
Z22 = None

H = None
H21 = None
H22 = None

G = None
G21 = None
G22 = None

J = None
J21 = None
J22 = None

p = None
dp = None

q = None
dq = None
dqq = None 

t = None
theta1 = 1.0
theta2 = 0.5

def CreateProblemPressure(tag, timer, mesh, temperature, velocity, material):
    global pressure
    pressure = Pressure()

    pressure.problem = dive.CreateProblemPressure(tag)
    pressure.problem.SetTimer(timer)
    pressure.problem.SetMesh(mesh)
    pressure.problem.SetTemperature(temperature)
    pressure.problem.SetVelocity(velocity)

    pressure.numberDof = pressure.problem.GetNumberDof()
    pressure.material = material

    meshes.routines.SetNumberDof(mesh.GetElements(), pressure.numberDof)
    meshes.routines.ApplyMaterial(mesh.GetElements(), material)
    
    return pressure.problem

def CreateProblemVelocity(tag, timer, mesh, temperature, pressure, material):
    global velocity 
    velocity = Velocity()    

    velocity.problem = dive.CreateProblemFluid(tag)
    velocity.problem.SetTimer(timer)
    velocity.problem.SetMesh(mesh)
    velocity.problem.SetTemperature(temperature)
    velocity.problem.SetPressure(pressure) 
    
    velocity.numberDof = velocity.problem.GetNumberDof()
    velocity.material = material

    meshes.routines.SetNumberDof(mesh.GetElements(), velocity.numberDof) 
    meshes.routines.ApplyMaterial(mesh.GetElements(), material) 

    return velocity.problem

def InitializePressure(): 
    global pressure

    pressure.problem.Initialize()
    pressure.totalDof = pressure.problem.GetTotalDof()
    pressure.pivot = pressure.problem.GetPivot()
    
    return

def InitializeVelocity(): 
    global velocity

    velocity.problem.Initialize()
    velocity.totalDof = velocity.problem.GetTotalDof()
    velocity.pivot = velocity.problem.GetPivot()
    
    return

def TransientDiffusion():
    global velocity

    global M
    global M21
    global M22

    global K
    global K21
    global K22

    global q
    global dq
    global dqq

    t = velocity.problem.GetTimer().GetCurrentTime()
    dt = velocity.problem.GetTimer().GetStepSize()
    totalDof = velocity.totalDof
    pivot = velocity.pivot

    q = velocity.problem.Momentum()
    q1 = q.Region(0, pivot - 1)
    q2 = q.Region(pivot, totalDof - 1)

    dq = dive.Vector(totalDof, 0.0)
    dq1 = dq.Region(0, pivot - 1)
    dq2 = dq.Region(pivot, totalDof - 1)

    if t == velocity.problem.GetTimer().GetInitialTime():      
        M = velocity.problem.Stiffness()
        M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)  
        M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)  

        K = velocity.problem.Stiffness()
        K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)  
        K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)          

    k2 = (K * q).Region(pivot, totalDof - 1)

    f = velocity.problem.LoadDistributedVolume() 
    f1 = f.Region(0, pivot - 1)  
    f2 = f.Region(pivot, totalDof - 1)  
    
    dive.IterativBiCGStab(dq2, M22, - dt * (k2 - f2), 1.0e-6)

    dq.Region(0, pivot - 1, dq1)
    dq.Region(pivot, totalDof - 1, dq2)

    return dq

def TransientConvection():
    global velocity

    global M
    global M21
    global M22

    global K
    global K21
    global K22

    global C
    global C21
    global C22    

    global q
    global dq
    global dqq

    t = velocity.problem.GetTimer().GetCurrentTime()
    dt = velocity.problem.GetTimer().GetStepSize()
    totalDof = velocity.totalDof
    pivot = velocity.pivot

    q = velocity.problem.Momentum()
    q1 = q.Region(0, pivot - 1)
    q2 = q.Region(pivot, totalDof - 1)

    dq = dive.Vector(totalDof, 0.0)
    dq1 = dq.Region(0, pivot - 1)
    dq2 = dq.Region(pivot, totalDof - 1)

    if t == velocity.problem.GetTimer().GetInitialTime():      
        M = velocity.problem.Stiffness()
        M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)  
        M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)  

        K = velocity.problem.Stiffness()
        K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)  
        K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)          

    k2 = (K * q).Region(pivot, totalDof - 1)

    C = velocity.problem.Stiffness()
    c2 = (C * q).Region(pivot, totalDof - 1)

    f = velocity.problem.LoadDistributedVolume() 
    f1 = f.Region(0, pivot - 1)  
    f2 = f.Region(pivot, totalDof - 1)  
    
    dive.IterativBiCGStab(dq2, M22, - dt * (c2 + k2 - f2), 1.0e-6)

    dq.Region(0, pivot - 1, dq1)
    dq.Region(pivot, totalDof - 1, dq2)

    return dq

def TransientPressure():
    global pressure
    global velocity

    global H
    global H21
    global H22

    global G    
    global G21
    global G22  
    
    global p
    global dp

    global q
    global dq
    global dqq

    global theta1
    global theta2

    t = pressure.problem.GetTimer().GetCurrentTime()
    dt = pressure.problem.GetTimer().GetStepSize()
    totalDof = pressure.totalDof
    pivot = pressure.pivot
    
    if t == pressure.problem.GetTimer().GetInitialTime():      
        H = pressure.problem.Stiffness()

    
    M = pressure.problem.Mass()
    M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    #G = pressure.problem.Crossed(velocity.problem)      
    #g1 = (G * q).Region(pivot, totalDof - 1)  
    #g2 = (G * dq).Region(pivot, totalDof - 1)  

    #f = pressure.problem.LoadDistributedFaceFlux(velocity.problem, q)
    #f2 = f.Region(pivot, totalDof - 1)

    p = pressure.problem.Pressure()
    p2 = (H * p).Region(pivot, totalDof - 1)  

    dp = dive.Vector(totalDof, 0.0)
    dp1 = dp.Region(0, pivot - 1)
    dp2 = dp.Region(pivot, totalDof - 1)

    dive.IterativBiCGStab(dp2, M22, dt * (- dt * theta1 * p2), 1.0e-6)
    #dive.IterativBiCGStab(dp2, M22, dt * (g1 + theta1 * g2 - dt * theta1 * p2), 1.0e-6)
    #dive.IterativBiCGStab(dp2, (M22 + dt * dt * theta1 * theta2 * H22), dt * (g1 + theta1 * g2 - dt * theta1 * p2), 1.0e-6)

    dp.Region(0, pivot - 1, dp1)
    dp.Region(pivot, totalDof - 1, dp2)

    return dp

def TransientCorrection():
    global pressure
    global velocity

    global M    
    global M21
    global M22

    global G    
    global G21
    global G22

    global J    
    global J21
    global J22

    global p
    global dp

    global q
    global dq
    global dqq

    global theta1

    t = velocity.problem.GetTimer().GetCurrentTime()
    dt = velocity.problem.GetTimer().GetStepSize()
    totalDof = velocity.totalDof
    pivot = velocity.pivot

    G = pressure.problem.Crossed(velocity.problem).Transpose()
    g2 = (G * p).Region(pivot, totalDof - 1)    
   
    J = pressure.problem.Stabilization(velocity.problem).Transpose()
    j2 = (J * p).Region(pivot, totalDof - 1)    

    dqq = dive.Vector(totalDof, 0.0)
    dqq1 = dqq.Region(0, pivot - 1)
    dqq2 = dqq.Region(pivot, totalDof - 1)    
    
    dive.IterativBiCGStab(dqq2, M22, - dt * (g2 + 1.0 / 2.0 * dt * j2), 1.0e-6)
    #dive.IterativBiCGStab(dqq2, M22, - dt * (g2), 1.0e-6)
    dqq.Region(0, pivot - 1, dqq1)
    dqq.Region(pivot, totalDof - 1, dqq2)

    return dqq  

"""  

def StationaryConvection():
    global velocity

    global K
    global K21
    global K22

    global C
    global C21
    global C22    
  
    global q

    totalDof = velocity.totalDof
    pivot = velocity.pivot

    K = velocity.problem.Stiffness()
    K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
    K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)  

    C = velocity.problem.Convection()
    C21 = C.Region(pivot, 0, totalDof - 1, pivot - 1)
    C22 = C.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    f = velocity.problem.LoadDistributedVolume() 
    f1 = f.Region(0, pivot - 1)  
    f2 = f.Region(pivot, totalDof - 1)  

    q = velocity.problem.Momentum()
    q1 = q.Region(0, pivot - 1)
    q2 = q.Region(pivot, totalDof - 1)

    dive.IterativBiCGStab(q2, (C22 + K22), -(C21 + K21) * q1 + f2, 1.0e-8)
 
    q.Region(0, pivot - 1, q1)
    q.Region(pivot, totalDof - 1, q2)

    #velocity.problem.UpdateMeshValues(q)    

    return

def StationaryStabilized():
    global velocity

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

    dt = 0.1
    totalDof = velocity.totalDof
    pivot = velocity.pivot

    K = velocity.problem.Stiffness()
    K21 = K.Region(pivot, 0, totalDof - 1, pivot - 1)
    K22 = K.Region(pivot, pivot, totalDof - 1, totalDof - 1)  

    C = velocity.problem.Convection()
    C21 = C.Region(pivot, 0, totalDof - 1, pivot - 1)
    C22 = C.Region(pivot, pivot, totalDof - 1, totalDof - 1)

    S = velocity.problem.Stabilization()
    S21 = S.Region(pivot, 0, totalDof - 1, pivot - 1)
    S22 = S.Region(pivot, pivot, totalDof - 1, totalDof - 1)    

    f = velocity.problem.LoadDistributedVolume() 
    f1 = f.Region(0, pivot - 1)  
    f2 = f.Region(pivot, totalDof - 1)  

    g = velocity.problem.LoadDistributedVolumeStabilization() 
    g1 = g.Region(0, pivot - 1)  
    g2 = g.Region(pivot, totalDof - 1)  

    q = velocity.problem.Momentum()
    q1 = q.Region(0, pivot - 1)
    q2 = q.Region(pivot, totalDof - 1)

    dive.IterativBiCGStab(q2, (C22 + K22 - dt * S22), - (C21 + K21 - dt * S21) * q1 + f2 + dt * g2, 1.0e-8)
 
    q.Region(0, pivot - 1, q1)
    q.Region(pivot, totalDof - 1, q2)

    return

def StationaryPressure():
    global pressure
    global velocity

    global H
    global H21
    global H22

    global G    
    global G21
    global G22  
    
    global p
    global q

    dt = 0.1
    theta1 = 1.0
    totalDof = pressure.totalDof
    pivot = pressure.pivot

    H = pressure.problem.Stiffness()
    H21 = H.Region(pivot, 0, totalDof - 1, pivot - 1)
    H22 = H.Region(pivot, pivot, totalDof - 1, totalDof - 1)    

    G = pressure.problem.Crossed(velocity.problem)      
    g1 = (G * q).Region(0, pivot - 1)  
    g2 = (G * q).Region(pivot, totalDof - 1)  

    f = pressure.problem.LoadDistributedFaceFlux(velocity.problem, q)
    f1 = f.Region(0, pivot - 1)
    f2 = f.Region(pivot, totalDof - 1)

    p = pressure.problem.Pressure()
    p1 = p.Region(0, pivot - 1)
    p2 = p.Region(pivot, totalDof - 1)

    dive.IterativBiCGStab(p2, H22, - H21 * p1 + (1.0 / (dt * theta1)) *(g2 - dt * f2), 1.0e-8)

    p.Region(0, pivot - 1, p1)
    p.Region(pivot, totalDof - 1, p2)

    pressure.problem.UpdateMeshValues(p)

    return

def StationaryCorrection():
    global pressure
    global velocity

    global M    
    global M21
    global M22

    global G    
    global G21
    global G22

    global J    
    global J21
    global J22

    global p

    dt = 0.1
    theta1 = 1.0

    totalDof = velocity.totalDof
    pivot = velocity.pivot

    M = velocity.problem.Mass()
    M21 = M.Region(pivot, 0, totalDof - 1, pivot - 1)
    M22 = M.Region(pivot, pivot, totalDof - 1, totalDof - 1)  

    G = pressure.problem.Crossed(velocity.problem).Transpose()
    g1 = (G * p).Region(0, pivot - 1)
    g2 = (G * p).Region(pivot, totalDof - 1)    
   
    J = pressure.problem.Stabilization(velocity.problem).Transpose()
    j1 = (J * p).Region(0, pivot - 1)
    j2 = (J * p).Region(pivot, totalDof - 1)    

    dqq = dive.Vector(totalDof, 0.0)
    dqq1 = dqq.Region(0, pivot - 1)
    dqq2 = dqq.Region(pivot, totalDof - 1)    
    
    dive.IterativBiCGStab(dqq2, M22, - dt * (g2 + 1.0 / 2.0 * dt * j2), 1.0e-8)

    dqq.Region(0, pivot - 1, dqq1)
    dqq.Region(pivot, totalDof - 1, dqq2)

    velocity.problem.UpdateMeshValuesMomentum(q + dqq)

    return     

def Transient():
    t =  timer.GetInitialTime()
    dt =  timer.GetStepSize()    

    while t != timer.GetEndTime():        
        print("Solving time step (velocity) = {0}".format(t))

        p = fluid.cavity.pressure.problem.p()
        dp = fluid.dive.Vector(p.GetRows(), 0.0)
        
        q = fluid.cavity.momentum.problem.q()
        dq = dt * fluid.cavity.momentum.Derivative(q, t)

        print("Solving time step (pressure) = {0}".format(t))

        dp0 = dp
        while True:
            dp1 = dt * fluid.cavity.pressure.Derivative(q, dq, p + dp0, t)
            
            error = fluid.dive.NormP2(dp1 - dp0) / fluid.dive.NormP2(dp1)
            if (error < 0.001):
                dp = dp1
                break
            
            dp0 = dp1

        print("Solving time step (correction) = {0}".format(t))
        dqq = dt * fluid.cavity.momentum.Correction(p, dp, t)

        print("Solving time step (update) = {0}".format(t))
        fluid.cavity.momentum.problem.UpdateMeshValues(q + dq + dqq)
        fluid.cavity.pressure.problem.UpdateMeshValues(p + dp)

        timer.SetNextStep()
        t =  timer.GetCurrentTime()

        fluid.plot.FieldVectorNorm(fluid.solver.meshPressure.GetNodes())
        fluid.plot.FieldVectorNorm(fluid.solver.meshVelocity.GetNodes())

    return

def EulerExplicit(y0, dy0, dt):
    y1 = y0 + dt * dy0
    return y1

"""