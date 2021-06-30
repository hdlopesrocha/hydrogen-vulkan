#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <string>
#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include <map> 

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
	glm::vec2 texCoord;

    std::string toString() {
		std::stringstream ss;
		ss << pos.x <<  "," <<  pos.y <<  "," <<  pos.z <<  "," << normal.x <<  "," <<  normal.y <<  "," <<  normal.z <<  "," <<  texCoord.x <<  "," <<  texCoord.y;
		return ss.str();
    }
};

std::vector<Vertex> vertices;
std::vector<uint32_t> indices;

void loadModel(char * path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path)) {
        throw std::runtime_error(err);
    }
  	std::map <std::string, int> compactMap;        // empty map container 
  	std::cout << "{\"g\":{" << std::endl;
   
   	int sh = 0;
    for (const auto& shape : shapes) {
	    for (const auto& index : shape.mesh.indices) {
	        Vertex vertex = {};

			vertex.pos = {
			    attrib.vertices[3 * index.vertex_index + 0],
			    attrib.vertices[3 * index.vertex_index + 1],
			    attrib.vertices[3 * index.vertex_index + 2]
			};
			if(3 * index.normal_index < attrib.normals.size()) {
				vertex.normal = {
				    attrib.normals[3 * index.normal_index + 0],
				    attrib.normals[3 * index.normal_index + 1],
				    attrib.normals[3 * index.normal_index + 2]
				};
			}

			if(2 * index.texcoord_index < attrib.texcoords.size()) {
	  			vertex.texCoord = {
	               attrib.texcoords[2 * index.texcoord_index + 0],
	               1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
	            };
        	}

            if(!compactMap.count(vertex.toString())) {
            	compactMap[vertex.toString()] = compactMap.size();
		        vertices.push_back(vertex); 
            }

            int idx = compactMap[vertex.toString()];
	        indices.push_back(idx);
	    }

		std::cout << (sh++ == 0 ? "" : ",")  << "\t\""<< shape.name <<"\""<<":{" << std::endl;

		int it;
		std::cout << "\t\t\"v\":[";
	    it = 0;
		for (const auto& val : vertices) {
			std::cout << (it++ == 0 ? "" : ",") << val.pos.x << "," << val.pos.y << "," << val.pos.z;
		}
		std::cout << "]," << std::endl;

		std::cout << "\t\t\"t\":[";
	    it = 0;
	    for (const auto& val : vertices) {
			std::cout << (it++ == 0 ? "" : ",") << val.texCoord.x << "," << val.texCoord.y;
		}
		std::cout << "]," << std::endl;

		std::cout << "\t\t\"n\":[";
	    it = 0;
	    for (const auto& val : vertices) {
			std::cout << (it++ == 0 ? "" : ",") << val.normal.x << "," << val.normal.y << "," << val.normal.z;
		}
		std::cout << "]," << std::endl;

		std::cout << "\t\t\"i\":[";
	    it = 0;
	    for (const auto& val : indices) {
			std::cout << (it++ == 0 ? "" : ",") << val;
		}
		std::cout << "]" << std::endl;
		std::cout << "\t}" << std::endl;

	}
	std::cout << "}}" << std::endl;    
}


int main(int argc, char ** argv) {
	loadModel(argv[1]);
	return 0;
}
