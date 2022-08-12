#include "vulkan/vulkan.hpp"
#include <math.h>
#include "math/math.hpp"

class HelloTriangleApplication;
#include "tesselator.hpp"

class SphereContainmentHandler : public ContainmentHandler {

	public:
		BoundingSphere sphere;
		unsigned char texture;

		SphereContainmentHandler(BoundingSphere s, unsigned char t) : ContainmentHandler(){
			this->sphere = s;
			this->texture = t;
		}

		glm::vec3 getCenter() {
			return sphere.center;
		}

		ContainmentResult check(BoundingCube cube, Vertex * vertex) {
			ContainmentResult result = sphere.contains(cube); 

			if(result.type == ContainmentType::Intersects) {
				glm::vec3 c = this->sphere.center;
				float r = this->sphere.radius;
				glm::vec3 a = cube.getCenter();
				glm::vec3 n = glm::normalize(a-c);
				glm::vec3 p = glm::clamp(c + n*r, cube.getMin(), cube.getMax());

				vertex->normal = n;
				vertex->pos = p;
				vertex->texIndex = this->texture;
			}
			return result;
		}
};

class BoxContainmentHandler : public ContainmentHandler {

	public:
		BoundingBox box;
		unsigned char texture;

		BoxContainmentHandler(BoundingBox b, unsigned char t) : ContainmentHandler(){
			this->box = b;
			this->texture = t;
		}

		glm::vec3 getCenter() {
			return box.getCenter();
		}

		ContainmentResult check(BoundingCube cube, Vertex * vertex) {
			ContainmentResult result = box.contains(cube); 
			if(result.type == ContainmentType::Intersects) {
				glm::vec3 min = this->box.getMin();
				glm::vec3 max = this->box.getMax();
				glm::vec3 c = cube.getCenter();
				glm::vec3 a = box.getCenter();
				glm::vec3 n = glm::normalize(c-a);
				vertex->normal = n;
				vertex->texIndex = this->texture;

				for(int i=0; i < 3 ; ++i) {
					if(cube.getMax()[i] >= max[i]) {
						c[i] = max[i];
					}
					if(cube.getMin()[i] <= min[i]) {
						c[i] = min[i];
					}
				}
				vertex->pos = c;
			}
			return result;
		}
};

class HeightMapContainmentHandler : public ContainmentHandler {

	public:
		HeightMap * map;
		unsigned char texture;
		unsigned char textureOut;

		HeightMapContainmentHandler(HeightMap * m, unsigned char t, unsigned char o) : ContainmentHandler(){
			this->map = m;
			this->texture = t;
			this->textureOut = o;
		}

		glm::vec3 getCenter() {
			return map->getCenter();
		}

		ContainmentResult check(BoundingCube cube, Vertex * vertex) {
			ContainmentResult result = map->contains(cube); 
			if(result.type == ContainmentType::Intersects) {
				glm::vec3 c = cube.getCenter();
				glm::vec3 a = map->getCenter();
				glm::vec3 n = glm::normalize(c-a);
				vertex->pos = c;
				vertex->normal = n;
				vertex->texIndex = this->textureOut;

				if(!map->hitsBoundary(cube)) {
					vertex->pos = map->getPointAt(cube);
					vertex->texIndex = this->texture;
				}
			}
			return result;
		}
};

class HelloTriangleApplication : public VulkanApplication {
	std::vector<Image> images;
	IndexBufferObject ibo;
	Model3D model;
	Camera camera;
	glm::mat4 modelMatrix=glm::mat4(1.0f);
	Octree * tree;
	IndexBufferObject mesh;
	#ifdef DEBUG
	IndexBufferObject debugMesh;
	#endif
public:
	HelloTriangleApplication() {

	}

	~HelloTriangleApplication() {

	}

    void loadImages() {
        images.push_back(loadTextureImage("textures/pixel.jpg"));
        images.push_back(loadTextureImage("textures/grid.png"));
        images.push_back(loadTextureImage("textures/grass.png"));
        images.push_back(loadTextureImage("textures/sand.png"));
        images.push_back(loadTextureImage("textures/rock.png"));
        images.push_back(loadTextureImage("textures/snow.png"));
        images.push_back(loadTextureImage("textures/lava.png"));
        images.push_back(loadTextureImage("textures/dirt.png"));
        images.push_back(loadTextureImage("textures/grid2.png"));

        images.push_back(loadTextureImage("textures/pixel.jpg"));
        images.push_back(loadTextureImage("textures/pixel.jpg"));
        images.push_back(loadTextureImage("textures/pixel.jpg"));
        images.push_back(loadTextureImage("textures/pixel.jpg"));
        images.push_back(loadTextureImage("textures/pixel.jpg"));
        images.push_back(loadTextureImage("textures/pixel.jpg"));
        images.push_back(loadTextureImage("textures/pixel.jpg"));
    }
 
