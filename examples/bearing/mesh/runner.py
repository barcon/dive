import sys
import math
from gleitermeshrunnerrectangular import *

runner_file_name = "runner.msh"
runner_mesh_size_length = 100
runner_mesh_size_width  = 100
runner_mesh_size_height = 100

def runner_mesh(shape, size1, size2, size3):
    if shape == 0:
        runner_mesh_rectangular_size(size1, size2, size3)
        runner_mesh_rectangular(runner_file_name, runner_corners)
    else:
        print("[ERROR]: Segment shape not supported")      

print("[INFO]: Generating mesh of runner")

runner_mesh(runner_shape, runner_mesh_size_length, runner_mesh_size_width, runner_mesh_size_height)