#ifndef VULKAN_HPP
#define VULKAN_HPP

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define KEYBOARD_SIZE 1024
#define VERTEX_BUFFER_BIND_ID 0
#define INSTANCE_BUFFER_BIND_ID 1
#define MAX_FRAMES_IN_FLIGHT 2
#define WIDTH 1280
#define HEIGHT 720
//#define DEBUG 1
//#define NDEBUG 1


#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <array>
#include <map>
#include <cstring>
#include <set>
#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <chrono>
#include "rapidjson/document.h"
#include <sstream>
#include <string>
#include "../math/math.hpp"

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

struct InstanceData {
	glm::mat4 model;
};

struct UniformBufferObject {
	glm::mat4 view;
	glm::mat4 projection;
};


struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct IndexBufferObject {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    long indicesCount;
};

struct InstanceBufferObject {
    VkBuffer instanceBuffer;
    VkDeviceMemory instanceBufferMemory;
};

struct IndirectDraw {
	std::vector<InstanceData> instances;
    IndexBufferObject * indexBufferObject;
};


struct Image {
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
   	VkImageView textureImageView;
	uint32_t mipLevels;

   	void destroy(VkDevice device) {
   		vkDestroyImageView(device, textureImageView, nullptr);
   		vkDestroyImage(device, textureImage, nullptr);
    	vkFreeMemory(device, textureImageMemory, nullptr);
   	}
};

struct Model3D {
     std::map<std::string, IndexBufferObject> buffers;
};

struct Camera {
    glm::mat4 projection;
    glm::mat4 view;
    glm::quat quaternion;
    glm::vec3 pos;
};

struct Swap {
	VkImage image;
	VkImageView imageView;
	VkFramebuffer framebuffer;
	VkBuffer uniformBuffer;
	VkDeviceMemory uniformBufferMemory;
	VkDescriptorSet descriptorSet;
	VkCommandBuffer commandBuffer;
	std::vector<IndirectDraw> indirectDraws;
	UniformBufferObject uniformBufferObject;
};

class VulkanApplication {
	public:
		VkDevice device;
		VkQueue graphicsQueue;
		VkCommandPool commandPool;
		std::vector<Swap> swapChainVector;

	private: 
		VkInstance instance;
		GLFWwindow* window;
		VkDebugReportCallbackEXT callback;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkSurfaceKHR surface;
		VkQueue presentQueue;
		VkPipelineLayout pipelineLayout;
		VkDescriptorSetLayout descriptorSetLayout;
		VkRenderPass renderPass;
		VkPipeline graphicsPipeline;
		VkDescriptorPool descriptorPool;
	    VkSampler textureSampler;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		size_t currentFrame = 0;
		bool framebufferResized = false;

		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;
		bool alive = true;
		int keyboard[KEYBOARD_SIZE];

		// SWAPCHAIN
		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

	public:

		virtual void setup() = 0;
		virtual void draw(Swap * swap) = 0;
		virtual void update(float deltaTime) = 0;
     	virtual void clean() = 0;
	
		void run();
     	void close();
     	int getKeyboardStatus(int key);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	    Image loadTextureImage(const std::string& filename);
	    IndexBufferObject createIndexBufferObject(std::vector<Vertex> vertices, std::vector<uint16_t> indices);
	    void cleanIndexBufferObject(IndexBufferObject ibo);
	    void drawIndexBufferObject(Swap * swap, IndexBufferObject * ibo, glm::mat4 model);
	    void updateUniformBuffer(Swap * swap, UniformBufferObject uniformBufferObject);
		Model3D loadModel(std::string filename);
		int getWidth();
		int getHeight();
	    void bindImage(Swap * swap, int index, Image image);

	private:

		
		void initWindow();

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	    void createDescriptorSetLayout();
		void createUniformBuffer(Swap * swap);
	    void createTextureSampler();
	    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	    VkFormat findDepthFormat();
	    void createDepthResources();
	    bool hasStencilComponent(VkFormat format);
		void initVulkan();
	    void createDescriptorPool();
	    void createDescriptorSets(Swap * swap);
	    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void cleanupSwapChain();
		void recreateSwapChain();
		void createSyncObjects();
		void createCommandBuffer(Swap * swap);
		void createCommandPool();
        void createFramebuffer(Swap * swap);
		void createRenderPass();
		void createGraphicsPipeline();
		void createImageView(Swap * swap);
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		void createSwapChain();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);
		VkShaderModule createShaderModule(const std::vector<char>& code);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		void setupDebugCallback();
		void createInstance();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
	    void createIndexBuffer(IndexBufferObject * ibo, std::vector<uint16_t> indices);
	    void createVertexBuffer(IndexBufferObject * ibo, std::vector<Vertex> vertices);
	    void createInstanceBuffer(InstanceBufferObject * ibo, InstanceData * instance);
		void bindUniformBufferObject(Swap * swap);
		void drawFrame();
		void mainLoop();
		void cleanup();
};


#endif