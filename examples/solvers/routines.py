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

def ForwardMethod(timer, u0, equation):
    t = timer.GetCurrent()
    dt = timer.GetStepSize()
   
    dudt = dive.Vector(u0)

    M, f = equation(t, u0)
    monitor = IterativeBiCGStab(M, dudt, f)
    
    return u0 + dt * dudt

def ForwardMethod2(timer, u0, v0, equation1, equation2):
    t = timer.GetCurrent()
    dt = timer.GetStepSize()
 
    dvdt = dive.Vector(v0)
    dudt = dive.Vector(u0)

    M, f = equation1(t, u0, v0)
    monitor = IterativeBiCGStab(M, dvdt, f)
    v1 = v0 + dt * dvdt
    
    M, f = equation2(t, v1)
    monitor = IterativeBiCGStab(M, dudt, f)
    u1 = u0 + dt * dudt

    return [u1, v1]

def BackwardMethod(timer, u0, equation):
    tolerance = 1.0e-5
    t = timer.GetCurrent()
    dt = timer.GetStepSize()

    u1 = dive.Vector(u0)
    u2 = dive.Vector(u0)
    dudt = dive.Vector(u0)
  
    norm1 = math.inf
    while (norm1 > tolerance):
        M, f = equation(t + dt, u1)
        monitor = IterativeBiCGStab(M, dudt, f)
    
        u2 = u0 + dt * dudt

        norm1 = dive.NormP2(u2 - u1)
        u1 = u2
       
    return u1

def BackwardMethod2(timer, u0, v0, equation1, equation2):
    tolerance = 1.0e-5
    t = timer.GetCurrent()
    dt = timer.GetStepSize()

    u1 = dive.Vector(u0)
    u2 = dive.Vector(u0)
    dudt = dive.Vector(u0)

    v1 = dive.Vector(v0)
    v2 = dive.Vector(v0)
    dvdt = dive.Vector(v0)
  
    norm1 = math.inf
    norm2 = math.inf
    while (norm1 > tolerance and norm2 > tolerance):
        M, f = equation1(t + dt, u0, v0)
        monitor = IterativeBiCGStab(M, dvdt, f)
        v2 = v0 + dt * dvdt
        
        M, f = equation2(t + dt, v1)
        monitor = IterativeBiCGStab(M, dudt, f)
        u2 = u0 + dt * dudt
    
        norm1 = dive.NormP2(u2 - u1)
        norm2 = dive.NormP2(v2 - v1)
        u1 = u2
        v1 = v2
       
    return [u1, v1]

def CrankNicolsonMethod(timer, u0, equation):
    tolerance = 1.0e-5
    t = timer.GetCurrent()
    dt = timer.GetStepSize()
    u1 = dive.Vector(u0)
    u2 = dive.Vector(u0)
    dudt0 = dive.Vector(u0)
    dudt1 = dive.Vector(u0)

    M, f = equation(t, u0)
    monitor = IterativeBiCGStab(M, dudt0, f)

    norm1 = math.inf
    while (norm1 > tolerance):
        M, f = equation(t + dt, u1)
        monitor = IterativeBiCGStab(M, dudt1, f)
        u2 = u0 + dt * 0.5* (dudt1 + dudt0)

        norm1 = dive.NormP2(u2 - u1)
        u1 = u2
        
    return u1

def CrankNicolsonMethod2(timer, u0, v0, equation1, equation2):
    tolerance = 1.0e-5
    t = timer.GetCurrent()
    dt = timer.GetStepSize()

    u1 = dive.Vector(u0)
    u2 = dive.Vector(u0)
    dudt0 = dive.Vector(u0)
    dudt1 = dive.Vector(u0)

    v1 = dive.Vector(v0)
    v2 = dive.Vector(v0)
    dvdt0 = dive.Vector(v0)
    dvdt1 = dive.Vector(v0)

    M, f = equation1(t, u0, v0)
    monitor = IterativeBiCGStab(M, dvdt0, f)

    M, f = equation2(t, v0)
    monitor = IterativeBiCGStab(M, dudt0, f)

    norm1 = math.inf
    norm2 = math.inf
    while (norm1 > tolerance and norm2 > tolerance):
        M, f = equation1(t + dt, u1, v1)
        monitor = IterativeBiCGStab(M, dvdt1, f)  
        v2 = v0 + dt * 0.5* (dvdt1 + dvdt0)
        
        M, f = equation2(t + dt, v2)
        monitor = IterativeBiCGStab(M, dudt1, f)
        u2 = u0 + dt * 0.5* (dudt1 + dudt0)
    
        norm1 = dive.NormP2(u2 - u1)
        norm2 = dive.NormP2(v2 - v1)
        u1 = u2
        v1 = v2
       
    return [u1, v1]