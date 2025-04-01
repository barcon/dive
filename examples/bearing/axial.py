import dive
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