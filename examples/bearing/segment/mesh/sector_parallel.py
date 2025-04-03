import gmsh
import math

segment_mesh_size1 = 1
segment_mesh_size2 = 1
segment_mesh_size3 = 1

def SetSize(size1, size2, size3):
    global segment_mesh_size1
    global segment_mesh_size2
    global segment_mesh_size3
    
    segment_mesh_size1 = size1
    segment_mesh_size2 = size2
    segment_mesh_size3 = size3

    return

def Print(segment):
    points = segment.GetPoints()
    
    for i, point in enumerate(points):
        print("Point(", i + 1, ") = {", point.GetPoint()(0), ",", point.GetPoint()(1), ",",point.GetPoint()(2), "};") 

    return
   
def segment_mesh_sector(segment_fileName, segment_corners, fluid_thickness):
    global segment_mesh_size1
    global segment_mesh_size2
    global segment_mesh_size3

    gmsh.initialize()
    gmsh.option.setNumber("General.Terminal", 1)
    gmsh.model.add(segment_fileName)
    
    aux1 = len(segment_corners)

    for i in range(0, aux1):
        print("[INFO]: Adding node", gmsh.model.geo.addPoint(segment_corners[i][0], segment_corners[i][1], segment_corners[i][2] - fluid_thickness, 0, i + 1))
 
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 6, 11,  1))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(15, 16,  2))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(13, 14,  3))
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 4,  9,  4))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(22, 27,  5))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(31, 32,  6))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(29, 30,  7))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(20, 25,  8))
    
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 6, 22,  9))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(15, 31, 10))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(13, 29, 11))
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 4, 20, 12))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(11, 27, 13))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(16, 32, 14))
    print("[INFO]: Adding line", gmsh.model.geo.addLine(14, 30, 15))
    print("[INFO]: Adding line", gmsh.model.geo.addLine( 9, 25, 16))

    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc( 6,  2, 15, 17))
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(15,  2, 13, 18))
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(13,  2,  4, 19))
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(11,  2, 16, 20))
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(16,  2, 14, 21))
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(14,  2,  9, 22))
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(31, 18, 29, 23))
    print("[INFO]: Adding arc", gmsh.model.geo.addCircleArc(32, 18, 30, 24))
    
    print("[INFO]: Adding ellipse arc", gmsh.model.geo.addEllipseArc(22, 18, 18, 31, 25))    
    print("[INFO]: Adding ellipse arc", gmsh.model.geo.addEllipseArc(29, 18, 18, 20, 26))    
    print("[INFO]: Adding ellipse arc", gmsh.model.geo.addEllipseArc(27, 18, 18, 32, 27))    
    print("[INFO]: Adding ellipse arc", gmsh.model.geo.addEllipseArc(30, 18, 18, 25, 28))    

    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 1, 13,  -5,  -9],  1))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 2, 14,  -6, -10],  2))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 3, 15,  -7, -11],  3))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 4, 16,  -8, -12],  4))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 1, 20,  -2, -17],  5))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 2, 21,  -3, -18],  6))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 3, 22,  -4, -19],  7))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 5, 27,  -6, -25],  8))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 6, 24,  -7, -23],  9))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 7, 28,  -8, -26], 10))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([ 9, 25, -10, -17], 11))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([10, 23, -11, -18], 12))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([11, 26, -12, -19], 13))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([13, 27, -14, -20], 14))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([14, 24, -15, -21], 15))
    print("[INFO]: Adding loop", gmsh.model.geo.addCurveLoop([15, 28, -16, -22], 16))

    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 1], 1))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 2], 2))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 3], 3))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 4], 4))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 5], 5))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 6], 6))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 7], 7))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 8], 8))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([ 9], 9))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([10],10))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([11],11))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([12],12))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([13],13))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([14],14))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([15],15))
    print("[INFO]: Adding surface", gmsh.model.geo.addSurfaceFilling([16],16))

    print("[INFO]: Adding surface loop", gmsh.model.geo.addSurfaceLoop([1, 5, 14, 8, 11, 2], 1))
    print("[INFO]: Adding surface loop", gmsh.model.geo.addSurfaceLoop([9, 15, 6, 12, 2, 3], 2))
    print("[INFO]: Adding surface loop", gmsh.model.geo.addSurfaceLoop([10, 16, 4, 7, 13, 3], 3))
    
    print("[INFO]: Adding volume", gmsh.model.geo.addVolume([1], 1))
    print("[INFO]: Adding volume", gmsh.model.geo.addVolume([2], 2))
    print("[INFO]: Adding volume", gmsh.model.geo.addVolume([3], 3))

    gmsh.model.geo.synchronize()
    
    Xt = (segment_corners[8][0] + segment_corners[3][0]) / 2.0
    Yt = (segment_corners[8][1] + segment_corners[3][1]) / 2.0
    Xa = (segment_corners[13][0] + segment_corners[12][0]) / 2.0
    Ya = (segment_corners[13][1] + segment_corners[12][1]) / 2.0
    Aux1 = (Xt * Xa + Yt * Ya) / (math.sqrt(Xt * Xt + Yt * Yt) * math.sqrt(Xa * Xa + Ya * Ya))

    Xt = (segment_corners[13][0] + segment_corners[12][0]) / 2.0
    Yt = (segment_corners[13][1] + segment_corners[12][1]) / 2.0
    Xa = (segment_corners[15][0] + segment_corners[14][0]) / 2.0
    Ya = (segment_corners[15][1] + segment_corners[14][1]) / 2.0
    Aux2 = (Xt * Xa + Yt * Ya) / (math.sqrt(Xt * Xt + Yt * Yt) * math.sqrt(Xa * Xa + Ya * Ya))    

    segment_alpha1 = math.acos(Aux1)    
    segment_alpha2 = math.acos(Aux2)    
    segment_radius = (segment_corners[9][0] + segment_corners[4][0]) / 2.0 
    segment_length1 = segment_alpha1 * segment_radius 
    segment_length2 = segment_alpha2 * segment_radius     
    segment_width = segment_corners[9][0] - segment_corners[4][0]
    segment_height = segment_corners[20][2] - segment_corners[4][2]
     
    divisions_length1 = int(math.ceil(segment_length1 / segment_mesh_size1))
    divisions_length2 = int(math.ceil(segment_length2 / segment_mesh_size1))
    divisions_width = int(math.ceil(segment_width / segment_mesh_size2))
    divisions_height = int(math.ceil(segment_height / segment_mesh_size3))
    
    gmsh.option.setNumber('Mesh.SecondOrderIncomplete', 1)      
    gmsh.option.setNumber('Mesh.SecondOrderIncomplete', 1)      
    
    gmsh.model.mesh.setTransfiniteCurve( 1, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 2, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 5, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 6, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 3, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 7, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 4, divisions_width, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 8, divisions_width, "Progression", 1.00)

    gmsh.model.mesh.setTransfiniteCurve( 9, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(10, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(11, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(12, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(13, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(14, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(15, divisions_height, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(16, divisions_height, "Progression", 1.00)
        
    gmsh.model.mesh.setTransfiniteCurve(17, divisions_length1, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(25, divisions_length1, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(19, divisions_length1, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(26, divisions_length1, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(20, divisions_length1, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(27, divisions_length1, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(22, divisions_length1, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(28, divisions_length1, "Progression", 1.00)
    
    gmsh.model.mesh.setTransfiniteCurve(18, divisions_length2, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(23, divisions_length2, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(21, divisions_length2, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(24, divisions_length2, "Progression", 1.00)

    gmsh.model.mesh.setTransfiniteSurface( 1, "Left", [ 6, 11, 27, 22])
    gmsh.model.mesh.setTransfiniteSurface( 2, "Left", [15, 16, 32, 31])
    gmsh.model.mesh.setTransfiniteSurface( 3, "Left", [13, 14, 30, 29])
    gmsh.model.mesh.setTransfiniteSurface( 4, "Left", [ 4,  9, 25, 20])
    gmsh.model.mesh.setTransfiniteSurface( 5, "Left", [ 6, 11, 16, 15])
    gmsh.model.mesh.setTransfiniteSurface( 6, "Left", [15, 16, 14, 13])
    gmsh.model.mesh.setTransfiniteSurface( 7, "Left", [13, 14,  9,  4])
    gmsh.model.mesh.setTransfiniteSurface( 8, "Left", [22, 27, 32, 31])
    gmsh.model.mesh.setTransfiniteSurface( 9, "Left", [31, 32, 30, 29])
    gmsh.model.mesh.setTransfiniteSurface(10, "Left", [29, 30, 25, 20])
    gmsh.model.mesh.setTransfiniteSurface(11, "Left", [ 6, 22, 31, 15])
    gmsh.model.mesh.setTransfiniteSurface(12, "Left", [15, 31, 29, 13])
    gmsh.model.mesh.setTransfiniteSurface(13, "Left", [13, 29, 20,  4])
    gmsh.model.mesh.setTransfiniteSurface(14, "Left", [11, 27, 32, 16])
    gmsh.model.mesh.setTransfiniteSurface(15, "Left", [16, 32, 30, 14])
    gmsh.model.mesh.setTransfiniteSurface(16, "Left", [14, 30, 25,  9])

    gmsh.model.mesh.setTransfiniteVolume(1, [ 6, 11, 16, 15, 22, 27, 32, 31])
    gmsh.model.mesh.setTransfiniteVolume(2, [15, 16, 14, 13, 31, 32, 30, 29])
    gmsh.model.mesh.setTransfiniteVolume(3, [13, 14,  9,  4, 29, 30, 25, 20])
 
    gmsh.model.mesh.setRecombine(2, 1) 
    gmsh.model.mesh.setRecombine(2, 2) 
    gmsh.model.mesh.setRecombine(2, 3) 
    gmsh.model.mesh.setRecombine(2, 4) 
    gmsh.model.mesh.setRecombine(2, 5) 
    gmsh.model.mesh.setRecombine(2, 6) 
    gmsh.model.mesh.setRecombine(2, 7) 
    gmsh.model.mesh.setRecombine(2, 8) 
    gmsh.model.mesh.setRecombine(2, 9) 
    gmsh.model.mesh.setRecombine(2,10) 
    gmsh.model.mesh.setRecombine(2,11) 
    gmsh.model.mesh.setRecombine(2,12) 
    gmsh.model.mesh.setRecombine(2,13) 
    gmsh.model.mesh.setRecombine(2,14) 
    gmsh.model.mesh.setRecombine(2,15) 
    gmsh.model.mesh.setRecombine(2,16) 
    gmsh.model.mesh.generate(3)       
    gmsh.model.mesh.setOrder(2)    
    
    gmsh.write(segment_fileName)
    gmsh.finalize()
    
    print("[INFO]: Mesh generated. Filename:", segment_fileName) 

    return