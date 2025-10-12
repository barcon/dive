import matplotlib.pyplot as plt
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

def Show(monitor):
    fig, ax = plt.subplots()

    ax.xaxis.set_major_locator(MultipleLocator(25))
    ax.xaxis.set_minor_locator(MultipleLocator(5))
    ax.yaxis.set_minor_locator(MultipleLocator(10))

    ax.grid(True)
    ax.grid(which='minor', linestyle=':', linewidth=0.5, color='blue', alpha=0.5)       
    ax.minorticks_on()
    
    plt.title('Solution convergence')
    plt.plot(monitor.iteration, monitor.residual)
    
    xlim = max(monitor.iteration)
    
    if(xlim % 25 != 0):
        xlim = 25 * (xlim // 25 + 1)

    plt.xlim(0, xlim)
    plt.xlabel('Iteration [--]')

    #plt.ylim(1.0e-7, 1.0e2)
    plt.ylabel('Residual')
    plt.yscale('log')
    
    plt.show()

    return