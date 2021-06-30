#version 450
#extension GL_ARB_separate_shader_objects : enable

// Uniform attributes
layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 projection;
} ubo;

// Vertex attributes
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uint inTexIndex;

// Instance attributes
layout(push_constant) uniform PushConsts {
	mat4 model;
} pushConsts;


layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out float fragTexIndex[16];

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = ubo.projection * ubo.view * pushConsts.model * vec4(inPosition, 1.0);
    fragNormal = inNormal;
    fragTexCoord = inTexCoord;
    for(int i =0 ; i < 16 ; ++i) {
		fragTexIndex[i] = i == inTexIndex ? 1.0: 0.0;
    } 
}

