//
// gxCamera.cpp
//
// See header file comment for overview.
//
//	  which			Typically only needs Updating
//	  Matrix		 upon change to
//  ----------		----------------
// 	Projection		device orientation or window size.
// 	   View			camera's position/orientation.
//
// Created 1/27/19 by Tadd Jensen
//	© 2023 Megaphone Games under <TBD> License (i.e. project will use other FOSS-licensed code)
//
#include "gxCamera.h"
#include <stdio.h>		// for printf


void gxCamera::initialize()
{

}


// Runs every frame: check to see if window size or device orientation changed,
//													thus altering Projection.
void gxCamera::update(float deltaSeconds)
{
	if (previousScreenWidth != swapchainExtent.width || previousScreenHeight != swapchainExtent.height) {
		previousScreenWidth = swapchainExtent.width;
		previousScreenHeight = swapchainExtent.height;

		float aspectRatio = swapchainExtent.width / (float) swapchainExtent.height;
		float vertFOV = DEFAULT_DEGREE_FOV_IN_LANDSCAPE;

		updateProjectionMatrix(aspectRatio, vertFOV);
	}
}

// Calculate Projection matrix, perhaps considering device rotation (although may only apply to mobile platforms).
//  Should REcalculate when screen dimensions change, not on every frame!  So only call this if a resize is detected.
//
void gxCamera::updateProjectionMatrix(float aspectRatio, float degreeLandscapeFOV)
{
	float landscapeVerticalFOV = radians(degreeLandscapeFOV);

	float verticalFOV = adjustedFOVperOrientation(landscapeVerticalFOV, aspectRatio);

	MVP.proj = glm::perspective(verticalFOV, aspectRatio, zNearPlane, zFarPlane);

	MVP.proj[1][1] *= -1.0f;
}

float gxCamera::adjustedFOVperOrientation(float landscapeVerticalFOV, float aspectRatio)
{
	bool isPortraitMode = (aspectRatio < 1.0f);

	if (isPortraitMode) {
		return 2.0f * atanf(tanf(landscapeVerticalFOV / 2.0f) / aspectRatio);
	}
	return landscapeVerticalFOV;
}


void gxCamera::updateViewMatrix()
{
	MVP.view = glm::lookAt(position3D, xyzLookAt, vecUp);
}


// First-time setup for drawing: mainly dereference variables that loop will use continually, so it
//	doesn't have to repeatedly.  Also init big objects (e.g. matrices) that don't change every frame.
//
void gxCamera::initSeldomChangedValues()
{
	updateViewMatrix();
}
