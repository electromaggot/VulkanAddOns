//
// gxControlCameraLocked.cpp
//	gxEngine (Vulkan game/graphics engine)
//
// See header file for main comment.
//
// Created 4/4/23 by Tadd Jensen
//	© 2023 Megaphone Games under <TBD> License (i.e. project will use other FOSS-licensed code)
//
#include "gxControlCameraLocked.h"
#include <stdio.h>	// TEMPORARY! for printf

//#define DEBUG_LOW


void gxControlCameraLocked::handlePrimaryPressAndDrag(int toX, int toY)
{
	if (timePrimaryPressed > 0.0f && !isZooming)
	{
		vec3 position = camera.position3D;
		float moveX = pressX - toX;  float moveY = pressY - toY;
		camera.matrix = glm::rotate(mat4(1.0f), radians(priorMoveX - moveX), vec3(0.0f, 1.0f, 0.0f));	 // moving X-wise rotates around Y-axix
		camera.matrix = glm::rotate(camera.matrix, radians(priorMoveY - moveY), vec3(1.0f, 0.0f, 0.0f)); //		Y-wise around X-axis
			// Note that the above rotations *could* be applied directly in radians, but we'd have to multiply a "sensitivity"
			//	factor.  It just so happens that by chance the mouse movements translate well "straight across" to degrees.
		priorMoveX = moveX;  priorMoveY = moveY;
			// Applying a Rotation to the matrix of the Camera simply spins it in place.  Afterwards this Camera will turn to
			//	LookAt() a point, then that Rotation supersedes.  So must also apply Translation, effective after the Rotation.
		camera.matrix = glm::translate(camera.matrix, position);

		camera.updateViewMatrix();

		#ifdef DEBUG_LOW
			printf("DRAG xy%4.0f,%3.0f\t\txyzCamera%7.2f,%6.2f,%6.2f\n", moveX, moveY, camera.x, camera.y, camera.z);
		#endif
	}
}

void gxControlCameraLocked::handlePrimaryPressDown(int atX, int atY)
{
	timePrimaryPressed = clock.secondsSinceGameStart();		//TJ: Note that each SDL event does have a .timestamp field!

	pressX = atX;  pressY = atY;
}

void gxControlCameraLocked::handlePrimaryPressUp(int atX, int atY)
{
	float timeReleased = clock.secondsSinceGameStart();

	if (timeReleased - timePrimaryPressed < MINIMUM_SECONDS_LONG_PRESS)
	{
		#ifdef DEBUG_LOW
			printf("PRESS xy %3d,%3d\n", atX, atY);
		#endif
		//TJ_TODO: soon, add support to recognize Directional SWIPE !!!
		//TJ_TODO:	oh also... add support for DOUBLE-CLICK/DOUBLE-TAP!
	} else {
		if (abs(atX - pressX) <= MAXIMUM_XY_MOVE_LONG_PRESS && abs(atY - pressY) <= MAXIMUM_XY_MOVE_LONG_PRESS)
		{
			// Note: it is possible to "undo" any drag, even if small, since now this is really considered a long-press.
			//	OR can choose to ignore it
			#ifdef DEBUG_LOW
				printf("LONG-PRESS xy %3d,%3d  began xy %3d,%3d\n", atX, atY, pressX, pressY);
			#endif
		}
		priorMoveX = priorMoveY = 0.0f;
	}
	timePrimaryPressed = -1.0f;
	isZooming = false;
}

void gxControlCameraLocked::handleMouseWheel(int spunX, int spunY)
{
	const float SCALING_FRACTION = 0.1f;

	float scale = 1.0f - SCALING_FRACTION * (float) spunY;

	#ifdef DEBUG_LOW
		printf("MOUSE-WHEEL xy%2d,%3d\tscale%7.4f", spunX, spunY, scale);
	#endif

	mimicZooming(scale);
}

void gxControlCameraLocked::handlePinchSpread(float distance)
{
	const float SCALING_MULTIPLIER = 10.0f;

	isZooming = true;

	float scale = 1.0f - SCALING_MULTIPLIER * distance;

	#ifdef DEBUG_LOW
		printf("PINCH/SPREAD%8.4f\tscale%7.4f", distance, scale);
	#endif

	mimicZooming(scale);
}

// "Zoom" simply moves camera closer-to/farther-from LookAt point, uses no actual scaling.
//	factor > 1.0 zooms-in ("scales" up), factor < 1.0 (> 0.0) zooms-out ("scales" down)
//
//TJ_TODO: Make sure you TEST THIS thoroughly for LookAt point that's not the origin! (then delete this comment)
//
void gxControlCameraLocked::mimicZooming(float factor)
{
	camera.position3D *= factor;

	camera.updateViewMatrix();

	#ifdef DEBUG_LOW
		printf("\tscale %.3f\txyzCamera%7.2f,%6.2f,%6.2f\n", factor, camera.x, camera.y, camera.z);
	#endif
}

