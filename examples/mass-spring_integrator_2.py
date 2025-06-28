import structural
import solvers
import plots.oscillator
import math

def Harmonic(t):
    amplitude = 10.0
    omega = 0.5

    return amplitude
    #return amplitude * math.cos(omega * t)

mass = 1.0
stiffness = 100.0
damping = 0.5
omega = math.sqrt(stiffness / mass)
period = 2.0 * math.pi / omega
timer   = structural.CreateTimerStepped(1, 0.0, 100.0 * period, 0.001)
time = []
position = []
velocity = []

M = structural.Ellpack(2, 2)
M.SetValue(0, 0, 1.0)
M.SetValue(1, 1, mass)

K = structural.Ellpack(2, 2)
K.SetValue(0, 1, -1.0)
K.SetValue(1, 0, stiffness)
K.SetValue(1, 1, damping)

u = structural.Vector(2, 0.0)
f = structural.Vector(2, 0.0)

def ODE(time, u):
    global M
    global K
    global f
    
    f.SetValue(1, Harmonic(time))

    return [M, -K * u + f]

while(timer.GetCurrent() < timer.GetEnd()):
    time.append(timer.GetCurrent())
    position.append(u[0])
    velocity.append(u[1])

    u = solvers.ForwardMethod(timer, u, ODE)
    #u = solvers.BackwardMethod(timer, u, ODE)
    #u = solvers.CrankNicolsonMethod(timer, u, ODE)
    timer.SetNextStep()

plots.oscillator.Show(time, position, velocity)