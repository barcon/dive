import dive
import gmsh

fileName = None

def Initialize(file):
    global fileName

    fileName = file
    gmsh.initialize()
    
    gmsh.option.setNumber("General.Terminal", 1)
    gmsh.option.setNumber("Mesh.SaveAll", 1)    
    gmsh.option.setNumber('Mesh.SecondOrderIncomplete', 1)  
    
    gmsh.model.add(fileName)

    return

def Finalize():
    global fileName

    gmsh.write(fileName)
    gmsh.finalize()

    return

def GetPhysicalGroupByName(physicalGroup):
    groups = gmsh.model.getPhysicalGroups()

    for dimension, tag in groups:
        if(physicalGroup == gmsh.model.getPhysicalName(dimension, tag)):
            return [dimension, tag]

    return [-1, -1]

def GetMeshForPhysicalGroup(meshTag, numberDof, physicalGroup):
    mesh = dive.CreateMesh(meshTag)
    dimension, tag = GetPhysicalGroupByName(physicalGroup)
    status = 0

    nodeTags, coordinates = gmsh.model.mesh.getNodesForPhysicalGroup(dimension, tag)
    for i in range(0, len(nodeTags)):
        nodeTag = int(nodeTags[i])
        x = float(coordinates[3 * i + 0])
        y = float(coordinates[3 * i + 1])
        z = float(coordinates[3 * i + 2])
        node = dive.CreateNode(nodeTag, x, y, z)
        node.SetNumberDof(numberDof)
        status = mesh.AddNode(node, status, True)
    
    mesh.SortNodesByTag()

    counter = 0
    entities = gmsh.model.getEntitiesForPhysicalGroup(dimension, tag)
    for entity in entities:
        elementTypes, elementTags, elementNodeTags = gmsh.model.mesh.getElements(dimension, entity)
        for i in range(0, len(elementTags[0])):     
            if(elementTypes[0] == 5):
                elementTag = int(elementTags[0][i])
                element = dive.CreateElementHexa(elementTag)
                element.SetOrder(dive.order_linear)
                element.SetParametric(dive.parametric_linear)
                element.SetNumberDof(numberDof)
                status = mesh.AddElement(element, status, True)
                numberNodes = element.GetNumberNodes()
                
                for k in range(0, numberNodes):
                    nodeTag = int(elementNodeTags[0][counter + k])
                    node, status = mesh.GetNodeSorted(nodeTag, status)
                    element.SetNode(k, node)

                counter += numberNodes          
            elif(elementTypes[0] == 17):
                elementTag = int(elementTags[0][i])
                element = dive.CreateElementHexa(elementTag)
                element.SetOrder(dive.order_quadratic)
                element.SetParametric(dive.parametric_quadratic)
                element.SetNumberDof(numberDof)
                status = mesh.AddElement(element, status, True)
                numberNodes = element.GetNumberNodes()
                
                for k in range(0, numberNodes):
                    nodeTag = int(elementNodeTags[0][counter + k])
                    node, status = mesh.GetNodeSorted(nodeTag, status)
                    element.SetNode(k, node)

                counter += numberNodes                
                
    mesh.SortElementsByTag()

    return mesh

def GetNodesForPhysicalGroup(mesh, physicalGroup):
    nodes = dive.vecNodes()
    dimension, tag = GetPhysicalGroupByName(physicalGroup)
    status = 0
    
    nodeTags, coordinates = gmsh.model.mesh.getNodesForPhysicalGroup(dimension, tag)
    for i in range(0, len(nodeTags)):         
        node, status = mesh.GetNodeSorted(int(nodeTags[i]), status)
        nodes.append(node)

    return nodes

def GetElementsForPhysicalGroup(mesh, physicalGroup):
    elements = dive.vecElements()
    dimension, tag = GetPhysicalGroupByName(physicalGroup)
    status = 0
    
    entities = gmsh.model.getEntitiesForPhysicalGroup(dimension, tag)
    for entity in entities:
        elementTypes, elementTags, elementNodeTags = gmsh.model.mesh.getElements(dimension, entity)
        for i in range(0, len(elementTags[0])):         
            element, status = mesh.GetElementSorted(int(elementTags[0][i]), status)
            elements.append(element)

    return elements

