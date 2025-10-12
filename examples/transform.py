#!/usr/bin/env python

# Example for turbine distributor pressure field with 24x wicket gates
#
# fileIn contains a pressure field sector (15°): 
# x1 y1 z1 p1
# x2 y2 z2 p2
# .. .. .. ..
# xi yi zi pi
#
# fileOut contains a pressure field (360°) with pressure clipped 
# by water vapour pressure and added tailwater pressure
#----------------------------------------------------------------------------------

import dive
import math

fileIn  = 'fem.BANDWG'
fileOut = 'fem.BANDWG.out'
status  = 0
 
g       = 9.8081    # Gravity
rho     = 999.4     # Water density
h_ATM   = 10.08     # Atmosphere pressure
h_VAP   = 0.15      # water vapor pressure
KMLa    = 208.4     # Runner centerline elevation
TWL     = 214.0     # Tail water elevation

p_TWL   = rho * g * (TWL - KMLa)
p_ATM   = rho * g * h_ATM
p_VAP   = rho * g * h_VAP

data    = dive.Matrix()
status  = dive.ReadFromFile(data, fileIn)

if status != 0:
    print("Error while reading pressure field")
    quit()

data    = dive.TableValuesAdd(data, p_TWL )                #Add tail water pressure
data    = dive.TableValuesAdd(data, p_ATM )                #Convert to absolute pressure
data    = dive.TableValuesClipSmallerThan(data, p_VAP, 3)  #Clip pressure below vapor pressure
data    = dive.TableValuesAdd(data, -p_ATM )               #Convert to relativ pressure

output  = dive.Matrix(data)

z = 24
step = 360.0/z
angles = [i * step for i in range(1, z, 1)] 

for angle in angles:  
    dataRotated = dive.TablePointsRotate(data, dive.axis_z, math.radians(angle))
    output = dive.TableAppend(output, dataRotated)

dive.WriteToFile(output, fileOut)