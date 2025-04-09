import dive
import math

from dataclasses import dataclass, field

@dataclass
class Sector:
    segment = None
    initialThickness: float = 0.1

    def __init__(self, segment):
        self.segment = segment

    def GetPoints(self):
        tag = 0
        points = []
        segmentPoints = self.segment.GetPoints()

        for point in segmentPoints:
            tag = point.GetTag()
            x = point.GetPoint()(0)
            y = point.GetPoint()(1)
            z = point.GetPoint()(2)

            points.append(dive.CreateNode(tag, x, y, z))

        for point in segmentPoints:
            tag = point.GetTag()
            x = point.GetPoint()(0)
            y = point.GetPoint()(1)
            z = point.GetPoint()(2)

            points.append(dive.CreateNode(tag, x, y, z + self.initialThickness))            

        return points        