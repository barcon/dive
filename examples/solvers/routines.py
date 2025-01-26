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

def EulerExplicit(y0, dy0, dt):
    y1 = y0 + dt * dy0
    return y1