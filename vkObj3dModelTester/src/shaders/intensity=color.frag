//
// Ultra-simple-although-ultra-specific Fragment shader to assist shading
//	that's calculated in the Vertex shader and passed-in as an intesity
//	value, which is trivially converted into a monochrome/grayscale color.
//
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in float inIntensity;
layout(location = 0) out vec4 outFragColor;

void main() {
	outFragColor = vec4(inIntensity);
}
