import dive
import math
import numpy as np
import matplotlib.pyplot as plt

from dataclasses import dataclass

@dataclass
class Point:
    iteration: int
    residual: float

fig = None
ax = None
dict = { }

def Monitoring(name):
    global fig
    global ax

    #plt.ion()    
    plt.subplot()

    t = np.linspace(0,2*math.pi,400)
    a = np.sin(t)
    b = np.cos(t)
    c = a + b

    plt.plot(t, a, t, b, t, c)
    plt.show()

    return