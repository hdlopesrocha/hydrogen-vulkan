
unsigned int getTexture2(OctreeNode * node){
	return 1;
}

class TesselatorHandler : public IteratorHandler {
		VulkanApplication *app;
		Octree * tree;
		Model3D model;
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	  	std::map <std::string, int> compactMap;        // empty map container 

		std::vector<glm::ivec3> tessOrder;
		std::vector<glm::vec2> tessTex;

		//      6-----7
		//     /|    /|
		//    4-+---5 |
		//    | 2---+-3
		//    |/    |/
		//    0-----1

	public: 
		TesselatorHandler(VulkanApplication * app, Octree * tree) {
			this->app = app;
			this->tree = tree;

			tessOrder.push_back(glm::ivec3(0,3,1));
			tessOrder.push_back(glm::ivec3(0,2,3));
			tessOrder.push_back(glm::ivec3(0,6,2));
			tessOrder.push_back(glm::ivec3(0,4,6));
			tessOrder.push_back(glm::ivec3(0,5,4));
			tessOrder.push_back(glm::ivec3(0,1,5));

			//tessOrder.push_back(glm::ivec3(7,6,4));
			//tessOrder.push_back(glm::ivec3(7,4,5));
			//tessOrder.push_back(glm::ivec3(7,5,1));
			//tessOrder.push_back(glm::ivec3(7,1,3));
			//tessOrder.push_back(glm::ivec3(7,3,2));
			//tessOrder.push_back(glm::ivec3(7,2,6));

			tessTex.push_back(glm::vec2(0,0));
			tessTex.push_back(glm::vec2(0,1));
			tessTex.push_back(glm::vec2(1,0));
		}

		void addVertex(Vertex vertex){
			if(!compactMap.count(vertex.toString())) {
				compactMap[vertex.toString()] = compactMap.size();
			    vertices.push_back(vertex); 
			}
			int idx = compactMap[vertex.toString()];
			indices.push_back(idx);
		//	std::cout << "i=" << idx << std::endl;
		}

		void iterate(int level, OctreeNode * node, BoundingCube cube) {			
			if(node->leaf){
				std::vector<OctreeNode*> corners;
				// Get corners
				corners.push_back(node);
			    for(int i=1; i < 8; ++i) {
					glm::vec3 pos = cube.getCenter() + Octree::getShift(i)*cube.getLength();
					OctreeNode * n = tree->getNodeAt(pos,level);
					corners.push_back(n);
				}

				// Tesselate
				for(int k=0; k<tessOrder.size(); ++k){
					glm::ivec3 cornerOrder = tessOrder[k];

					if(node->leaf && node->solid == ContainmentType::Intersects){
						bool accept = true;
						for(int j=0; j < 3; ++j){
							OctreeNode * n = corners[cornerOrder[j]];
							if(n==NULL || n->solid != ContainmentType::Intersects) {
								accept = false;
								break;
							}
						}
						
						if(accept) {
							for(int j=0; j < 3; ++j){
								OctreeNode * n = corners[cornerOrder[j]];
								Vertex vtx = n->vertex;
								vtx.texCoord = tessTex[j];
								addVertex(vtx);
							}
						}
					}
				}
			} 			
		}

		IndexBufferObject finalize() {
			return app->createIndexBufferObject(vertices, indices);
		}
};




class DebugHandler : public IteratorHandler {
		VulkanApplication *app;
		Octree * tree;
		Model3D model;
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	  	std::map <std::string, int> compactMap;        // empty map container 


	public: 
		DebugHandler(VulkanApplication * app, Octree * tree) {
			this->app = app;
			this->tree = tree;
		}

		void addVertex(Vertex vertex){
			if(!compactMap.count(vertex.toString())) {
				compactMap[vertex.toString()] = compactMap.size();
			    vertices.push_back(vertex); 
			}
			int idx = compactMap[vertex.toString()];
			indices.push_back(idx);
		}


		void iterate(int level, OctreeNode * node, BoundingCube cube) {
			if(node->leaf  )
			{
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,0,0), glm::vec3(0,0,1), glm::vec2(1,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,0), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,0), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,1,0), glm::vec3(0,0,1), glm::vec2(0,1), getTexture2(node)));

				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,0,1), glm::vec3(0,0,1), glm::vec2(1,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,1,1), glm::vec3(0,0,1), glm::vec2(0,1), getTexture2(node)));


				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,0,0), glm::vec3(0,0,1), glm::vec2(1,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,0,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,0,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec2(0,1), getTexture2(node)));

				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,1,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,0), glm::vec3(0,0,1), glm::vec2(1,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,1,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,1,1), glm::vec3(0,0,1), glm::vec2(0,1), getTexture2(node)));

				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec2(1,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,1,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,1,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(0,1,0), glm::vec3(0,0,1), glm::vec2(0,1), getTexture2(node)));

				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,0,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,0,1), glm::vec3(0,0,1), glm::vec2(1,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,0,0), glm::vec3(0,0,1), glm::vec2(0,0), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec2(1,1), getTexture2(node)));
				addVertex(Vertex(cube.getMin()+cube.getLength()*glm::vec3(1,1,0), glm::vec3(0,0,1), glm::vec2(0,1), getTexture2(node)));	
			}		
		}

		IndexBufferObject finalize() {
			return app->createIndexBufferObject(vertices, indices);
		}
};