#include "math.hpp"

OctreeNode::OctreeNode(Vertex vertex) {
	this->vertex = vertex;
    this->solid = ContainmentType::Disjoint;
	this->leaf = false;
	for(int i=0; i < 8 ; ++i) {
		setChild(i, NULL);
	}
}

void OctreeNode::setChild(int i, OctreeNode * node) {
	this->children[i] = node;
}

OctreeNode::~OctreeNode() {
	this->solid = ContainmentType::Disjoint;;
	this->clear();
	this->leaf = false;
}

void OctreeNode::clear(){
	for(int i=0; i < 8 ; ++i){
		OctreeNode * child = children[i];
		if(child != NULL) {
			delete child;
			children[i] = NULL;
		}
	}
}



