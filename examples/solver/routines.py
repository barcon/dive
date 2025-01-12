import dive
import math

from dataclasses import dataclass, field

tolerance = 1.0e-5

@dataclass
class Monitor:
    name: str
    iteration : list[float] = field(default_factory = list)
    residual : list[float] = field(default_factory = list)

    def Add(self, iteration, residual):
        self.iteration.append(iteration)
        self.residual.append(residual)
        return

def CallbackIterative(iteration, residual):  
    global iterations
    global residuals
    global tolerance
    
    if(math.isnan(residual)):
        return dive.EILIG_NOT_CONVERGED

    monitor.Add(iteration, residual)

    if(residual < tolerance):
        return dive.EILIG_SUCCESS

    return dive.EILIG_CONTINUE

def EulerExplicit(y0, dy0, dt):    
    y1 = y0 + dt * dy0
    return y1