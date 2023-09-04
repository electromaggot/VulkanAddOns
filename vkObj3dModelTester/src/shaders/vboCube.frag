#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPosition;
layout(location = 0) out vec4 outColor;

void main() {
	//outColor = vec4(1.0, 1.0, 1.0, 1.0);
	outColor = vec4(fragPosition, 1.0);
	outColor.x += 0.5f;
	outColor.y += 0.5f;
	outColor.z += 0.5f;
}
