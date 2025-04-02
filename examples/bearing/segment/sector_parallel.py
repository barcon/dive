import math

from dataclasses import dataclass, field
from enum import Enum

class Shape(Enum):
    sectorParallel = 1
    sectorRadial = 2

class Chamfer(Enum):
    none = 0
    parallel = 1
    radial = 2

@dataclass
class Create:
    type = Shape.sectorParallel
    chamfer = None
    chamferDepth = 0.0
    chamferLength = 0.0
    chamferAngle = 0.0
    unidirectional: bool = True
    number : int = 1
    diameter : float = 0.0
    size : float = 0.0
    thickness: float = 0.0
    groove: float = 0.0
 
    def SetChamferNone(self):
        self.chamfer = Chamfer.none
        return
    
    def SetChamferParallel(self, chamfer, length, depth):
        self.chamfer = Chamfer.radial
        self.chamferDepth = depth
        self.chamferLength = length 
        return

    def SetChamferRadial(self, chamfer, angle, depth):
        self.chamfer = Chamfer.radial
        self.chamferDepth = depth
        self.chamferAngle = angle
        return

    def GetArea(self):
        area = 0.0

        if self.number > 0:
            Do = self.diameter + self.size
            Di = self.diameter - self.size            
            area = (math.pi * (Do**2 - Di**2) / 4.0 ) / self.number - self.groove * self.size

        return area 
    
    def GetCorners(self):
        corners = []

        radiusInner = (self.diameter - self.size) / 2.0
        radiusOuter = (self.diameter + self.size) / 2.0
        alpha1 = math.pi / self.number

        x2 = radiusInner * math.cos(alpha1)
        y2 = radiusInner * math.sin(alpha1)

        aux =  radiusInner * radiusInner - (self.groove * self.groove) / 4.0
        if (aux >= 0):
            xt = math.sqrt(aux)
            yt = -self.groove / 2.0

            x3 = xt * math.cos(alpha1) - yt * math.sin(alpha1)
            y3 = xt * math.sin(alpha1) + yt * math.cos(alpha1)

        x4 = radiusInner
        y4 = 0.0

        x5 = x3
        y5 = -y3

        x6 = x2
        y6 = -y2

        x7 = radiusOuter * math.cos(alpha1)
        y7 = radiusOuter * math.sin(alpha1)

        aux =  radiusOuter * radiusOuter - (self.groove * self.groove) / 4.0
        if (aux >= 0):
            xt = math.sqrt(aux)
            yt = -self.groove / 2.0

            x8 = xt * math.cos(alpha1) - yt * math.sin(alpha1)
            y8 = xt * math.sin(alpha1) + yt * math.cos(alpha1)

        x9 = radiusOuter
        y9 = 0.0

        x10 = x8
        y10 = -y8

        x11 = x7
        y11 = -y7

        x0 = x9
        y0 = y8

        x1 = 0.0
        y1 = 0.0

        if self.chamfer == Chamfer.none:
            xt = (x8 + x3) / 2.0
            yt = (y8 + y3) / 2.0

            xa = self.diameter / 2.0
            ya = 0.0

            aux = (xt * xa + yt * ya) / (math.sqrt(xt * xt + yt * yt) * math.sqrt(xa * xa + ya * ya))
            alpha2 = math.acos(aux)
            alpha3 = alpha2 / 2.0

            x12 = radiusInner * math.cos(alpha3)
            y12 = radiusInner * math.sin(alpha3)

            x13 = radiusOuter * math.cos(alpha3)
            y13 = radiusOuter * math.Sin(alpha3)

            x14 = x12
            y14 = -y12

            x15 = x13
            y15 = -y13
        elif self.chamfer == Chamfer.parallel:
            aux =  radiusInner * radiusInner - (self.chamferLength + self.groove / 2.0) * (self.chamferLength + self.groove / 2.0)
            if (aux >= 0):
                xt = math.sqrt(aux)
                yt = -(self.chamferLength + self.groove / 2.0)

                x12 = xt * math.cos(alpha1) - yt * math.sin(alpha1)
                y12 = xt * math.sin(alpha1) + yt * math.cos(alpha1)

            aux =  radiusOuter * radiusOuter - (self.chamferLength + self.groove / 2.0) * (self.chamferLength + self.groove / 2.0)
            if (aux >= 0):
                xt = math.sqrt(aux)
                yt = -(self.chamferLength + self.groove / 2.0)

                x13 = xt * math.cos(alpha1) - yt * math.sin(alpha1)
                y13 = xt * math.sin(alpha1) + yt * math.cos(alpha1)

            x14 = x12
            y14 = -y12

            x15 = x13
            y15 = -y13
        elif self.chamfer == Chamfer.radial:
            xt = (x8 + x3) / 2.0
            yt = (y8 + y3) / 2.0

            xa = self.diameter / 2.0
            ya = 0.0

            aux = (xt * xa + yt * ya) / (math.sqrt(xt * xt + yt * yt) * math.sqrt(xa * xa + ya * ya))
            alpha2 = math.acos(aux)
            alpha3 = alpha2 - math.pi * self.chamferLength / 180.0

            x12 = radiusInner * math.cos(alpha3)
            y12 = radiusInner * math.sin(alpha3)

            x13 = radiusOuter * math.cos(alpha3)
            y13 = radiusOuter * math.sin(alpha3)

            x14 = x12
            y14 = -y12

            x15 = x13
            y15 = -y13

        if self.rotation == False:
            corners.append(TCorner.Create(x0, y0, -FThickness))
            corners.append(TCorner.Create(x1, y1, -FThickness))
            corners.append(TCorner.Create(x2, y2, -FThickness))
            corners.append(TCorner.Create(x3, y3, -FThickness))
            corners.append(TCorner.Create(x4, y4, -FThickness))
            corners.append(TCorner.Create(x5, y5, -FThickness))
            corners.append(TCorner.Create(x6, y6, -FThickness))
            corners.append(TCorner.Create(x7, y7, -FThickness))
            corners.append(TCorner.Create(x8, y8, -FThickness))
            corners.append(TCorner.Create(x9, y9, -FThickness))
            corners.append(TCorner.Create(x10, y10, -FThickness))
            corners.append(TCorner.Create(x11, y11, -FThickness))
            corners.append(TCorner.Create(x12, y12, -FThickness))
            corners.append(TCorner.Create(x13, y13, -FThickness))
            corners.append(TCorner.Create(x14, y14, -FThickness))
            corners.append(TCorner.Create(x15, y15, -FThickness))

            corners.append(TCorner.Create(X0, Y0, 0.0))
            corners.append(TCorner.Create(X1, Y1, 0.0))
            corners.append(TCorner.Create(X2, Y2, 0.0))
            corners.append(TCorner.Create(X3, Y3, 0.0))
            corners.append(TCorner.Create(X4, Y4, 0.0))
            corners.append(TCorner.Create(X5, Y5, -FChamfer.Depth))
            corners.append(TCorner.Create(X6, Y6, 0.0))
            corners.append(TCorner.Create(X7, Y7, 0.0))
            corners.append(TCorner.Create(X8, Y8, 0.0))
            corners.append(TCorner.Create(X9, Y9, 0.0))
            corners.append(TCorner.Create(X10, Y10, -FChamfer.Depth))
            corners.append(TCorner.Create(X11, Y11, 0.0))
            corners.append(TCorner.Create(X12, Y12, 0.0))
            corners.append(TCorner.Create(X13, Y13, 0.0))
            corners.append(TCorner.Create(X14, Y14, 0.0))
            corners.append(TCorner.Create(X15, Y15, 0.0))
        else:
            corners.append(TCorner.Create(X0, Y0, -FThickness))
            corners.append(TCorner.Create(X1, Y1, -FThickness))
            corners.append(TCorner.Create(X2, Y2, -FThickness))
            corners.append(TCorner.Create(X3, Y3, -FThickness))
            corners.append(TCorner.Create(X4, Y4, -FThickness))
            corners.append(TCorner.Create(X5, Y5, -FThickness))
            corners.append(TCorner.Create(X6, Y6, -FThickness))
            corners.append(TCorner.Create(X7, Y7, -FThickness))
            corners.append(TCorner.Create(X8, Y8, -FThickness))
            corners.append(TCorner.Create(X9, Y9, -FThickness))
            corners.append(TCorner.Create(X10, Y10, -FThickness))
            corners.append(TCorner.Create(X11, Y11, -FThickness))
            corners.append(TCorner.Create(X12, Y12, -FThickness))
            corners.append(TCorner.Create(X13, Y13, -FThickness))
            corners.append(TCorner.Create(X14, Y14, -FThickness))
            corners.append(TCorner.Create(X15, Y15, -FThickness))

            corners.append(TCorner.Create(X0, Y0, 0.0))
            corners.append(TCorner.Create(X1, Y1, 0.0))
            corners.append(TCorner.Create(X2, Y2, 0.0))
            corners.append(TCorner.Create(X3, Y3, -FChamfer.Depth))
            corners.append(TCorner.Create(X4, Y4, 0.0))
            corners.append(TCorner.Create(X5, Y5, -FChamfer.Depth))
            corners.append(TCorner.Create(X6, Y6, 0.0))
            corners.append(TCorner.Create(X7, Y7, 0.0))
            corners.append(TCorner.Create(X8, Y8, -FChamfer.Depth))
            corners.append(TCorner.Create(X9, Y9, 0.0))
            corners.append(TCorner.Create(X10, Y10, -FChamfer.Depth))
            corners.append(TCorner.Create(X11, Y11, 0.0))
            corners.append(TCorner.Create(X12, Y12, 0.0))
            corners.append(TCorner.Create(X13, Y13, 0.0))
            corners.append(TCorner.Create(X14, Y14, 0.0))
            corners.append(TCorner.Create(X15, Y15, 0.0))
                        
        return corners