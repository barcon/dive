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

def ForwardMethod(timer, y, equation):
    t = timer.GetCurrent()
    dt = timer.GetStepSize()
    dydt = dive.Vector(y.GetRows(), 1.0)

    M, f = equation(t)
    monitor = IterativeBiCGStab(M, dydt, f)
    #print(monitor.iteration[-1], monitor.status, monitor.residual[-1])
    #dive.DiagonalLinearSystem(dydt, M, f)

    #print("{:.2e}".format(M.GetValue(0,0)), "{:.2e}".format(f.GetValue(0)), "{:.2e}".format(dydt.GetValue(0)))
    return y + dt * dydt

def BackwardMethod(timer, y, equation):
    tolerance = 1.0e-3
    t = timer.GetCurrent()
    dt = timer.GetStepSize()
    y1 = dive.Vector(y)
    y2 = dive.Vector(y)
    dydt = dive.Vector(y)
  
    norm = math.inf
    while (norm > tolerance):
        M, f = equation(t + dt)
        monitor = IterativeCG(M, dydt, f)
    
        y2 = y + dt * dydt

        norm = dive.NormP2(y2 - y1) / dive.NormP2(y2)
        y1 = y2
       
    return y1

def CrankNicolsonMethod(timer, y, equation):
    tolerance = 1.0e-3
    t = timer.GetCurrent()
    dt = timer.GetStepSize()
    y1 = dive.Vector(y)
    y2 = dive.Vector(y)
    dydt0 = dive.Vector(y)
    dydt1 = dive.Vector(y)    

    M, f = equation(t)
    monitor = IterativeCG(M, dydt0, f)

    norm = math.inf
    while (norm > tolerance):
        M, f = equation(t + dt)
        monitor = IterativeCG(M, dydt1, f)
    
        y2 = y + dt * 0.5* (dydt1 + dydt0)

        norm = dive.NormP2(y2 - y1) / dive.NormP2(y2)
        y1 = y2
        
    return y1