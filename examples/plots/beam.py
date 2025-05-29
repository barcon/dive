import dive
import math
import numpy as np
import matplotlib.pyplot as plt

def Cantilever(nodes) -> None:
    x = []   
    f = []

    for node in nodes:
        x.append(node.GetPoint()[0])
        f.append(node.GetValue(2))  

    x, f = zip(*sorted(zip(x, f)))

    fig, ax = plt.subplots()
    ax.grid(True)    
    ax.set_title("Cantilever Beam Deflection")

    y0 = f[0] if f[-1] > f[0] else f[-1]
    y1 = f[-1] if f[-1] > f[0] else f[0]

    plt.ylim(y0, y1)
    plt.xlim(min(x), max(x))
    #ax.grid(which = "both", linestyle = "dashed")
    ax.grid(which = "major", linewidth = 1)
    #ax.grid(which = "minor", linewidth = 0.2)
    #ax.minorticks_on()
    plt.plot(x, f)   
    plt.show()

    return