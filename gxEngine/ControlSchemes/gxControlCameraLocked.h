//
// gxControlCameraLocked.h
//	gxEngine (Vulkan game/graphics engine)
//
// Camera "locked on" to a specifically defined point.
//
// Created 4/4/23 by Tadd Jensen
//	© 2023 Megaphone Games under <TBD> License (i.e. project will use other FOSS-licensed code)
//
#include "iControlScheme.h"
#include "gxCamera.h"
#include "gxGameClock.h"


class gxControlCameraLocked : public iControlScheme
{
public:
	const float MINIMUM_SECONDS_LONG_PRESS = 0.5f;
	const float MAXIMUM_XY_MOVE_LONG_PRESS = 3.0f;	// long-press allows slight wiggle, especially touchscreen

	gxControlCameraLocked(gxCamera& camera, gxGameClock& clock)
		:	camera(camera),
			clock(clock),
			timePrimaryPressed(-1.0f),
			isZooming(false),
			priorMoveX(0.0f),
			priorMoveY(0.0f)
	{ }

	void handlePrimaryPressAndDrag(int toX, int toY);
	void handlePrimaryPressDown(int atX, int atY);
	void handlePrimaryPressUp(int atX, int atY);
	void handleMouseWheel(int spunX, int spunY);
	void handlePinchSpread(float distance);
	//void handleTwoFingerTwist(float angle);

	bool WasPrimaryShortPressed()		{ bool wasPrimaryShortPress = isPrimaryShortPress;
		 /* This is a ONE SHOT! */		  isPrimaryShortPress = false;
		 /*	Only resets once read. */	  return wasPrimaryShortPress; }

protected:
	void mimicZooming(float factor);

private:
	gxCamera&	 camera;	// operated on by this ControlScheme
	gxGameClock& clock;		// aids timing of UI events, like tap vs. long-press

	float	timePrimaryPressed;
	bool	isZooming;
	float	priorMoveX, priorMoveY;
	int		pressX, pressY;

	bool	isPrimaryShortPress = false;
};
