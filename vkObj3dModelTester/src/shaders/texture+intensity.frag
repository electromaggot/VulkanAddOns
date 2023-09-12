#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in float fragIntensity;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = vec4(fragIntensity * texture(texSampler, fragTexCoord).rgb, 1.0);

	//outColor = texture(texSampler, fragTexCoord);		// <-- unadorned/uncolored
	//outColor = texture(texSampler, fragTexCoord * 2.0);	// <-- experience VK_SAMPLER_ADDRESS_MODE_REPEAT
	//outColor = vec4(fragTexCoord, 0.0, 1.0);	// <-- or, simply view texture coordinates as colors!
}
