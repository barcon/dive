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

def IterativeBiCGStab(A, x, b):
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

def IterativeCG(A, x, b):
    tolerance = 1.0e-5
    monitor = Monitor()

    def CallbackIterative(iteration, residual):
        if(math.isnan(residual)):
            return dive.EILIG_NOT_CONVERGED

        monitor.Add(iteration, residual)

        if(residual < tolerance):
            return dive.EILIG_SUCCESS
        
        return dive.EILIG_CONTINUE

    monitor.status = dive.IterativeCG(A, x, b, CallbackIterative)

    return monitor

def ForwardMethod(timer, y0, equation):
    t = timer.GetCurrentTime()
    dt = timer.GetStepSize()
    y1 = dive.Vector(y0)
    dydt = dive.Vector(y0)

    M, f = equation(t, y0)
    monitor = Iterative(M, dydt, f)
    
    y1 = y0 + dt * dydt

    return y1

def BackwardMethod(timer, y0, equation):
    tolerance = 1.0e-3
    t = timer.GetCurrentTime()
    dt = timer.GetStepSize()
    y1 = dive.Vector(y0)
    y2 = dive.Vector(y0)
    dydt = dive.Vector(y0)
  
    norm = math.inf
    while (norm > tolerance):
        M, f = equation(t + dt, y1)
        monitor = Iterative(M, dydt, f)
    
        y2 = y0 + dt * dydt

        norm = dive.NormP2(y2 - y1) / dive.NormP2(y2)
        y1 = y2
       
    return y1

def CrankNicolsonMethod(timer, y0, equation):
    tolerance = 1.0e-3
    t = timer.GetCurrentTime()
    dt = timer.GetStepSize()
    y1 = dive.Vector(y0)
    y2 = dive.Vector(y0)
    dydt0 = dive.Vector(y0)    
    dydt1 = dive.Vector(y0)    

    M, f = equation(t, y0)
    monitor = Iterative(M, dydt0, f)

    norm = math.inf
    while (norm > tolerance):
        M, f = equation(t + dt, y1)
        monitor = Iterative(M, dydt1, f)
    
        y2 = y0 + dt * 0.5* (dydt1 + dydt0)

        norm = dive.NormP2(y2 - y1) / dive.NormP2(y2)
        y1 = y2
        
    return y1