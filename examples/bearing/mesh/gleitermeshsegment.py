import sys
import math
from gleitermeshsegmentsector import *

fluid_thickness = 10

segment_file_name = "segment.msh"
segment_mesh_size_length = 100
segment_mesh_size_width  = 100
segment_mesh_size_height = 100

def segment_mesh(shape, rotation, chamfer, size1, size2, size3):
    if shape == 0:
        segment_mesh_sector_size(size1, size2, size3)
        segment_mesh_sector(segment_file_name, segment_corners, fluid_thickness)
    elif shape == 1:
        segment_mesh_sector_size(size1, size2, size3)
        segment_mesh_sector(segment_file_name, segment_corners, fluid_thickness)
    else:
        print("[ERROR]: Segment shape not supported")      

print("[INFO]: Generating mesh of segment")

segment_mesh(segment_shape, segment_rotation, segment_chamfer, segment_mesh_size_length, segment_mesh_size_width, segment_mesh_size_height)