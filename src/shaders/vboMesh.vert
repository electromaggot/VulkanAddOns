#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 1) uniform UniformBufferObject2 {
	mat4 model;
} ubo2;

											// From Vertex2DColored, for each vertex:
layout(location = 0) in vec2 inPosition;		// 2D XY vertex coordinate
//layout(location = 1) in vec3 inColor;			// RGB color

//layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * ubo2.model * vec4(inPosition, 0.0, 1.0);
//    fragColor = inColor;
}
