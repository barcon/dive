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
class SegmentSectorParallel:
    type = Shape.sectorParallel
    chamfer = None
    chamferDepth = 0.0
    chamferSize = 0.0
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
    
    def SetChamferParallel(self, chamfer, size, depth):
        self.chamfer = Chamfer.radial
        self.chamferDepth = depth
        self.chamferSize = size 
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

            X12 = RadiusInner * Cos(Alpha3)
            Y12 = RadiusInner * Sin(Alpha3)

            X13 = RadiusOuter * Cos(Alpha3)
            Y13 = RadiusOuter * Sin(Alpha3)

            X14 = X12
            Y14 = -Y12

            X15 = X13
            Y15 = -Y13


      TSegmentBevel.sbParallel:
      begin

        Aux :=  RadiusInner * RadiusInner - (FChamfer.Length + FGroove / 2.0) * (FChamfer.Length + FGroove / 2.0);
        if (Aux >= 0) then
        begin
          Xt := sqrt(Aux);
          Yt := -(FChamfer.Length + FGroove / 2.0);

          X12 := Xt * Cos(Alpha1) - Yt * Sin(Alpha1);
          Y12 := Xt * Sin(Alpha1) + Yt * Cos(Alpha1);
        end;

        Aux :=  RadiusOuter * RadiusOuter - (FChamfer.Length + FGroove / 2.0) * (FChamfer.Length + FGroove / 2.0);
        if (Aux >= 0) then
        begin
          Xt := sqrt(Aux);
          Yt := -(FChamfer.Length + FGroove / 2.0);

          X13 := Xt * Cos(Alpha1) - Yt * Sin(Alpha1);
          Y13 := Xt * Sin(Alpha1) + Yt * Cos(Alpha1);
        end;

        X14 := X12;
        Y14 := -Y12;

        X15 := X13;
        Y15 := -Y13;
      end;
      TSegmentBevel.sbRadial:
      begin
        Xt := (X8 + X3) / 2.0;
        Yt := (Y8 + Y3) / 2.0;

        Xa := FDiameter / 2.0;
        Ya := 0.0;

        Aux := (Xt * Xa + Yt * Ya) / (sqrt(Xt * Xt + Yt * Yt) * sqrt(Xa * Xa + Ya * Ya));
        Alpha2 := ArcCos(Aux);
        Alpha3 := Alpha2 - Pi * FChamfer.Length / 180.0;

        X12 := RadiusInner * Cos(Alpha3);
        Y12 := RadiusInner * Sin(Alpha3);

        X13 := RadiusOuter * Cos(Alpha3);
        Y13 := RadiusOuter * Sin(Alpha3);

        X14 := X12;
        Y14 := -Y12;

        X15 := X13;
        Y15 := -Y13;
      end;
    end;        

        return corners
    
@dataclass
class SegmentSectorRadial:
    type = Shape.sectorRadial
    chamfer = None
    chamferDepth = 0.0
    chamferSize = 0.0
    chamferAngle = 0.0
    unidirectional: bool = True
    number : int = 1
    diameter : float = 0.0
    size : float = 0.0
    thickness: float = 0.0
    angle: float = 0.0
 
    def SetChamferNone(self):
        self.chamfer = Chamfer.none
        return
    
    def SetChamferParallel(self, chamfer, size, depth):
        self.chamfer = Chamfer.radial
        self.chamferDepth = depth
        self.chamferSize = size 
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
            area = (math.pi * (Do**2 - Di**2) / 4.0) * (self.angle / (2.0 * math.pi))

        return area