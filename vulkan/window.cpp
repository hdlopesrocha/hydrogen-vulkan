#include "vulkan.hpp"

void VulkanApplication::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	for(int i=0; i < KEYBOARD_SIZE; ++i) {
		keyboard[i] = 0;
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void VulkanApplication::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   	auto app = reinterpret_cast<VulkanApplication*>(glfwGetWindowUserPointer(window));
 	if(key > 0 && key < KEYBOARD_SIZE) {
 		app->keyboard[key] = action;
 	}
}

void VulkanApplication::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<VulkanApplication*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}


void VulkanApplication::createSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

int VulkanApplication::getKeyboardStatus(int key) {
	return keyboard[key];
}
