import dive
import math
import plots.residual

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

tolerance = 1.0e-5
monitor = None

def CallbackIterative(iteration, residual):
    global tolerance
    global monitor
    
    if(math.isnan(residual)):
        return dive.EILIG_NOT_CONVERGED

    monitor.Add(iteration, residual)

    if(residual < tolerance):
        return dive.EILIG_SUCCESS

    return dive.EILIG_CONTINUE

def Iterative(A, b):
    global monitor

    y = dive.Vector(b.GetRows(), 0.0)
    monitor = Monitor()
    status = dive.IterativeBiCGStab(y, A, b, CallbackIterative)

    plots.residual.Show(monitor)

    return y, status

def EulerExplicit(y0, dy0, dt):    
    y1 = y0 + dt * dy0
    return y1