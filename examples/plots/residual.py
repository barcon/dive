import dive
import math
import matplotlib.pyplot as plt
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

def Show(iterations, residuals):
    fig, ax = plt.subplots()

    ax.xaxis.set_minor_locator(MultipleLocator(25))
    ax.yaxis.set_minor_locator(MultipleLocator(10))

    ax.grid(True)
    ax.grid(which='minor', linestyle=':', linewidth=0.5, color='blue', alpha=0.5)       
    ax.minorticks_on()

    plt.plot(iterations, residuals)
    
    plt.xlim(0, 150)
    plt.xlabel('Iteration [--]')
    
    plt.ylim(1.0e-7, 1.0e1)
    plt.ylabel('Residual')
    plt.yscale('log')
    
    plt.show()

    return