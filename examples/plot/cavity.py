import dive
import math
import numpy as np
import matplotlib.pyplot as plt

def HorizontalVelocity(nodes, speed):
    x = []   
    f = []

    for node in nodes:
        x.append(node.GetValue(0) / speed)  
        f.append(node.GetPoint()[1])       

    f, x = zip(*sorted(zip(f, x)))
    #ax = plt.subplots()
    #ax.set_title(title)
    plt.ylim(0.0, 1.0)
    plt.xlim(-0.4, 1.0)
    #ax.grid(which = "both", linestyle = "dashed")
    #ax.grid(which = "major", linewidth = 1)
    #ax.grid(which = "minor", linewidth = 0.2)
    #ax.minorticks_on()
    plt.plot(x, f)   
    plt.show()

    return