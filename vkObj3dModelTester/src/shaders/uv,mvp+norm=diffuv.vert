//
// Texture-mapped diffuse-shaded Vertex shader
//	Inputs: MVP UBO, XYZ vertex and UV texture coordinates, normal 3D vector
//	Outputs to Fragment shader:
//		Intensity value from simplistically calculated Lambert shading.
//			(see "mvp+normal=diffuse.vert" for shading explanation)
//		Texture coordinate passed-through as-is.
//
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;
											// From Vertex3DTextured, for each vertex:
layout(location = 0) in vec3 inPosition;		// 3D XYZ vertex coordinate
layout(location = 1) in vec2 inTexCoord;		// UV texture coordinate
layout(location = 2) in vec3 inNormal;			// normal vector

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out float fragIntensity;	// Diffuse-shaded

void main() {
	const vec3 lightVector = normalize(vec3(0, 2, 1));	// points AT light source
	const float ambient = 0.2;

	mat4 modelView = ubo.view * ubo.model;
	gl_Position = ubo.proj * modelView * vec4(inPosition, 1.0);

	vec3 modelviewNormalVector = mat3(modelView) * inNormal;
	vec3 viewLightVector = mat3(ubo.view) * lightVector;

	fragIntensity = max(0.0, dot(modelviewNormalVector, viewLightVector)) + ambient;
	fragTexCoord = inTexCoord;
}