	virtual void setup() {
	    loadImages();
	    model = loadModel("models/cube.json");
   	    camera.quaternion =   glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 0, 1))
   	    					* glm::angleAxis(glm::radians(145.0f), glm::vec3(1, 0, 0))
   	    					* glm::angleAxis(glm::radians(135.0f), glm::vec3(0, 1, 0));  
		camera.pos = glm::vec3(48,48,48);

		tree = new Octree(2.0);

		HeightMap map(glm::vec3(0,-24,0),glm::vec3(128,0,128), 8, 8);
		tree->add(new HeightMapContainmentHandler(&map, 8, 7));

		BoundingSphere sph(glm::vec3(0,0,0),20);
		tree->add(new SphereContainmentHandler(sph, 2));

		BoundingSphere sph2(glm::vec3(-11,11,11),10);
		tree->add(new SphereContainmentHandler(sph2, 5));

		BoundingSphere sph3(glm::vec3(11,11,-11),10);
		tree->del(new SphereContainmentHandler(sph3, 4));

		BoundingSphere sph4(glm::vec3(4,4,-4),8);
		tree->del(new SphereContainmentHandler(sph4, 6));

		BoundingSphere sph5(glm::vec3(11,11,-11),4);
		tree->add(new SphereContainmentHandler(sph5, 3));

		BoundingBox box1(glm::vec3(0,-24,0),glm::vec3(24,0,24));
		tree->add(new BoxContainmentHandler(box1,4));

		TesselatorHandler tesselator(this, tree);
		tree->iterate(&tesselator);
		mesh = tesselator.finalize();

		tree->save("space.json");

		#ifdef DEBUG
		DebugHandler debugTesselator(this, tree);
		tree->iterate(&debugTesselator);
		debugMesh = debugTesselator.finalize();
		#endif

		for(int i=0; i < swapChainVector.size() ; ++i){
			Swap * swap = &swapChainVector[i];
			for(int i=0 ; i < images.size(); ++i) {
				bindImage(swap, i, images[i]);		
			}	
		}
	}

	virtual void update(float deltaTime) {

	   	if (getKeyboardStatus(GLFW_KEY_ESCAPE) != GLFW_RELEASE) {
		   	close();
		}

   	    camera.projection = glm::perspective(glm::radians(45.0f), getWidth() / (float) getHeight(), 0.1f, 512.0f);
	    camera.projection[1][1] *= -1;
    
	 //   modelMatrix = glm::rotate(modelMatrix, deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	   	float rsense = 0.0005;

	   	if (getKeyboardStatus(GLFW_KEY_W) != GLFW_RELEASE) {
	   	   	camera.quaternion = glm::angleAxis(+rsense, glm::vec3(1,0,0))*camera.quaternion;
		}
	   	if (getKeyboardStatus(GLFW_KEY_S) != GLFW_RELEASE) {
	   	   	camera.quaternion = glm::angleAxis(-rsense, glm::vec3(1,0,0))*camera.quaternion;
		}
   		if (getKeyboardStatus(GLFW_KEY_A) != GLFW_RELEASE) {
	   	   	camera.quaternion = glm::angleAxis(-rsense, glm::vec3(0,1,0))*camera.quaternion;
		}
	   	if (getKeyboardStatus(GLFW_KEY_D) != GLFW_RELEASE) {
	   	   	camera.quaternion = glm::angleAxis(+rsense, glm::vec3(0,1,0))*camera.quaternion;
		}
		if (getKeyboardStatus(GLFW_KEY_Q) != GLFW_RELEASE) {
	   	   	camera.quaternion = glm::angleAxis(+rsense, glm::vec3(0,0,1))*camera.quaternion;
		}
	   	if (getKeyboardStatus(GLFW_KEY_E) != GLFW_RELEASE) {
	   	   	camera.quaternion = glm::angleAxis(-rsense, glm::vec3(0,0,1))*camera.quaternion;
		}

		glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f)*camera.quaternion;
		glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f)*camera.quaternion;
		glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f)*camera.quaternion;

	   	float tsense = deltaTime*20;
	   	if (getKeyboardStatus(GLFW_KEY_UP) != GLFW_RELEASE) {
	   		camera.pos -= zAxis*tsense;
		}
	   	if (getKeyboardStatus(GLFW_KEY_DOWN) != GLFW_RELEASE) {
	   		camera.pos += zAxis*tsense;
		}
	   	if (getKeyboardStatus(GLFW_KEY_RIGHT) != GLFW_RELEASE) {
	   		camera.pos += xAxis*tsense;
		}
   		if (getKeyboardStatus(GLFW_KEY_LEFT) != GLFW_RELEASE) {
	   		camera.pos -= xAxis*tsense;
		}

		camera.quaternion = glm::normalize(camera.quaternion);
		glm::mat4 rotate = glm::mat4_cast(camera.quaternion);
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), -camera.pos);
	    camera.view = rotate * translate;
	}

	int i=0;

	virtual void draw(Swap * swap) {	

		drawIndexBufferObject(swap, &mesh, glm::mat4(1));

		#ifdef DEBUG
		drawIndexBufferObject(swap, &debugMesh, glm::mat4(1));
		#endif
		swap->uniformBufferObject.view = camera.view;
		swap->uniformBufferObject.projection = camera.projection;
	}

	virtual void clean() {
		while(images.size() > 0) {
			images.back().destroy(device);
			images.pop_back();
		}
		for ( const auto &group : model.buffers ) {
       		cleanIndexBufferObject(group.second);
	    }
	}
};


int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	} catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}