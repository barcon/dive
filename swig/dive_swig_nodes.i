%inline
%{
	#include "nodes_types.hpp"
	#include "nodes_basic.hpp"

	typedef std::shared_ptr<nodes::INode> INodePtr;
	typedef std::shared_ptr<const nodes::INode> ConstINodePtr;
	
	typedef std::shared_ptr<nodes::Node> NodePtr;
	typedef std::shared_ptr<const nodes::Node> ConstNodePtr;

	typedef std::vector<nodes::INodePtr> Nodes;	
%}

%shared_ptr(nodes::INode);
%shared_ptr(nodes::Node);

%template(vecNodes) std::vector<std::shared_ptr<nodes::INode>>;

%include "..\..\nodes\src\nodes_types.hpp"
%include "..\..\nodes\src\nodes_basic.hpp"

%pythoncode
%{

def AddVecNodes(self, nodes):
    for node in nodes:
        self.append(node)
    return self

def CompVecNodes(self, other):
    return self.GetTag() == other.GetTag()

def HashVecNodes(self):
    return hash(self.GetTag())

vecNodes.__add__ = AddVecNodes
vecNodes.__eq__ = CompVecNodes
vecNodes.__hash__ = HashVecNodes

%}