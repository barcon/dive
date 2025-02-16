import dive
import math

from dataclasses import dataclass, field

@dataclass
class Monitor:
    status: int = dive.EILIG_CONTINUE
    iteration : list[int] = field(default_factory = list)
    residual : list[float] = field(default_factory = list)

    def Add(self, iteration, residual):
        self.iteration.append(iteration)
        self.residual.append(residual)
        return

def Iterative(A, x, b):
    tolerance = 1.0e-5
    monitor = Monitor()

    def CallbackIterative(iteration, residual):
        
        if(math.isnan(residual)):
            return dive.EILIG_NOT_CONVERGED

        monitor.Add(iteration, residual)

        if(residual < tolerance):
            return dive.EILIG_SUCCESS

        return dive.EILIG_CONTINUE

    monitor.status = dive.IterativeBiCGStab(A, x, b, CallbackIterative)

    return monitor

def ForwardMethod(timer, x0, equation):
    t = timer.GetCurrentTime()
    dt = timer.GetStepSize()
    x1 = [dive.Vector(x0[0]), dive.Vector(x0[1])]
    dfdt = dive.Vector(x0[1].GetRows())

    M, f = equation(t, x0)
    monitor = Iterative(M, dfdt, f)
    
    x1[1] = x0[1] + dt * dfdt

    return x1

def BackwardMethod(timer, y0, equation):
    tolerance = 1.0e-3
    t = timer.GetCurrentTime()
    dt = timer.GetStepSize()
    y1 = [dive.Vector(y0[0]), dive.Vector(y0[1])]
    y2 = [dive.Vector(y0[0]), dive.Vector(y0[1])]
    norm = math.inf

    while (norm > tolerance):
        M, f = equation(t + dt, y1)
        dydt, monitor = Iterative(M, f)
    
        y2[1] = y0[1] + dt * dydt

        norm = dive.NormP2(y2[1] - y1[1]) / dive.NormP2(y2[1])
        y1[1] = y2[1]
       
    return y1

def CrankNicolsonMethod(timer, y0, equation):
    tolerance = 1.0e-3
    t = timer.GetCurrentTime()
    dt = timer.GetStepSize()
    y1 = [dive.Vector(y0[0]), dive.Vector(y0[1])]
    y2 = [dive.Vector(y0[0]), dive.Vector(y0[1])]
    norm = math.inf

    M, f = equation(t, y0)
    dydt0, monitor = Iterative(M, f)

    while (norm > tolerance):
        M, f = equation(t + dt, y1)
        dydt1, monitor = Iterative(M, f)
    
        y2[1] = y0[1] + dt * 0.5* (dydt1 + dydt0)

        norm = dive.NormP2(y2[1] - y1[1]) / dive.NormP2(y2[1])
        y1[1] = y2[1]
        
    return y1