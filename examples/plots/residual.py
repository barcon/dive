
import matplotlib.pyplot as plt
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

from dataclasses import dataclass, field

@dataclass
class Monitor:
    name: str
    iteration : list[float] = field(default_factory = list)
    residual : list[float] = field(default_factory = list)

    def Add(self, iteration, residual):
        self.iteration.append(iteration)
        self.residual.append(residual)
        return

def Show(monitor):
    fig, ax = plt.subplots()

    ax.xaxis.set_minor_locator(MultipleLocator(25))
    ax.yaxis.set_minor_locator(MultipleLocator(10))

    ax.grid(True)
    ax.grid(which='minor', linestyle=':', linewidth=0.5, color='blue', alpha=0.5)       
    ax.minorticks_on()

    plt.plot(monitor.iteration, monitor.residual)
    
    plt.xlim(0, 150)
    plt.xlabel('Iteration [--]')
    
    plt.ylim(1.0e-7, 1.0e1)
    plt.ylabel('Residual')
    plt.yscale('log')
    
    plt.show()

    return