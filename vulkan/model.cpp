#include "vulkan.hpp"

Model3D VulkanApplication::loadModel(std::string filename) {
	rapidjson::Document document; 
	Model3D model;
	std::vector<char> file = readFile(filename);
	std::string str(file.begin(),file.end());
//std::cout << str << std::endl;

	document.Parse(str.c_str());
	const rapidjson::Value& jGroups = document["g"];
				
	for (rapidjson::Value::ConstMemberIterator itr = jGroups.MemberBegin(); itr != jGroups.MemberEnd(); ++itr) {
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;

//				std::cout << "Group:" << itr->name.GetString() << std::endl;
		const rapidjson::Value& jGroup = itr->value;


		const rapidjson::Value& jIndices = jGroup["i"];
		if(jIndices.IsArray()) {
			for (int i = 0; i < jIndices.Size(); ) {
				indices.push_back(jIndices[i++].GetInt64());
			}
		}				

		const rapidjson::Value& jVertices = jGroup["v"];
		if(jVertices.IsArray()) {
			for (int i = 0; i < jVertices.Size(); ) {
				Vertex vertex;
				vertex.pos[0] = jVertices[i++].GetFloat();
				vertex.pos[1] = jVertices[i++].GetFloat();
				vertex.pos[2] = jVertices[i++].GetFloat();
				vertices.push_back(vertex);
			}
		}

		const rapidjson::Value& jTexCoords = jGroup["t"];
		if(jTexCoords.IsArray()) {
			int j =0;
			for (int i = 0; i < jTexCoords.Size(); ++j) {
				vertices[j].texCoord[0] = jTexCoords[i++].GetFloat();
				vertices[j].texCoord[1] = jTexCoords[i++].GetFloat();
			}
		}

		const rapidjson::Value& jNormals = jGroup["n"];
		if(jNormals.IsArray()) {
			int j =0;
			for (int i = 0; i < jNormals.Size(); ++j) {
				vertices[j].normal[0] = jNormals[i++].GetFloat();
				vertices[j].normal[1] = jNormals[i++].GetFloat();
				vertices[j].normal[2] = jNormals[i++].GetFloat();
			}
		}

		model.buffers[itr->name.GetString()] = createIndexBufferObject(vertices, indices);
	//	std::cout << vertices.size() << std::endl;
	}

	return model;
}

void VulkanApplication::createIndexBuffer(IndexBufferObject * ibo, std::vector<uint16_t> indices) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
	VkDeviceSize bufferSize = sizeof(uint16_t) * indices.size();
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ibo->indexBuffer, ibo->indexBufferMemory);

    copyBuffer(stagingBuffer, ibo->indexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VulkanApplication::createVertexBuffer(IndexBufferObject * ibo, std::vector<Vertex> vertices) {
    VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ibo->vertexBuffer, ibo->vertexBufferMemory);
    copyBuffer(stagingBuffer, ibo->vertexBuffer, bufferSize);
    
    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

IndexBufferObject VulkanApplication::createIndexBufferObject(std::vector<Vertex> vertices, std::vector<uint16_t> indices) {
	IndexBufferObject ibo;
	ibo.indices = indices;
	ibo.vertices = vertices;
	createIndexBuffer(&ibo, indices);
	createVertexBuffer(&ibo, vertices);
	ibo.indicesCount = indices.size();
	return ibo;
}

void VulkanApplication::cleanIndexBufferObject(IndexBufferObject ibo) {
    vkDestroyBuffer(device, ibo.indexBuffer, nullptr);
    vkFreeMemory(device, ibo.indexBufferMemory, nullptr);



    vkDestroyBuffer(device, ibo.vertexBuffer, nullptr);
    vkFreeMemory(device, ibo.vertexBufferMemory, nullptr);
}