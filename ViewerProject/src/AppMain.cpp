//
// ApplicationMain.cpp
//
// See header file comment for overview.
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "AppMain.h"

#include "CommandObjects.h"
#include "MeshObject.h"
#include "UniformBufferLiterals.h"


// This "Post-construction Initialization" runs after VulkanSetup's initializer list instantiates/initializes
//  nearly all of the Vulkan objects.  Now instruct it how to finish, specifying app-specific customizations.
//
void Application::Init()
{
	Renderables& renderables = vulkan.command.renderables;

	pCube3D = new RenderableCube(camera.uboMVP);
	renderables.Add(FixedRenderable(*pCube3D, vulkan, platform));

	vulkan.command.PostInitPrepBuffers(vulkan);

	initPersistentValues();

	platform.RegisterForceRenderCallback(Application::ForceUpdateRender, this);
}

void Application::initPersistentValues()
{
	camera.initSeldomChangedValues();	// and simply use camera's default "failsafe" position
}


void Application::Run()
{
	platform.ClearEvents();

	for (bool quit = false; !quit; )
	{
		while (platform.PollEvent(&controlScheme))
			quit = platform.IsEventQUIT();

		if (platform.IsWindowMinimizedOrHidden())
			platform.AwaitEvent();

		updateRender();
	}
}

void Application::updateRender()
{
	gameClock.BeginNewFrame();
	update();
	draw();
}

// Let an external caller force a redraw.  One example: "live window resizing" or pseudorealtime rendering while dragging:
//	an operation that otherwise fully blocks, but may have a Watcher applied that gets called per each change in dimension.
//
void Application::ForceUpdateRender(void* pOurself)
{
	Application* pSelf = static_cast<Application*>(pOurself);
	if (pSelf) {
		if (pSelf->platform.isWindowResized)
			pSelf->vulkan.RecreateRenderingResources();

		pSelf->updateRender();
	}
}


// "Step forward" all active game elements (e.g. those that move) according to time that has passed since previous step.
//
void Application::update()
{
	float deltaTime = gameClock.deltaSeconds();

	vulkan.command.renderables.Update(deltaTime);	// advances each game element, calling its gxAction.Update(dt)

	camera.update(deltaTime);
}


// Render all in-game elements.
//
void Application::draw()
{
	const uint64_t NO_TIMEOUT = numeric_limits<uint64_t>::max();
	const uint64_t FAILSAFE_TIMEOUT = 100'000'000;				// 1/10th second in nanoseconds
	uint32_t iNextImage;

	// Await prior submission's finish...						(and to never risk deadlock ↓ )
	vkWaitForFences(device, 1, &syncObjects.inFlightFences[iCurrentFrame], VK_TRUE, FAILSAFE_TIMEOUT);

	call = vkAcquireNextImageKHR(device, swapchain, NO_TIMEOUT,
								 syncObjects.imageAvailableSemaphores[iCurrentFrame],
								 VK_NULL_HANDLE, &iNextImage);
	const char* called = "Acquire Next Image";

	if (platform.IsWindowResized() || call == VK_ERROR_OUT_OF_DATE_KHR || call == VK_SUBOPTIMAL_KHR)
		vulkan.RecreateRenderingResources();

	//	...then restore Fence back to unsignaled state.
	vkResetFences(device, 1, &syncObjects.inFlightFences[iCurrentFrame]);

	if (call == VK_SUCCESS)
	{
		vulkan.command.RecordRenderablesForNextFrame(vulkan, iNextImage);

		vulkan.command.renderables.UpdateUniformBuffers(iNextImage);

		// SUBMIT --------------------------------------------------------------------------------------

		VkPipelineStageFlags waitStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		vector<VkCommandBuffer> allRenderablesCommandBuffers = vulkan.command.BuffersForFrame(iNextImage);

		VkSubmitInfo submitInfo = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &syncObjects.imageAvailableSemaphores[iCurrentFrame],
			.pWaitDstStageMask = &waitStageFlags,
			.commandBufferCount = (uint32_t) allRenderablesCommandBuffers.size(),
			.pCommandBuffers = allRenderablesCommandBuffers.data(),
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &syncObjects.renderFinishedSemaphores[iCurrentFrame]
		};
		VkSubmitInfo submits[] = { submitInfo };
		int numSubmits = N_ELEMENTS_IN_ARRAY(submits);

		call = vkQueueSubmit(deviceQueue, numSubmits, submits, syncObjects.inFlightFences[iCurrentFrame]);
		called = "Queue Submit draw command buffer FAILURE";

		if (call == VK_SUCCESS)
		{
			// PRESENT -------------------------------------------------------------------------------------

			VkPresentInfoKHR presentInfo = {
				.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.pNext = nullptr,
				.waitSemaphoreCount = 1,
				.pWaitSemaphores = &syncObjects.renderFinishedSemaphores[iCurrentFrame],
				.swapchainCount = 1,
				.pSwapchains = &swapchain,
				.pImageIndices = &iNextImage,
				.pResults = nullptr
			};

			call = vkQueuePresentKHR(deviceQueue, &presentInfo);
			called = "Queue Present";
		}
		if (platform.IsWindowResized() || call == VK_ERROR_OUT_OF_DATE_KHR || call == VK_SUBOPTIMAL_KHR)
			vulkan.RecreateRenderingResources();
	}
	if (call != VK_SUCCESS)
		Log(ERROR, called + ErrStr(call));

	iCurrentFrame = (iCurrentFrame + 1) % syncObjects.MaxFramesInFlight;
}