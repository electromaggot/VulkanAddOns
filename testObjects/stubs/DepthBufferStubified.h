//
// DepthBufferStub.h
//	Vulkan Objects
//
// Drop-in dummy for depth buffer if it's not needed.
// See DepthBuffer.h for what non-stubbed version does.
//
// Created 10/12/22 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef DepthBuffer_h
#define DepthBuffer_h

#include "ImageResource.h"
#include "Swapchain.h"


class DepthBuffer //: protected ImageResource
{
public:
	DepthBuffer(GraphicsDevice& graphics, iPlatform& platform, bool enabled = false);

		// MEMBERS
private:
	ImageInfo	imageInfo;

		// METHODS
public:
	void Recreate(Swapchain& swapchain);

		// getter
	VkImageView* getpImageView() {
		return nullptr;
	}
};

#endif	// DepthBuffer_h
