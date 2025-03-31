import dive

from dataclasses import dataclass, field
from enum import Enum

#@dataclass
#class Project:
#    name : str = ""
#    user : str = ""
#    revision : str = ""
#    workspace: str = ""
#    date : None

class Shape(Enum):
    sectorParallel = 1
    sectorRadial = 2

class Chamfer(Enum):
    none = 0
    parallel = 1
    radial = 2

@dataclass
class Segment:
    shape = None
    chamfer = None
    unidirectional: bool = True
    number : int = 4
    diameter : float = 0.0
    size : float = 0.0
    thickness: float = 0.0
    groove: float = 0.0
    angle: float = 0.0

    def SetShape(self, shape):
        if shape == Shape.sectorParallel.value:
            self.shape = Shape.sectorParallel
        elif shape == Shape.sectorRadial.value:
            self.shape = Shape.sectorRadial
        return
    
    def SetChamfer(self, chamfer):
        if chamfer == Chamfer.none.value:
            self.chamfer = Chamfer.none
        elif chamfer == Chamfer.parallel.value:
            self.chamfer = Chamfer.parallel
        elif chamfer == Chamfer.radial.value:
            self.chamfer = Chamfer.radial
        return