//
// ApplicationMain.h
//
// Graphics program using Vulkan with SDL for platform-independence.
//	Create a window (or go fullscreen on mobile) and return a Vulkan-
//	compatible surface to render to.  Vulkan itself is encapsulated into
//	classes to: create the Vulkan instance, select a graphics device,
//	then set up swapchain, render pass, pipeline, frame buffers, etc.
//
// Note that there are two phases to setting-up Vulkan:
//	1. VulkanSetup's constructor/initializer list (instantiated below)-
//		Creates nearly all of Vulkan's objects, in order of dependency,
//		letting a "resource allocation is initialization" approach
//		manage object lifespans in minimal code.
//	2. Post-construction (once program has the chance to start running)-
//		Finishes initialization that requires some processing, like
//		to specify and load shader files, etc., which the rest of
//		Vulkan's pipeline depends upon.
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "AppSettings.h"
#include "PlatformSDL.h"
#include "VulkanSetup.h"
#include "gxGameClock.h"
#include "gxCamera.h"
#include "gxControlCameraLocked.h"

//#include "Cube3DTextured.h"
#include "TestModel.h"


class Application
{
private:
	PlatformSDL		platform;	// SDL is target "platform" (i/o abstraction layer).

	VulkanSetup		vulkan;		// This one instantiation...

public:
	Application()
		:	platform(),
			vulkan(platform),	//	...initializes almost all of Vulkan.
			device(vulkan.device.getLogical()),
			swapchain(vulkan.swapchain.getVkSwapchain()),
			deviceQueue(vulkan.device.Queues.getCurrent()),
			syncObjects(vulkan.syncObjects),
			swapchainExtent(vulkan.swapchain.getExtent()),
			camera(vulkan.swapchain.getExtent()),
			gameClock(),
			controlScheme(camera, gameClock)
	{
		pObject3D = nullptr;
	}

	~Application()			// Before destruction, in turn destroying child
	{								//	data structures instanced above, ensure
		vkDeviceWaitIdle(device);	//	Vulkan finish any operations in-process.

		if (pObject3D)
			delete pObject3D;
	}

		// lesser MEMBERS
private:
	int iCurrentFrame = 0;

	// Vulkan convenience references used every frame:
	VkDevice&		device;
	VkSwapchainKHR&	swapchain;
	VkQueue&		deviceQueue;
	SyncObjects&	syncObjects;
	VkExtent2D&		swapchainExtent;

	// Implementation
	DrawableObject* pObject3D;

	gxGameClock		gameClock;
	gxCamera			camera;
	gxControlCameraLocked	controlScheme;

	// Draw-loop-related detail
	const uint64_t NO_TIMEOUT = numeric_limits<uint64_t>::max();
	const uint64_t FAILSAFE_TIMEOUT = 100'000'000;				// 1/10th second in nanoseconds
	VkResult priorCall;
	VkResult call;			// local instance (the global one, while convenient, isn't thread-safe)

		// METHODS
public:
	void Run();
	void Init();
	void DialogBox(const char* message) { platform.DialogBox(message); }
private:
	void initPersistentValues();
	void updateGameElements(float deltaSeconds);

	void updateRender();
	void update();
	void draw();

	static void ForceUpdateRender(void* pOurself);
};
