import dive
import math
import numpy as np
import matplotlib.pyplot as plt

def AddCurve(nodes, axis = 0, dof = 0):
    x = []
    f = []
    for node in nodes:
        x.append(node.GetPoint()[axis])       
        f.append(node.GetValue(dof))

    x, f = zip(*sorted(zip(x, f)))
    #ax = plt.subplots()
    #ax.set_title(title)
    plt.xlim(x[0], x[-1])
    plt.ylim(min(f), max(f))
    #ax.grid(which = "both", linestyle = "dashed")
    #ax.grid(which = "major", linewidth = 1)
    #ax.grid(which = "minor", linewidth = 0.2)
    #ax.minorticks_on()
    plt.plot(x, f)
   
    #plt.show()

    return

def Field(nodes, dof = 0, title = ''):
    x = []
    y = []
    f = []
    for node in nodes:
        norm = dive.NormP2(node.GetValue())
        x.append(node.GetPoint()[0])
        y.append(node.GetPoint()[1])
        f.append(node.GetValue(dof))

    fig, ax = plt.subplots()
    ax.set_title(title)
    #ax.axis([0.0, 1.0, 0.0, 1.0])    
    ax.plot(x, y, 'o', markersize = 1, color = 'grey')
    ax.tricontourf(x, y, f)
    #fig.colorbar(c)
        
    #fig = plt.figure(figsize=(10, 10))
    #ax = fig.add_subplot(projection='3d')
    #ax.scatter(x, y, f);
    
    plt.show()

    return

def FieldVector(nodes):
    x = []
    y = []
    u = []
    v = []
    for node in nodes:
        x.append(node.GetPoint()[0])
        y.append(node.GetPoint()[1])
        u.append(node.GetValue(0))
        v.append(node.GetValue(1))

    fig, ax = plt.subplots()
    #ax.axis([0.0, 1.0, 0.0, 1.0])    
    ax.plot(x, y, 'o', markersize=0, color='grey')
    ax.quiver(x, y, u, v)
 
    plt.show()
    plt.close() 
    return

def FieldVectorNorm(nodes):
    x = []
    y = []
    f = []
    for node in nodes:
        x.append(node.GetPoint()[0])
        y.append(node.GetPoint()[1])

        dof = node.GetNumberDof()
        value = 0.0
        for i in range(0, dof):
            value += node.GetValue(i)**2.0

        f.append(math.sqrt(value))

    fig, ax = plt.subplots()
    #ax.axis([0.0, 1.0, 0.0, 1.0])    
    #ax.plot(x, y, 'o', markersize=0, color='grey')
    ax.tricontourf(x, y, f)

    plt.show()
    plt.close() 
    return


