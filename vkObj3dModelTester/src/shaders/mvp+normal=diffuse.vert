//
// Simple Lambert diffuse shader
//	Explained at bottom.
// Inputs: MVP UBO, XYZ vertex coordinate, normal vector
// Output: intensity (at this position interpolated between the face's
//					  3 vertices and 3 normal vectors at those vertices)
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;	// From Vertex3DwNormal: XYZ coordinate
layout(location = 1) in vec3 inNormal;		//						 normal vector

layout(location = 0) out float fragDiffusity;

void main() {
	const vec3 lightVector = normalize(vec3(0, 2, 1));	// points AT light source
	const float ambient = 0.2;

	mat4 modelView = ubo.view * ubo.model;
    gl_Position = ubo.proj * modelView * vec4(inPosition, 1.0);

	vec3 modelviewNormalVector = mat3(modelView) * inNormal;
	vec3 viewLightVector = mat3(ubo.view) * lightVector;

	fragDiffusity = max(0.0, dot(modelviewNormalVector, viewLightVector)) + ambient;
}


/* EXPLAINER
Per https://en.wikipedia.org/wiki/Lambertian_reflectance
and https://en.wikipedia.org/wiki/Lambert%27s_cosine_law
 simulates lighting a fully matte material consistently across its entire surface.
 So... completely dull, no specular highlights or other reflection.

As for advantages/drawbacks to this particular implementation...
PROs: Dead simple.  Educational.
CONs: Light position/intensity/hue/color, and that of ambient component, are fixed.
	  Bland monochrome output.  Non-flexible, not otherwise very useful.
*/