def GetFacesForPhysicalGroup(mesh, physicalGroup):
    elements = dive.vecElements()
    dimension, tag = GetPhysicalGroupByName(physicalGroup)
    status = 0
    
    entities = gmsh.model.getEntitiesForPhysicalGroup(dimension, tag)
    for entity in entities:
        elementTypes, elementTags, elementNodeTags = gmsh.model.mesh.getElements(dimension, entity)
        for i in range(0, len(elementTags[0])):         
            element, status = mesh.GetElementSorted(int(elementTags[0][i]), status)
            elements.append(element)

    return elements

def Entities():
    entities = gmsh.model.getEntities()

    for e in entities:
        # Dimension and tag of the entity:
        dim = e[0]
        tag = e[1]

        # Mesh data is made of `elements' (points, lines, triangles, ...), defined
        # by an ordered list of their `nodes'. Elements and nodes are identified by
        # `tags' as well (strictly positive identification numbers), and are stored
        # ("classified") in the model entity they discretize. Tags for elements and
        # nodes are globally unique (and not only per dimension, like entities).

        # A model entity of dimension 0 (a geometrical point) will contain a mesh
        # element of type point, as well as a mesh node. A model curve will contain
        # line elements as well as its interior nodes, while its boundary nodes will
        # be stored in the bounding model points. A model surface will contain
        # triangular and/or quadrangular elements and all the nodes not classified
        # on its boundary or on its embedded entities. A model volume will contain
        # tetrahedra, hexahedra, etc. and all the nodes not classified on its
        # boundary or on its embedded entities.

        # Get the mesh nodes for the entity (dim, tag):
        nodeTags, nodeCoords, nodeParams = gmsh.model.mesh.getNodes(dim, tag)

        # Get the mesh elements for the entity (dim, tag):
        elemTypes, elemTags, elemNodeTags = gmsh.model.mesh.getElements(dim, tag)

        # Elements can also be obtained by type, by using `getElementTypes()'
        # followed by `getElementsByType()'.

        # Let's print a summary of the information available on the entity and its
        # mesh.

        # * Type and name of the entity:
        type = gmsh.model.getType(dim, tag)
        name = gmsh.model.getEntityName(dim, tag)
        if len(name): name += ' '
        print("Entity " + name + str(e) + " of type " + type)

        # * Number of mesh nodes and elements:
        numElem = sum(len(i) for i in elemTags)
        print(" - Mesh has " + str(len(nodeTags)) + " nodes and " + str(numElem) +
            " elements")

        # * Upward and downward adjacencies:
        up, down = gmsh.model.getAdjacencies(dim, tag)
        if len(up):
            print(" - Upward adjacencies: " + str(up))
        if len(down):
            print(" - Downward adjacencies: " + str(down))

        # * Does the entity belong to physical groups?
        physicalTags = gmsh.model.getPhysicalGroupsForEntity(dim, tag)
        if len(physicalTags):
            s = ''
            for p in physicalTags:
                n = gmsh.model.getPhysicalName(dim, p)
                if n: n += ' '
                s += n + '(' + str(dim) + ', ' + str(p) + ') '
            print(" - Physical groups: " + s)

        # * Is the entity a partition entity? If so, what is its parent entity?
        partitions = gmsh.model.getPartitions(dim, tag)
        if len(partitions):
            print(" - Partition tags: " + str(partitions) + " - parent entity " +
                str(gmsh.model.getParent(dim, tag)))

        # * List all types of elements making up the mesh of the entity:
        for t in elemTypes:
            name, dim, order, numv, parv, _ = gmsh.model.mesh.getElementProperties(
                t)
            print(" - Element type: " + name + ", order " + str(order) + " (" +
                str(numv) + " nodes in param coord: " + str(parv) + ")")

    return

def SetNumberDof(elements, dof):
    for element in elements:
        element.SetNumberDof(dof)
    return

def ApplyMaterial(elements, material):
    for element in elements:
        element.SetMaterial(material)
    return

def ApplyField(mesh, dof, function):   
    SetNumberDof(mesh.GetElements(), dof)

    for node in mesh.GetNodes():
        point = node.GetPoint()
        value = function(point)
        node.SetValue(value)
    return