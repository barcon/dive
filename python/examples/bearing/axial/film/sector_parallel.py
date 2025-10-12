import dive
import math

from dataclasses import dataclass, field

@dataclass
class SectorParallel:
    segment = None
    fileName: str = ""
    initialThickness: float = 0.1

    def __init__(self, segment):
        self.segment = segment

    def GetPoints(self):
        points = []

        radiusInner = (self.segment.diameter - self.segment.size) / 2.0
        radiusOuter = (self.segment.diameter + self.segment.size) / 2.0
        alpha1 = math.pi / self.segment.number

        x2 = radiusInner * math.cos(alpha1)
        y2 = radiusInner * math.sin(alpha1)

        aux =  radiusInner * radiusInner - (self.segment.groove * self.segment.groove) / 4.0
        if (aux >= 0):
            xt = math.sqrt(aux)
            yt = -self.segment.groove / 2.0

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

        aux =  radiusOuter * radiusOuter - (self.segment.groove * self.segment.groove) / 4.0
        if (aux >= 0):
            xt = math.sqrt(aux)
            yt = -self.segment.groove / 2.0

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

        if self.segment.chamfer == None:
            xt = (x8 + x3) / 2.0
            yt = (y8 + y3) / 2.0

            xa = self.segment.diameter / 2.0
            ya = 0.0

            aux = (xt * xa + yt * ya) / (math.sqrt(xt * xt + yt * yt) * math.sqrt(xa * xa + ya * ya))
            alpha2 = math.acos(aux)
            alpha3 = alpha2 / 2.0

            x12 = radiusInner * math.cos(alpha3)
            y12 = radiusInner * math.sin(alpha3)

            x13 = radiusOuter * math.cos(alpha3)
            y13 = radiusOuter * math.sin(alpha3)

            x14 = x12
            y14 = -y12

            x15 = x13
            y15 = -y13
        elif self.segment.chamfer == "parallel":
            aux =  radiusInner * radiusInner - (self.segment.chamferLength + self.segment.groove / 2.0) * (self.segment.chamferLength + self.segment.groove / 2.0)
            if (aux >= 0):
                xt = math.sqrt(aux)
                yt = -(self.segment.chamferLength + self.segment.groove / 2.0)

                x12 = xt * math.cos(alpha1) - yt * math.sin(alpha1)
                y12 = xt * math.sin(alpha1) + yt * math.cos(alpha1)

            aux =  radiusOuter * radiusOuter - (self.segment.chamferLength + self.segment.groove / 2.0) * (self.segment.chamferLength + self.segment.groove / 2.0)
            if (aux >= 0):
                xt = math.sqrt(aux)
                yt = -(self.segment.chamferLength + self.segment.groove / 2.0)

                x13 = xt * math.cos(alpha1) - yt * math.sin(alpha1)
                y13 = xt * math.sin(alpha1) + yt * math.cos(alpha1)

            x14 = x12
            y14 = -y12

            x15 = x13
            y15 = -y13
        elif self.segment.chamfer == "radial":
            xt = (x8 + x3) / 2.0
            yt = (y8 + y3) / 2.0

            xa = self.segment.diameter / 2.0
            ya = 0.0

            aux = (xt * xa + yt * ya) / (math.sqrt(xt * xt + yt * yt) * math.sqrt(xa * xa + ya * ya))
            alpha2 = math.acos(aux)
            alpha3 = alpha2 - math.pi * self.segment.chamferLength / 180.0

            x12 = radiusInner * math.cos(alpha3)
            y12 = radiusInner * math.sin(alpha3)

            x13 = radiusOuter * math.cos(alpha3)
            y13 = radiusOuter * math.sin(alpha3)

            x14 = x12
            y14 = -y12

            x15 = x13
            y15 = -y13

        if self.segment.bidirectional == False:
            points.append(dive.CreateNode(0, x0, y0, 0.0))
            points.append(dive.CreateNode(1, x1, y1, 0.0))
            points.append(dive.CreateNode(2, x2, y2, 0.0))
            points.append(dive.CreateNode(3, x3, y3, 0.0))
            points.append(dive.CreateNode(4, x4, y4, 0.0))
            points.append(dive.CreateNode(5, x5, y5, -self.segment.chamferDepth))
            points.append(dive.CreateNode(6, x6, y6, -self.segment.chamferDepth))
            points.append(dive.CreateNode(7, x7, y7, 0.0))
            points.append(dive.CreateNode(8, x8, y8, 0.0))
            points.append(dive.CreateNode(9, x9, y9, 0.0))
            points.append(dive.CreateNode(10, x10, y10, -self.segment.chamferDepth))
            points.append(dive.CreateNode(11, x11, y11, -self.segment.chamferDepth))
            points.append(dive.CreateNode(12, x12, y12, 0.0))
            points.append(dive.CreateNode(13, x13, y13, 0.0))
            points.append(dive.CreateNode(14, x14, y14, 0.0))
            points.append(dive.CreateNode(15, x15, y15, 0.0))

            points.append(dive.CreateNode(16, x0, y0, self.initialThickness))
            points.append(dive.CreateNode(17, x1, y1, self.initialThickness))
            points.append(dive.CreateNode(18, x2, y2, self.initialThickness))
            points.append(dive.CreateNode(19, x3, y3, self.initialThickness))
            points.append(dive.CreateNode(20, x4, y4, self.initialThickness))
            points.append(dive.CreateNode(21, x5, y5, self.initialThickness))
            points.append(dive.CreateNode(22, x6, y6, self.initialThickness))
            points.append(dive.CreateNode(23, x7, y7, self.initialThickness))
            points.append(dive.CreateNode(24, x8, y8, self.initialThickness))
            points.append(dive.CreateNode(25, x9, y9, self.initialThickness))
            points.append(dive.CreateNode(26, x10, y10, self.initialThickness))
            points.append(dive.CreateNode(27, x11, y11, self.initialThickness))
            points.append(dive.CreateNode(28, x12, y12, self.initialThickness))
            points.append(dive.CreateNode(29, x13, y13, self.initialThickness))
            points.append(dive.CreateNode(30, x14, y14, self.initialThickness))
            points.append(dive.CreateNode(31, x15, y15, self.initialThickness))
        else:
            points.append(dive.CreateNode(0, x0, y0, 0.0))
            points.append(dive.CreateNode(1, x1, y1, 0.0))
            points.append(dive.CreateNode(2, x2, y2, -self.segment.chamferDepth))
            points.append(dive.CreateNode(3, x3, y3, -self.segment.chamferDepth))
            points.append(dive.CreateNode(4, x4, y4, 0.0))
            points.append(dive.CreateNode(5, x5, y5, -self.segment.chamferDepth))
            points.append(dive.CreateNode(6, x6, y6, -self.segment.chamferDepth))
            points.append(dive.CreateNode(7, x7, y7, -self.segment.chamferDepth))
            points.append(dive.CreateNode(8, x8, y8, -self.segment.chamferDepth))
            points.append(dive.CreateNode(9, x9, y9, 0.0))
            points.append(dive.CreateNode(10, x10, y10, -self.segment.chamferDepth))
            points.append(dive.CreateNode(11, x11, y11, -self.segment.chamferDepth))
            points.append(dive.CreateNode(12, x12, y12, 0.0))
            points.append(dive.CreateNode(13, x13, y13, 0.0))
            points.append(dive.CreateNode(14, x14, y14, 0.0))
            points.append(dive.CreateNode(15, x15, y15, 0.0))

            points.append(dive.CreateNode(16, x0, y0, self.initialThickness))
            points.append(dive.CreateNode(17, x1, y1, self.initialThickness))
            points.append(dive.CreateNode(18, x2, y2, self.initialThickness))
            points.append(dive.CreateNode(19, x3, y3, self.initialThickness))
            points.append(dive.CreateNode(20, x4, y4, self.initialThickness))
            points.append(dive.CreateNode(21, x5, y5, self.initialThickness))
            points.append(dive.CreateNode(22, x6, y6, self.initialThickness))
            points.append(dive.CreateNode(23, x7, y7, self.initialThickness))
            points.append(dive.CreateNode(24, x8, y8, self.initialThickness))
            points.append(dive.CreateNode(25, x9, y9, self.initialThickness))
            points.append(dive.CreateNode(26, x10, y10, self.initialThickness))
            points.append(dive.CreateNode(27, x11, y11, self.initialThickness))
            points.append(dive.CreateNode(28, x12, y12, self.initialThickness))
            points.append(dive.CreateNode(29, x13, y13, self.initialThickness))
            points.append(dive.CreateNode(30, x14, y14, self.initialThickness))
            points.append(dive.CreateNode(31, x15, y15, self.initialThickness))            

        return points      