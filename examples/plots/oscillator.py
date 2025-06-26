import dive
import math
import numpy as np
import matplotlib.pyplot as plt

def Show(t, f) -> None:

    t, f = zip(*sorted(zip(t, f)))
    #t, dfdt = zip(*sorted(zip(t, dfdt)))

    fig, ax = plt.subplots()
    ax.grid(True)    
    ax.set_title("Mass-Spring Oscillator")

    plt.xlim(min(t), max(t))
    #ax.grid(which = "both", linestyle = "dashed")
    #ax.grid(which = "major", linewidth = 1)
    #ax.grid(which = "minor", linewidth = 0.2)
    #ax.minorticks_on()
    plt.plot(t, f)   
    #plt.plot(t, dfdt)   
    plt.show()

    return