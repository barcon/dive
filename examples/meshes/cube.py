import gmsh

x = 2.0
y = 2.0
z = 2.0

nx = 2
ny = 2
nz = 2

quadratic = False

def Create(fileName):
    gmsh.initialize()
    gmsh.option.setNumber("General.Terminal", 1)
    gmsh.model.add(fileName)

    gmsh.model.geo.addPoint(-x/2.0, -y/2.0, -z/2.0, 0, 1)
    gmsh.model.geo.addPoint( x/2.0, -y/2.0, -z/2.0, 0, 2)
    gmsh.model.geo.addPoint( x/2.0,  y/2.0, -z/2.0, 0, 3)
    gmsh.model.geo.addPoint(-x/2.0,  y/2.0, -z/2.0, 0, 4)    
    gmsh.model.geo.addPoint(-x/2.0, -y/2.0,  z/2.0, 0, 5)
    gmsh.model.geo.addPoint( x/2.0, -y/2.0,  z/2.0, 0, 6)
    gmsh.model.geo.addPoint( x/2.0,  y/2.0,  z/2.0, 0, 7)
    gmsh.model.geo.addPoint(-x/2.0,  y/2.0,  z/2.0, 0, 8)

    gmsh.model.geo.addLine( 1,  2, 1)    
    gmsh.model.geo.addLine( 2,  3, 2)    
    gmsh.model.geo.addLine( 3,  4, 3)    
    gmsh.model.geo.addLine( 4,  1, 4) 
    gmsh.model.geo.addLine( 5,  6, 5)    
    gmsh.model.geo.addLine( 6,  7, 6)    
    gmsh.model.geo.addLine( 7,  8, 7)    
    gmsh.model.geo.addLine( 8,  5, 8)       
    gmsh.model.geo.addLine( 1,  5,  9)    
    gmsh.model.geo.addLine( 2,  6, 10)    
    gmsh.model.geo.addLine( 3,  7, 11)    
    gmsh.model.geo.addLine( 4,  8, 12)    

    gmsh.model.geo.addCurveLoop([ 1,  4,  3,  2], 1)
    gmsh.model.geo.addCurveLoop([ 5,  6,  7,  8], 2)
    gmsh.model.geo.addCurveLoop([ 2, 11,  -6, -10], 3)
    gmsh.model.geo.addCurveLoop([ -7, -11,  3, 12], 4)
    gmsh.model.geo.addCurveLoop([ -8, -12,  4,  9], 5)
    gmsh.model.geo.addCurveLoop([ -5, - 9,  1, 10], 6)

    gmsh.model.geo.addSurfaceFilling([ 1], 1)
    gmsh.model.geo.addSurfaceFilling([ 2], 2)
    gmsh.model.geo.addSurfaceFilling([ 3], 3)
    gmsh.model.geo.addSurfaceFilling([ 4], 4)
    gmsh.model.geo.addSurfaceFilling([ 5], 5)
    gmsh.model.geo.addSurfaceFilling([ 6], 6)

    gmsh.model.geo.addSurfaceLoop([1, 2, 3, 4, 5, 6], 1)
    gmsh.model.geo.addVolume([1], 1)
    gmsh.model.geo.synchronize()
    
    gmsh.option.setNumber('Mesh.SecondOrderIncomplete', 1)  

    gmsh.model.mesh.setTransfiniteCurve( 1, nx, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 3, nx, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 5, nx, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 7, nx, "Progression", 1.00)

    gmsh.model.mesh.setTransfiniteCurve( 2, ny, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 4, ny, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 6, ny, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve( 8, ny, "Progression", 1.00)
    
    gmsh.model.mesh.setTransfiniteCurve( 9, nz, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(10, nz, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(11, nz, "Progression", 1.00)
    gmsh.model.mesh.setTransfiniteCurve(12, nz, "Progression", 1.00)

    gmsh.model.mesh.setTransfiniteSurface( 1, "Left", [ 1,  2,  3,  4])
    gmsh.model.mesh.setTransfiniteSurface( 2, "Left", [ 5,  6,  7,  8])
    gmsh.model.mesh.setTransfiniteSurface( 3, "Left", [ 2,  3,  6,  7])
    gmsh.model.mesh.setTransfiniteSurface( 4, "Left", [ 3,  4,  7,  8])
    gmsh.model.mesh.setTransfiniteSurface( 5, "Left", [ 1,  4,  5,  8])
    gmsh.model.mesh.setTransfiniteSurface( 6, "Left", [ 1,  2,  5,  6])

    gmsh.model.mesh.setTransfiniteVolume(1, [ 1, 2, 3, 4, 5, 6, 7, 8])

    gmsh.model.mesh.setRecombine(2, 1) 
    gmsh.model.mesh.setRecombine(2, 2) 
    gmsh.model.mesh.setRecombine(2, 3) 
    gmsh.model.mesh.setRecombine(2, 4) 
    gmsh.model.mesh.setRecombine(2, 5) 
    gmsh.model.mesh.setRecombine(2, 6) 

    gmsh.model.mesh.generate(3)       
    
    if quadratic == False:
        gmsh.model.mesh.setOrder(1)   
    else:
        gmsh.model.mesh.setOrder(2)          

    gmsh.write(fileName)
    gmsh.finalize()

    return

