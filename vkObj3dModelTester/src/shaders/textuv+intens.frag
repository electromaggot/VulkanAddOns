//
// Simple single-texture, shaded Fragment shader
//	Shading is calculated Vertex-shader-side and passed-in as an intensity value.
//	Ins: texture sampler, texture UV 2D coordinate, pixel brightness
//	Out: color of this pixel
//
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in float fragIntensity;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = vec4(fragIntensity * texture(texSampler, fragTexCoord).rgb, 1.0);
}
