//
// Cube3DColorTestVulkanModel.h
//	Vulkan Convenience 3D Objects
//
// A very-specific-purpose 3D cube for testing, demonstration, learning.
// Further "what"/"why" at bottom of Cube3DColorTestVulkanModel.h file.
//
// Modeled using LEFT-HANDED coordinate system of VULKAN!  ...as opposed
//	to the default for this engine, which is right-handed/OpenGL (due to
//	familiarity and ubiquity of existing models for that legacy system).
// But beware, this can be confusing and seem contradictory!  See additional
// 	explanation/caveats on coordinate layout at bottom of this file.
//
// Created 7/15/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "Vertex3DTypes.h"
#include "FixedRenderable.h"
#include "MeshObject.h"


const vec3 CubeVertices[] = {		// add 0.5f to
									//	x= y= z=
	{ -0.5f, -0.5f, -0.5f },	// 0	r0 g0 b0	black
	{  0.5f, -0.5f, -0.5f },	// 1	r1 g0 b0	red
	{  0.5f,  0.5f, -0.5f },	// 2	r1 g1 b0	yellow
	{ -0.5f,  0.5f, -0.5f },	// 3	r0 g1 b0	green

	{  0.5f, -0.5f,  0.5f },	// 4	r1 g0 b1	magenta
	{ -0.5f, -0.5f,  0.5f },	// 5	r0 g0 b1	blue
	{ -0.5f,  0.5f,  0.5f },	// 6	r0 g1 b1	cyan
	{  0.5f,  0.5f,  0.5f }		// 7	r1 g1 b1	white
};

const IndexBufferDefaultIndexType CubeIndices[] = {

	4, 5, 6, 6, 7, 4,		// rear		// rendering back-to-front
	3, 2, 7, 7, 6, 3,		// bottom	//	(& bottom-to-top)
	1, 4, 7, 7, 2, 1,		// right
	5, 0, 3, 3, 6, 5,		// left		// however, sides won't look
	5, 4, 1, 1, 0, 5,		// top		//	correct unless culled!
	0, 1, 2, 2, 3, 0,		// front
};



class RenderableCube : public DrawableSpec
{
	VertexDescription<Vertex3D> vertexDescriptor;
	MeshObject cube3DObject = {
		vertexDescriptor,
		(void*) CubeVertices,
		N_ELEMENTS_IN_ARRAY(CubeVertices),
		(void*) CubeIndices,
		N_ELEMENTS_IN_ARRAY(CubeIndices)
	};

public:
	RenderableCube(UBO& refMVP)								// cube, vertex + index buffer
		:	DrawableSpec(cube3DObject)						// ...this vertex buffer and  <──╮
	{														//								 │
		shaders = { { VERTEX,	"mvp+xyz=xyz-vert.spv" },	// This shader expects... ───────┤
					{ FRAGMENT, "xyz=color-frag.spv" } };	//								 │
		pUBOs = { refMVP };									//	...this uniform buffer.  <───╯

		customize = MODELED_FOR_VULKAN | FRONT_CLOCKWISE;
	}
};



#if ASCII_ART_EXPLAINER

// Cube modeled for VULKAN					Thus take note!

			  5----------4					   ^ +Z
	 black	 /|         /|					  /
	(-,-,-) 0----------1 |					 /
			| |        | |					+-----> +X
			| 6--------|-7 (+,+,+)			|
			|/         |/	white			|
			3----------2					|
											V +Y
// FRONT FACE			WINDING
				-y
				  ^
		 0        |        1
		  o >-----------. o
(-.5,-.5) ,-.     |      \  (.5,-.5)
		  |  `.|  |   CW  `
		  |   -'  |       |
 -x <-----|-------+-------|-----> +x
		  |       |  __   |
		  .  CW   |  |.   |
 (-.5,.5)  \      |    `-'   (.5,.5)
		  o `-----------< o
		 3        |        2
				  v
				   +y
#endif

// Again, calling the above layout "left-handed" can be confusing and may
//	seem contradictory! ...since in the end, they say it all depends on the
//	View matrix used, which typically relies on GLM which favors OpenGL.
//
// Therefore we took from Vulkan sample code provided by LunarG, particularly
// https://github.com/LunarG/VulkanSamples/blob/master/API-Samples/data/cube_data.h
//	for preinitialized coordinate layouts mimicked here and
// https://github.com/LunarG/VulkanSamples/blob/master/API-Samples/07-init_uniform_buffer/07-init_uniform_buffer.cpp#L46
//	for GLM clip-model-view-projection matrix insights.
//
// However, coordinates here may still actually appear to be right-handed, with
//	+Y down and +Z AWAY from camera, despite front-facing triangles wound clockwise.
//	Overall, refer to TestApplication::initializeViewMatricesForVulkan() to see
//	the view/projection matrices' construction that make all display properly.

// Again, see bottom of Cube3DColorTest.h for further
//	explanation and justification for these test models.
