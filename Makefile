CC=g++
SRC=*.cpp vulkan/*.cpp math/*.cpp

VULKAN_SDK_PATH = /usr/include/vulkan
STB_INCLUDE_PATH = dependencies/stb
RAPID_JSON_PATH = dependencies/rapidjson
TINYOBJ_INCLUDE_PATH = dependencies/tinyobjloader

VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/explicit_layer.d
LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib

CFLAGS = -std=c++11 -g -I$(VULKAN_SDK_PATH) -I$(STB_INCLUDE_PATH) -I$(RAPID_JSON_PATH)/include -I$(TINYOBJ_INCLUDE_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

compile:
	mkdir -p bin
	mkdir -p bin/shaders
	mkdir -p bin/models
	cp -rf textures bin
	cp -rf models bin

	glslangValidator -V shaders/shader.vert -o bin/shaders/vert.spv
	glslangValidator -V shaders/shader.frag -o bin/shaders/frag.spv
	$(CC) $(CFLAGS) -o bin/vkHelium $(SRC) $(LDFLAGS)

run:
	cd bin;	./vkHelium

debug:
	@echo LD_LIBRARY_PATH=$(LD_LIBRARY_PATH)
	@echo VK_LAYER_PATH=$(VK_LAYER_PATH)
	cd bin;	valgrind ./vkHelium


tool:
	$(CC) $(CFLAGS) -o bin/converter tools/wavefrontConverter.cpp
