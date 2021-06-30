#include "math.hpp"


Octree::Octree(float minSize) : BoundingCube(glm::vec3(0,0,0), minSize){
	this->root = new OctreeNode(glm::vec3(minSize*0.5));
	this->minSize = minSize;
}

int clamp(int val, int min, int max) {
	return val < min ? min : val > max ? max : val;
}

BoundingCube getChildCube(BoundingCube cube, int i) {
	float newLength = 0.5*cube.getLength();
    return BoundingCube(cube.getMin() + newLength * Octree::getShift(i), newLength);
}

int getNodeIndex(glm::vec3 vec, BoundingCube cube, bool checkBounds) {
	if(checkBounds && !cube.contains(vec)) {
		return -1;
	}
	glm::vec3 diff = (vec - cube.getMin()) / cube.getLength();
	int px = clamp(round(diff[0]), 0, 1);
	int py = clamp(round(diff[1]), 0, 1);
	int pz = clamp(round(diff[2]), 0, 1);
	return px * 4 + py * 2 + pz;
}

OctreeNode * Octree::getNodeAt(glm::vec3 pos, int level) {
	OctreeNode * node = root;
	BoundingCube cube = *this;

	while(node!= NULL && level>0){
	    unsigned int i = getNodeIndex(pos, cube, true);
	  	if(i == -1) {
	  		return NULL;
	  	}
	    /*if(node->children[i] == NULL){
	    	break;
	    }*/
	    
	    cube = getChildCube(cube, i);
	    node = node->children[i];
	    --level;
	}

	return level == 0 ? node : NULL;
}

bool isEmpty(OctreeNode * node){
	for(int i=0; i < 8 ; ++i){
		if(node->children[i] != NULL) {
			return false;
		}
	}
	return true;
}

void Octree::expand(ContainmentHandler * handler) {
	while (true) {
		Vertex vertex(getCenter());
		ContainmentResult cont = handler->check(*this, &vertex);
	    if (cont.type == ContainmentType::Contains) {
	        break;
	    }
	
	    unsigned int i = 7 - getNodeIndex(handler->getCenter(), *this, false);

	    setMin(getMin() -  Octree::getShift(i) * getLength());
	    setLength(getLength()*2);

	    OctreeNode * newNode = new OctreeNode(getCenter());
	    if(isEmpty(root)) {
	    	delete root;
	    }
	    else {
			newNode->setChild(i, root);
	    }
	    root = newNode;
	}
}

bool canSplit(BoundingCube cube, float minSize){
	return cube.getLength()*0.5 > minSize;
}


OctreeNode * addAux(ContainmentHandler * handler, OctreeNode * node, BoundingCube cube, float minSize) {

	Vertex vertex(cube.getCenter());
	ContainmentResult check = handler->check(cube, &vertex);
	bool isLeaf = !canSplit(cube, minSize);


	if(check.type != ContainmentType::Disjoint) {
		if(node == NULL) {
			node = new OctreeNode(cube.getCenter());
		}

		node->vertex = vertex;
		node->mask |= check.mask;
	    node->leaf = isLeaf;

	    if(node->mask == 0xff) {
	    	node->clear();
	    	return node;
	    }

		if(!isLeaf) {
			for(int i=0; i <8 ; ++i) {
				BoundingCube subCube = getChildCube(cube,i);
				node->children[i] = addAux(handler, node->children[i], subCube, minSize);
			}	
		}
	}
	return node;
}

void split(OctreeNode * node, BoundingCube cube) {
	for(int i=0; i <8 ; ++i) {
		BoundingCube subCube = getChildCube(cube,i);
		node->children[i] = new OctreeNode(subCube.getCenter());
		node->children[i]->mask = 0xff;
	}	
}


OctreeNode * delAux(ContainmentHandler * handler, OctreeNode * node, BoundingCube cube, float minSize, unsigned char parentMask) {
	Vertex vertex(cube.getCenter());
	ContainmentResult check = handler->check(cube, &vertex);

	bool isLeaf = !canSplit(cube, minSize);

	if(check.type != ContainmentType::Disjoint) {
		bool isContained = check.type == ContainmentType::IsContained;
		bool isIntersecting = check.type == ContainmentType::Intersects;



		// Any full containment results in cleaning
		if(node == NULL && parentMask == 0xff) {
			node = new OctreeNode(cube.getCenter());
			node->mask = 0xff;
		}


		if(node!= NULL) {
			if(node->mask == 0xff) {
				split(node, cube);
			}	

			node->mask &= (0xff ^ check.mask);
		    node->leaf = isLeaf;

			if(isContained) {
				node->clear();
				delete node;
				return NULL;
			}
	

			if(parentMask != 0x0 && (isContained || isIntersecting)) {
				node->vertex = vertex;
				node->vertex.normal = -node->vertex.normal;
			}
		
			if(!isLeaf) {
				for(int i=0; i <8 ; ++i) {
					BoundingCube subCube = getChildCube(cube,i);
					node->children[i] = delAux(handler, node->children[i], subCube, minSize, node->mask);
				}	
			}

		} 
	}
	return node;
}

void Octree::add(ContainmentHandler * handler) {
	expand(handler);	
	root = addAux(handler, root, *this, minSize);
}

void Octree::del(ContainmentHandler * handler) {
	root = delAux(handler, root, *this, minSize, 0x0);
}

void iterateAux(IteratorHandler * handler, int level, OctreeNode * node, BoundingCube cube) {
	if(node != NULL) {
		handler->iterate(level,node, cube);
		for(int i=0; i <8 ; ++i) {
			iterateAux(handler, level+1,node->children[i], getChildCube(cube,i));
		}
	}
}

void Octree::iterate(IteratorHandler * handler) {
	BoundingCube cube(glm::vec3(getMinX(),getMinY(),getMinZ()),getLength());
	iterateAux(handler, 0, root, cube);
}



void saveAux(std::ofstream * myfile, OctreeNode * node) {
	if(node!= NULL) {
		Vertex vertex = node->vertex;
		*myfile << "\n{";
		*myfile << "\"v\":" << node->vertex.toString() << ",";
		*myfile << "\"m\":" << (int) node->mask << ",";
		*myfile << "\"l\":" << (node->leaf ? "1" : "0");
		if(node->children != NULL) {
			for(int i=0; i <8 ; ++i) {
				OctreeNode * c = node->children[i];
				if(c != NULL) {
					*myfile << ",";
					*myfile << "\"" << i << "\":";
					saveAux(myfile, c);
				}
			}
		}
		*myfile << "}";
	}else {
	  	*myfile << "null";
	}
}


void Octree::save(std::string filename) {
	std::ofstream myfile;
	myfile.open (filename);
	myfile << "{";
	myfile << "\"c\":[" << getMin()[0] << "," << getMin()[1] << "," << getMin()[2] << "],";
	myfile << "\"m\":" << minSize << ",";
	myfile << "\"l\":" << getLength() << ",";
	myfile << "\"r\":";
	saveAux(&myfile, root);
	myfile << "\n}";
	myfile.close();
}

glm::vec3 Octree::getShift(int i) {
	return glm::vec3( ((i >> 2) % 2) , ((i >> 1) % 2) , ((i >> 0) % 2));
}