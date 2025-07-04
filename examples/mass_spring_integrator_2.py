import structural
import solvers
import plots.oscillator
import math

def Harmonic(t: float) -> float:
    amplitude = 1.0
    omega = 1.0

    return amplitude
    #return amplitude * math.cos(omega * t)

mass = 1.0
stiffness = 100.0
damping = 0.5
omega = math.sqrt(stiffness / mass)
period = 2.0 * math.pi / omega
timer   = structural.CreateTimerStepped(1, 0.0, 50.0 * period, 0.1)
time = []
position = []
velocity = []

D = structural.Ellpack(1, 1)
D[0, 0] = 1.0

M = structural.Ellpack(1, 1)
M[0, 0] = mass

C = structural.Ellpack(1, 1)
C[0, 0] = damping

K = structural.Ellpack(1, 1)
K[0, 0] = stiffness

v = structural.Vector(1, 0.0)
u = structural.Vector(1, 0.0)
f = structural.Vector(1, 0.0)

def ODE1(time, u, v):
    global M
    global C
    global K
    global f
    
    f[0] = Harmonic(time)

    return [M, -C * v -K * u + f]

def ODE2(time, v):
    global D

    return [D, v]

while(timer.GetCurrent() < timer.GetEnd()):
    time.append(timer.GetCurrent())
    position.append(u[0])
    velocity.append(v[0])

    #u, v = solvers.ForwardMethod2(timer, u, v, ODE1, ODE2)
    #u, v = solvers.BackwardMethod2(timer, u, v, ODE1, ODE2)
    u, v = solvers.CrankNicolsonMethod2(timer, u, v, ODE1, ODE2)
    timer.SetNextStep()

plots.oscillator.Show(time, position, velocity)