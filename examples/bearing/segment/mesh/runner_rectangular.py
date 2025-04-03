import gmsh
import math

runner_mesh_size1 = 1
runner_mesh_size2 = 1
runner_mesh_size3 = 1

def runner_mesh_rectangular_gmsh_points(runner_corners):
    aux1 = len(runner_corners)
    
    #Point(1) = {X, Y, Z, 0}; 

    for i in range(0, aux1):
        print("Point(", i + 1, ") = {", runner_corners[i][0], ",", runner_corners[i][1], ",", runner_corners[i][2], ", 0};")

def runner_mesh_rectangular_size(size1, size2, size3):
    global runner_mesh_size1
    global runner_mesh_size2
    global runner_mesh_size3
    
    runner_mesh_size1 = size1
    runner_mesh_size2 = size2
    runner_mesh_size3 = size3    
   
def runner_mesh_rectangular(runner_fileName, runner_corners):
    global runner_mesh_size1
    global runner_mesh_size2
    global runner_mesh_size3

    gmsh.initialize()
    gmsh.option.setNumber("General.Terminal", 1)
    gmsh.model.add(runner_fileName)
    
    aux1 = len(runner_corners)

    for i in range(0, aux1):
        print("[INFO]: Adding node", gmsh.model.geo.addPoint(runner_corners[i][0], runner_corners[i][1], runner_corners[i][2], 0, i + 1))

    print("[INFO]: Adding line", gmsh.model.geo.addLine( 5,  8,  1))
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 3,  6,  2))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(13, 16,  3))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(11, 14,  4))
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 5, 13,  5))
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 3, 11,  6))
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 8, 16,  7))
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 6, 14,  8))
    
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc( 5,  2,  3,  9))    
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(13, 10, 11, 10))    
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc( 8,  2,  6, 11))    
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(16, 10, 14, 12)) 

    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 1,  7, -3,  -5],  1))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 2,  8, -4,  -6],  2))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 1, 11, -2,  -9],  3))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 3, 12, -4, -10],  4))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 5, 10, -6,  -9],  5))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 7, 12, -8, -11],  6))
    
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 1], 1))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 2], 2))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 3], 3))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 4], 4))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 5], 5))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 6], 6))

    print("[INFO]: Adding surface loop", gmsh.model.geo.addSurfaceLoop([4, 1, 3, 6, 2, 5], 1))
    print("[INFO]: Adding volume", gmsh.model.geo.addVolume([1], 1))    

    gmsh.model.geo.synchronize()
    
    Xt = (runner_corners[5][0] + runner_corners[2][0]) / 2.0
    Yt = (runner_corners[5][1] + runner_corners[2][1]) / 2.0
    Xa = (runner_corners[7][0] + runner_corners[5][0]) / 2.0
    Ya = (runner_corners[7][1] + runner_corners[5][1]) / 2.0
    Aux = (Xt * Xa + Yt * Ya) / (math.sqrt(Xt * Xt + Yt * Yt) * math.sqrt(Xa * Xa + Ya * Ya))    
    
    runner_alpha = math.acos(Aux)       
    runner_radius = (runner_corners[6][0] + runner_corners[3][0]) / 2.0 
    runner_length = runner_alpha * runner_radius    
    runner_width = runner_corners[6][0] - runner_corners[3][0]
    runner_height = runner_corners[11][2] - runner_corners[3][2]
     
    divisions_length = int(math.ceil(runner_length / runner_mesh_size1))
    divisions_width = int(math.ceil(runner_width / runner_mesh_size2))
    divisions_height = int(math.ceil(runner_height / runner_mesh_size3))    

    gmsh.option.setNumber('Mesh.SecondOrderIncomplete', 1)  
  
    gmsh.model.mesh.setTransfiniteCurve( 1, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 2, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 3, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 4, divisions_width, "Progression", 1.00)

    gmsh.model.mesh.setTransfiniteCurve( 5, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 6, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 7, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 8, divisions_height, "Progression", 1.00)
    
    gmsh.model.mesh.setTransfiniteCurve( 9, divisions_length, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(10, divisions_length, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(11, divisions_length, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(12, divisions_length, "Progression", 1.00)
    
    gmsh.model.mesh.setTransfiniteSurface( 1, "Left", [ 5,  8, 16, 13])    
    gmsh.model.mesh.setTransfiniteSurface( 2, "Left", [ 3,  6, 14, 11])    
    gmsh.model.mesh.setTransfiniteSurface( 3, "Left", [ 5,  8,  6,  3])    
    gmsh.model.mesh.setTransfiniteSurface( 4, "Left", [13, 16, 14, 11])    
    gmsh.model.mesh.setTransfiniteSurface( 5, "Left", [ 5, 13, 11,  3])    
    gmsh.model.mesh.setTransfiniteSurface( 6, "Left", [ 8, 16, 14,  6])    
    
    gmsh.model.mesh.setTransfiniteVolume(1, [ 5, 8, 6, 3, 13, 16, 14, 11])    
    
    gmsh.model.mesh.setRecombine(2, 1) 
    gmsh.model.mesh.setRecombine(2, 2) 
    gmsh.model.mesh.setRecombine(2, 3) 
    gmsh.model.mesh.setRecombine(2, 4) 
    gmsh.model.mesh.setRecombine(2, 5) 
    gmsh.model.mesh.setRecombine(2, 6)  
    gmsh.model.mesh.generate(3)   
    gmsh.model.mesh.setOrder(2)      
    
    gmsh.write(runner_fileName)
    gmsh.finalize()
    
    print("[INFO]: Mesh generated. Filename:", runner_fileName) 