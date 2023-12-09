//
// Texture-mapped diffuse-shaded Vertex shader
//	Inputs: *two* UBO (MVP, local), XYZ vertex and UV texture coordinates, normal 3D vector
//	Outputs to Fragment shader:
//		Intensity value from simplistically calculated Lambert shading.
//			(see "mvp+normal=diffuse.vert" for shading explanation)
//		Texture Coordinate passed-through as-is.
//		Since a Texture is assumed, make sure the Fragment Shader expects
//			it starting at (binding = 2), since 0 and 1 are bound below.
//
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 1) uniform UniformBufferObject2 {
	mat4 local;
} ubo2;
											// From Vertex3DTextured, for each vertex:
layout(location = 0) in vec3 inPosition;		// 3D XYZ vertex coordinate
layout(location = 1) in vec3 inNormal;			// normal vector
layout(location = 2) in vec2 inTexCoord;		// UV texture coordinate

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out float fragIntensity;	// Diffuse-shaded

void main() {
	const vec3 lightVector = vec3(0, 2, 1);		// Light source "proportional position" relative to origin.
	const float ambient = 0.2;

	const vec3 unitvec = normalize(lightVector);	// points AT light source
	const vec3 lightUnscaledVector = vec3(unitvec[0] / ubo2.local[0][0],	// Undo effect of scaling (if any) on lighting.
										  unitvec[1] / ubo2.local[1][1],	//	(Code assigning ubo2 should Assert identity matrix or
										  unitvec[2] / ubo2.local[2][2]);	//	 these are all non-zero, otherwise divide could crash!)

	mat4 modelView = ubo.view * ubo.model * ubo2.local;
	gl_Position = ubo.proj * modelView * vec4(inPosition, 1.0);

	vec3 modelviewNormalVector = mat3(modelView) * inNormal;
	vec3 viewLightVector = mat3(ubo.view) * lightUnscaledVector;

	fragIntensity = max(0.0, dot(modelviewNormalVector, viewLightVector)) + ambient;
	fragTexCoord = inTexCoord;
}
