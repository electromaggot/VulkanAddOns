//
// iOS-specific/gxCamera.cpp
//	gxEngine iOS-isolated extension to gxCamera
//
// Improve rendering appearance when device changes orientation.
// Upon device rotation on iOS, Apple immediately resizes the rendering surface, then over the next ~quarter second,
//	tries to "smoothly" animate the change via both scaling and rotation.  Unfortunately if incorrect, it's jarring.
//		• update() below detects the resize before the rotation animation starts.
//		• PlatformSDL::PollEvent() SDL_WINDOWEVENT_SIZE_CHANGED fires when the animation completes.
// If we don't change the Camera Matrix at all, the transition looks smooth, but what we're drawing appears to
//	stretch/shrink as the render-surface rotates, then ends rendering at the wrong aspect ratio.
// If we change the Camera Matrix first, our render immediately stretches/shrinks first, then rotates/scales into its
//	correct place.  If we change the Camera Matrix last, the rendered image rotates while stretching/shrinking
//	incorrectly, then appears to snap back to its correct aspect ratio at the end of the animation.
// The only answer is to adjust the Camera Matrix's aspect ratio gradually, "in sync" with the animation.  This means
//	guessing how long the animation is, whether its linear or eased, etc.
// The specific method devised is explored in the DEV NOTE at this file's end.
//
// In your project, only have this file's "Target Membership" be part of iOS build while excluding: ../gxCamera.cpp
//
// Why not just "turn off" the rotation animation?  It's complicated (see DEV NOTE 2).
//
// Created 3/23/23 by Tadd Jensen
//	© 2023 Megaphone Games under <TBD> License (i.e. project will use other FOSS-licensed code)
//


// Here's a quick-n-dirty way to "extend" gxCamera::update() and have this iOS-specific class substitute for the
//	general one without clumsy inheritance or device-specific #if conditional compilation.  Inclusion keeps the
//	other gxCamera::methods as one common codebase.  Call it a hack, or credit some brilliance to C++ flexibility.†
#define update(args)	update_overridden(args); void update(args)
#include "../gxCamera.h"
#undef update
#define update			update_overridden
#include "../gxCamera.cpp"
#undef update

// Unfortunately data members can't be similarly added to the original class declaration, so we have to do
//	them as "free" but still isolated for iOS only, declared "static" of course keeps them file-private...

static const float SECONDS_ANIMATE_IOS_ORIENTATION = 0.3f;
												// = 3.0f;	// if Simulator: Debug → ✓ Slow Animations  (slows by 10x)
static float secondsIntoAnimation = UNSET;

//#define DEBUG_LOW


void gxCamera::update(float deltaSeconds)
{
	if (previousScreenWidth != swapchainExtent.width || previousScreenHeight != swapchainExtent.height) {
		float aspectRatio, vertFOV;

		if (secondsIntoAnimation < SECONDS_ANIMATE_IOS_ORIENTATION) {
			if (secondsIntoAnimation == UNSET) {
				#ifdef DEBUG_LOW
					printf("iOSgxCamera startup: discarding first update()  deltaSeconds %.2f  swapchainExtent %u×%u\n", deltaSeconds, swapchainExtent.width, swapchainExtent.height);
				#endif
				// As this is first pass:	• initial deltaSeconds could be anything (no previous pass), so ignore it.
				//							• this call isn't because of device rotation, don't animate aspect ratio:
				secondsIntoAnimation = SECONDS_ANIMATE_IOS_ORIENTATION;		// just treat the animation like it's over,
				return;														//	and next-pass aspectRatio will get set.
			}
			float proportion = secondsIntoAnimation / SECONDS_ANIMATE_IOS_ORIENTATION;

			float smooth = proportion * proportion * proportion * (proportion * (proportion * 6 - 15) + 10);

			float interWidth = previousScreenWidth + ((float) swapchainExtent.width - previousScreenWidth) * smooth;
			float interHeight = previousScreenHeight + ((float) swapchainExtent.height - previousScreenHeight) * smooth;

			float bumpFOV = 1.5f - abs(smooth - 0.5f);

			vertFOV = DEFAULT_DEGREE_FOV_IN_LANDSCAPE * bumpFOV;

			aspectRatio = interWidth / interHeight;

			#ifdef DEBUG_LOW
				printf("iOSgxCamera inter aspectRatio %f  timeDelta %f  secondsIntoAnimation %f  bumpFOV %f\n", aspectRatio, deltaSeconds, secondsIntoAnimation, bumpFOV);
			#endif

			secondsIntoAnimation += deltaSeconds;
		} else {	// done
			aspectRatio = swapchainExtent.width / (float) swapchainExtent.height;

			#ifdef DEBUG_LOW
			printf("iOSgxCamera FINAL aspectRatio %f   %.f×%.f → %u×%u  secondsIntoAnimation %f\n", aspectRatio, previousScreenWidth, previousScreenHeight, swapchainExtent.width, swapchainExtent.height, secondsIntoAnimation);
			#endif

			previousScreenWidth = swapchainExtent.width;
			previousScreenHeight = swapchainExtent.height;

			vertFOV = DEFAULT_DEGREE_FOV_IN_LANDSCAPE;

			secondsIntoAnimation = 0.0f;
		}
		updateCameraMatrices(aspectRatio, vertFOV);
	}
}



