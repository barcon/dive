import dive
import math

from dataclasses import dataclass, field

@dataclass
class Spring:
	position: None
	length: float
	preload: float
	stiffness: float
	
@dataclass
class Mattress:
    springs : list = field(default_factory = list)
    
    def Add(self, spring):
        self.springs.append(spring)
	
	def Clear(self):
		self.springs.clear()
		
	def GetCenter():
		return 1

	def GetStiffness():
		return 1

	def GetNumber():
		return 1		
	