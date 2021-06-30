#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler[16];

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in float fragTexIndex[16];

layout(location = 0) out vec4 outColor;

void main() {
	vec3 lightDir = vec3(0.0, 0.0, -1.0);
	outColor = vec4(0.0);
	
	for(int i=0 ; i < 16; ++i) {
    	if(fragTexIndex[i]>0){
    		outColor += fragTexIndex[i]*texture(texSampler[i], fragTexCoord);
    	}
	}

    outColor.xyz *= clamp(dot(-lightDir, fragNormal), 0.5,1.0);
    if (outColor.w == 0.0)
      discard; 
}

