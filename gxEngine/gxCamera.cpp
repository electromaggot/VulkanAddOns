//
// gxCamera.cpp
//
// See header file comment for overview.
//
// Created 1/27/19 by Tadd Jensen
//	© 2023 Megaphone Games under <TBD> License (i.e. project will use other FOSS-licensed code)
//
#include "gxCamera.h"
#include <stdio.h>		// for printf


void gxCamera::update(float deltaSeconds)
{
	if (previousScreenWidth != swapchainExtent.width || previousScreenHeight != swapchainExtent.height) {
		previousScreenWidth = swapchainExtent.width;
		previousScreenHeight = swapchainExtent.height;

		float aspectRatio = swapchainExtent.width / (float) swapchainExtent.height;
		float vertFOV = DEFAULT_DEGREE_FOV_IN_LANDSCAPE;

		updateCameraMatrices(aspectRatio, vertFOV);
	}
}

// Calculate model and projection matrices.  Should only REcalculate when they
//	change, not on every frame!  So only call this if a resize is detected.
//
void gxCamera::updateCameraMatrices(float aspectRatio, float degreeLandscapeFOV)
{
	float landscapeVerticalFOV = radians(degreeLandscapeFOV);

	float verticalFOV = adjustedFOVperOrientation(landscapeVerticalFOV, aspectRatio);

	MVP.proj = glm::perspective(verticalFOV, aspectRatio, zNearPlane, zFarPlane);
}

float gxCamera::adjustedFOVperOrientation(float landscapeVerticalFOV, float aspectRatio)
{
	bool isPortraitMode = (aspectRatio < 1.0f);

	if (isPortraitMode) {
		return 2.0f * atanf(tanf(landscapeVerticalFOV / 2.0f) / aspectRatio);
	}
	return landscapeVerticalFOV;
}


// First-time setup for drawing: mainly dereference variables that loop will use continually, so it
//	doesn't have to repeatedly.  Also init big objects (e.g. matrices) that don't change every frame.
//
void gxCamera::initSeldomChangedValues()
{
	MVP.view = glm::lookAt(ptPosition3D, xyzLookAt, vecUp);
}
