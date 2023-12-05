//
// gxCamera.h
//	gxEngine (Vulkan graphics/game engine)
//
// Represents a "simple as possible" viewpoint, having a position and orientation (via its own
//	position/rotation-matrix, so is its own 3D object).  The "look at" direction that the camera
//	is pointing can either stay fixed, if not updated, or update if the camera moves to a new
//	location to stay pointing at a particular point in space, for example the origin.
// Includes/encapsulates:
//	- field-of-view (vertical FOV assuming a landscape orientation, specified in
//		degrees then baked-into the view matrix).
//	- an "up vector" for the camera (also bakes-into matrix).
//	- Model-View-Projection (MVP) matrices as a Uniform Buffer Object (UBO) used
//		when Vulkan renders an object model from this camera position.
//	- use of GLM for graphics primitives and basic operations like perspective, lookAt, translate, etc.
//		Obviously GLM originates from OpenGL but is perfectly handy for Vulkan.
//  - finally, may sneak-in some functionality from GLU, like custom versions of gluPerspective
//		and gluLookAt.  For detail on how to use those, see Dev Note at implementation file end.
//		(TJ_TODO: Nixed this as perhaps too redundant to what GLM offers, still TBD whether to reincorporate later.)
// Internally calculates Aspect Ratio automatically based on rendering-window resolution and orientation.
// For more advanced camera features and "look at" modes, look to: gxCameraAdvanced
//
// Created 1/27/19 by Tadd Jensen
//	© 2023 Megaphone Games under <TBD> License (i.e. project will use other FOSS-licensed code)
//
#ifndef gxCamera_h
#define gxCamera_h

#include "vulkan/vulkan_core.h"		// for VkExtent2D
#include "VulkanMath.h"				// for GLM
#include "gxMatrix.h"
#include "UniformBufferLiterals.h"
#include "Customizer.h"


const float UNSET = FLT_MIN;

const float DEFAULT_DEGREE_FOV_IN_LANDSCAPE = 45.0f;

const float DEFAULT_Z_NEAR_PLANE = 0.1f;
const float DEFAULT_Z_FAR_PLANE	 = 1000.0f;

const vec3	DEFAULT_UP_VECTOR = vec3(0.0f,  1.0f, 0.0f);
const vec3	VULKAN_UP_VECTOR  = vec3(0.0f, -1.0f, 0.0f);		// although for Vulkan, negative Y points "up" (note that
																//	default is to treat models like they came from OpenGL)
const vec3	DEFAULT_LOOK_AT_ORIGIN = vec3(0.0f, 0.0f, 0.0f);
																// Instead of initializing camera at the origin,
const vec3	FAILSAFE_CAMERA_POSITION = vec3(0.0f, 2.0f, 4.0f);	//	start with it 4 units back and 2 up "just in case"
																//	it's left unset & the rendered object is at origin.
																//	(e.g. that's 4 meters back, 2 up, in 1-meter units)

class gxCamera : public gxMatrix
{
public:
	gxCamera(VkExtent2D& extent, Customizer coordinateSystemChange = NONE)
		:	swapchainExtent(extent),
			uboMVP(MVP),
			zNearPlane(DEFAULT_Z_NEAR_PLANE),
			zFarPlane(DEFAULT_Z_FAR_PLANE),
			vecUp(DEFAULT_UP_VECTOR),
			xyzLookAt(DEFAULT_LOOK_AT_ORIGIN),
			previousScreenWidth(UNSET),
			previousScreenHeight(UNSET)
	{
		position3D = FAILSAFE_CAMERA_POSITION;

		defaultToLeftHandRendering = (coordinateSystemChange & MODELED_FOR_VULKAN);	// i.e. for all models rendered hence-
	}																				//	forth (versus on a per-model basis)

	~gxCamera()
	{ }

		// MEMBERS
	UBO_MVP	MVP;
	UBO uboMVP;
private:
	VkExtent2D&	swapchainExtent;	// so camera can tell how big the screen is

	vec3  vecUp;
	vec3  xyzLookAt;

	float zNearPlane;
	float zFarPlane;

	float previousScreenWidth;
	float previousScreenHeight;

	bool  defaultToLeftHandRendering = false;

	void  initialize();
		// METHODS
public:
	void  update(float deltaSeconds);
	void  updateProjectionMatrix(float aspectRatio, float degreeLandscapeFOV = DEFAULT_DEGREE_FOV_IN_LANDSCAPE);
	float adjustedFOVperOrientation(float landscapeVerticalFOV, float aspectRatio);
	void  updateViewMatrix();
	void  initSeldomChangedValues();

		// getters
	float getCurrentScreenWidth()	{ return previousScreenWidth; }
	float getCurrentScreenHeight()	{ return previousScreenHeight; }
};


#endif	// gxCamera_h
