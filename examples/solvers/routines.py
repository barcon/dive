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

def Iterative(A, b):
    tolerance = 1.0e-5
    monitor = Monitor()
    rows = b.GetRows()

    def CallbackIterative(iteration, residual):
        
        if(math.isnan(residual)):
            return dive.EILIG_NOT_CONVERGED

        monitor.Add(iteration, residual)

        if(residual < tolerance):
            return dive.EILIG_SUCCESS

        return dive.EILIG_CONTINUE

    y = dive.Vector(rows, 0.0)
    monitor.status = dive.IterativeBiCGStab(y, A, b, CallbackIterative)

    return y, monitor

def EulerExplicit(timer, y0, equation):
    t = timer.GetCurrentTime()
    dt = timer.GetStepSize()
    y = [dive.Vector(y0[0]), dive.Vector(y0[1])]

    M, f = equation(t, y)
    dydt, monitor = Iterative(M, f)
    
    y[1] = y0[1] + dt * dydt

    return y

def EulerImplicit(timer, y0, equation):
    tolerance = 1.0e-3
    t = timer.GetCurrentTime()
    dt = timer.GetStepSize()
    y1 = [dive.Vector(y0[0]), dive.Vector(y0[1])]
    yt = [dive.Vector(y0[0]), dive.Vector(y0[1])]
    norm = math.inf

    while (norm > tolerance):
        yt[1] = dive.Vector(y1[1])

        M, f = equation(t, y1)
        dydt, monitor = Iterative(M, f)
    
        y1[1] = y0[1] + dt * dydt

        norm = dive.NormP2(y1[1] - yt[1]) / dive.NormP2(y1[1])
        print("Norm = ", norm)
        
    return y1