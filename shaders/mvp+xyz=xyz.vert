//
// Couldn't-be-simpler Vertex shader, although special-purpose
//	3D-transformed per passed-in model-view-projection and
//	forwards incoming Vertex Coordinate through to Pixel shader.
//
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;	// From Vertex3D: XYZ vertex coordinate
layout(location = 0) out vec3 fragPosition;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	fragPosition = inPosition;
}