/* DEV NOTE - what the animation calculations are doing, specifically...
Upon device rotation, gxCamera must change the Aspect Ratio it's rendering at.  Otherwise, or if that is done before
 versus after the rotation, visible discontinuities appear - as already described in the top header comment.
 Apple handles keeping the render-surface upright, so no need to start "rendering sideways" or rotate 90º etc.
The purpose here is to gradually (that is, over 0.3 seconds) transition the Aspect Ratio so that everything looks
 smooth... and come to find out, it's necessary to adjust the scaling by affecting the FOV Field-Of-View as well.

Parameters were determined experimentally, like the 0.3 second length of Apple's animation, during which the rendering
 surface is both incrementally rotated and resized.  Apple also appears to use https://en.wikipedia.org/wiki/Smoothstep,
 but exactly which equation is a guess.  Using the Simulator's Debug → ✓ Slow Animations feature is very helpful, which
 supposedly slows to 1/10th speed, giving 3.0 seconds to view this file's effect on Apple's render-surface changes.
 In the debugger, it is not possible to single-step through the OS-controlled part of the transition.

So 'proportion' varies from 0.0 to 1.0 as animation proceeds from begin to end, but this is a linear time progression.

From observation, Apple appears to "ease-into/ease-out-of" rotation using the "smootherstep" method of interpolation:
	float smooth = proportion * proportion * proportion * (proportion * (proportion * 6 - 15) + 10);
 as opposed to, and NOT this mere "smoothstep" approach, which...
	//float smooth = proportion * proportion * (3 - 2 * proportion);	// ...starts stretching too early.
 Note that this biasing factor is applied to each dimension separately, not just the Aspect Ratio at once, in case it
 disproportionately affects one direction more than the other and introduces mathematical error.

 (FYI the above equations are symmetrical: horizontally across X = 0.5, ramp-up rate from 0.0 mirrors ramp-down into 1.0.
  We also trial-n-errored some asymmetrical functions.  For instance, if x = proportion (0.0 <= x <= 1.0) :
	y = x		(linear)			no ramp-up ramp-down at all, doesn't match Apple's actions at all.
	y = x * x	(square, parabolic)	ramps-up too slowly at start, lags too much, with no ramp-down at end.
	y = x * x * x 	(cube)			as above, but even more exaggerated.
	y = sqrt(x)	(square flipped along X = Y) ramps-up too fast, then slowing; looks bad as indistinguishably as linear.
	y = x^3 * (3 - 2 * x)			like smoothstep but with cube component: eases more at start, much less at end.
  Firing up a Mac's Applications/Utilities/Grapher.app is very helpful to view these too.  When implementing,
  it becomes evident which curves fit Apple's algorithm and which don't.  It makes sense Apple's are symmetrical.)

HOWEVER, we weren't done yet.  Once the Aspect Ratio looked consistent across Apple's resizing of the render-surface -
 that is, our drawn image no longer stretched/squashed/skewed during the animation - it still wasn't right.  The image
 uniformly "grew larger" mid-rotation, shrinking back down by end.  Answer: also simultaneously change scale, but how?

 To make 3D objects appear smaller, either move the camera back, or expand the FOV (in both directions).  The latter
 seems most appropriate, since what's changing is the size of the rendering surface.  So to "bump up" the FOV when
 proportion is at 0.5, a simple equation can do it linearly:		again, where x = proportion (0.0 <= x <= 1.0)
	y = -|x|  or = -abs(x)	pyramid-shaped (or A-shaped) graph with peak at origin.
	y = 1.5 - |x - 0.5|		shifts peak right to X = 0.5 and upwards on Y-axis to Y's ranging from 1.0 → 1.5 → 1.0
 and for an arbitrary height of the "bump in the middle":
	y = 1.0 - abs(x - 0.5) * peakHeight + peakHeight / 2.0

 That's linear, but we need something smooth.  I originally thought "bell curve" which the following works great for:
	y = 300^-x^2			where "300" is arbitrary, but starts/ends Y near 0 at X = -1 and 1, with bump at (0, 1).
 or for something that's more of a "pure" bell curve, use e ...or exp() in actual code, for example:
	float bellWidth = 1.6f;		// 2 is narrower, 1.5 starts rising just outside 0 & 1
	float bellHeight = 2.5f;	// 2 raises height by .5, 3 raises height by 1/3rd (2 scales down maybe too much at midpoint)
	float bumpFOV = 1.0f + exp(-pow(M_E * (bellWidth * proportion - bellWidth / 2.0f), 2.0f)) / bellHeight;

HOWEVER (#2) then it occurred to me: we already have smoothing, which we can simply apply to the original linear
 equation.  That is, apply 'smooth' to it, instead of 'proportion', like this:
	float bumpFOV = 1.5f - abs(smooth - 0.5f);
 which does multiply mid-animation with the FOV value by 1.5, but in visual trials, it seems to work well.  Done!
*/

