import structural
import solvers
import plots.oscillator
import math

omega = 2.0 * math.pi
amplitude = 1.0

def Harmonic(t):
    global omega
    global amplitude

    return amplitude * math.cos(omega * t)

timer   = structural.CreateTimerStepped(1, 0.0, 5.0, 0.01)
time = []
position = []
velocity = []


def ODE(time):
    M = structural.Ellpack(1, 1)
    f = structural.Vector(1)
    
    M.SetValue(0, 0, 1.0)
    f.SetValue(0, Harmonic(time))

    return [M, f]

x = structural.Vector(1)

while(timer.GetCurrent() < timer.GetEnd()):
    time.append(timer.GetCurrent())
    position.append(x[0])
    velocity.append(Harmonic(time[-1]))
    #x = solvers.ForwardMethod(timer, x, ODE)
    #x = solvers.BackwardMethod(timer, x, ODE)
    x = solvers.CrankNicolsonMethod(timer, x, ODE)

    timer.SetNextStep()

plots.oscillator.Show(time, position, velocity)