//
// DepthBufferStub.cpp
//	Vulkan Objects
//
// See header description.
//
// Created 10/12/22 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "DepthBufferStubified.h"


DepthBuffer::DepthBuffer(GraphicsDevice& graphics, iPlatform& platform, bool enabled)
{ graphics.getProfile().selectedDepthFormat = VK_FORMAT_UNDEFINED; }

void DepthBuffer::Recreate(Swapchain& swapchain) { }