/* DEV NOTE 2 - so why not just "turn off" the rotation animation?  Yes it's possible...
	https://developer.apple.com/documentation/uikit/uicontentcontainer/1621466-viewwilltransition
	https://developer.apple.com/documentation/uikit/uiview/1622420-setanimationsenabled
...but:
 • inaccessible via SDL, which already implements viewWillTransition in SDL/src/video/uikit/SDL_uikitviewcontroller.m
   although supporting code there is minimal (unhelpful).  It does set a BOOL rotatingOrientation to YES during
   rotation, but that variable (for what it's worth) is inaccessible as well.
 • even if animation is disabled, then the render-surface simply snaps immediately to its new position, which is
   equally-if-not-more jarring, or then we must implement a transition ourselves...
 ∴ all of these options are even less desirable and more complicated than what we're doing here: simply using
   Apple's existing animation and built-in process, but just improving its appearance.
*/

/* † - DEV NOTE 3  ...implementation thoughts
As for this "extension" approach being a hack, the standard ("correct"?) way to do it would have been via a Liskov-
 substitutable base class and iOS-customized derivation with reimplemented ::update(…).  So regardless of gxCamera's
 specific implementation, it is referenced in an abstract fashion in any platform-common code.  However this still
 requires platform-dependent conditionally compiled code when the object is constructed.  So for instance, the Windows
 project still sees iOS stuff in its source code.  Frankly, as already quipped, that just seems clumsy.
The approach herein, albeit non-standard, avoids all that.  The iOS-specific class only appears in the Xcode project,
 while the Windows solution knows nothing of it, nor does a Windows developer have to see any code related to iOS.

An alternative example places shared code in a base class, for instance called gxCameraBase.  Then each platform's
 project would contain a platform-specific "gxCamera.cpp" file with different implementation to
	class gxCamera : gxCameraBase { ...
 For iOS, it's easy to see how gxCamera::update(…) can override its base class version.
 For Windows, however, the derivation is basically empty.  The need for iOS's extension is still "felt" on every
 other platform, and adds a superfluous layer of inheritance.  Again, the "hack" avoids that.
*/
