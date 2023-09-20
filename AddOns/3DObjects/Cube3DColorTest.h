//
// Cube3DColorTest.h
//	Vulkan Convenience 3D Objects
//
// A very-specific-purpose 3D cube for testing,
//	demonstration, and especially learning.
// Further "what"/"why" at bottom of this file.
//
// Modeled using Right-Handed coordinate system
//	of OpenGL - the default for this engine - as
//	are worldwide majority of existing 3D models.
//
// Created 7/15/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "MeshObject.h"
#include "Vertex3D.h"
#include "FixedRenderable.h"


const Vertex3D CubeVertices[] = {	// add 0.5f to
									//	x= y= z=
	{  0.5f,  0.5f,  0.5f },	// 0	r1 g1 b1	white
	{ -0.5f,  0.5f,  0.5f },	// 1	r0 g1 b1	cyan
	{ -0.5f, -0.5f,  0.5f },	// 2	r0 g0 b1	blue
	{  0.5f, -0.5f,  0.5f },	// 3	r1 g0 b1	magenta

	{ -0.5f,  0.5f, -0.5f },	// 4	r0 g1 b0	green
	{  0.5f,  0.5f, -0.5f },	// 5	r1 g1 b0	yellow
	{  0.5f, -0.5f, -0.5f },	// 6	r1 g0 b0	red
	{ -0.5f, -0.5f, -0.5f }		// 7	r0 g0 b0	black
};

const IndexBufferIndexType CubeIndices[] = {

	4, 5, 6, 6, 7, 4,		// rear		// rendering back-to-front
	3, 2, 7, 7, 6, 3,		// bottom	//	(& bottom-to-top)
	5, 0, 3, 3, 6, 5,		// right
	1, 4, 7, 7, 2, 1,		// left		// however, sides won't look
	5, 4, 1, 1, 0, 5,		// top		//	correct unless culled!
	0, 1, 2, 2, 3, 0,		// front
};

static MeshObject Cube3DObject = {

	VertexDescriptor3D,
	(void*) CubeVertices,
	N_ELEMENTS_IN_ARRAY(CubeVertices),
	0,	// first vertex
	1,	// instance count
	0,	// first instance

	(void*) CubeIndices,
	N_ELEMENTS_IN_ARRAY(CubeIndices),
	0,	// first index
	0	// vertex offset
};


class RenderableCube : public Renderable {					// cube, vertex + index buffer
public:
	RenderableCube(UBO& refMVP)
		:	Renderable(Cube3DObject)						// ...this vertex buffer and  <──╮
	{														//								 │
		shaders = { { VERTEX,	"mvp+xyz=xyz-vert.spv" },	// This shader expects... ───────┤
					{ FRAGMENT, "xyz=color-frag.spv" } };	//								 │
		pUBOs = { refMVP };									//	...this uniform buffer.  <───╯
	}
};



#if ASCII_ART_EXPLAINER

// Cube modeled for OpenGL

			4----------5
			|\         |\
			| 1----------0 (+,+,+)
			| |        | |  white
	(-,-,-) 7-|--------6 |
	 black	 \|         \|
			  2----------3

// FRONT FACE			WINDING
				+y
				  ^
		 1        |        0
		  o ,-----------< o
 (-.5,.5)  /      |     ,-. (.5,.5)
		  |  CCW  |  |,'  |
		  |       |  '--  |
 -x <-----|-------+-------|-----> +x
		  |  __.  |       |
		  |  ,'|  |  CCW  |
(-.5,-.5) `-'     |      /  (.5,-.5)
		  o >-----------' o
		 2        |        3
				  v
				   -y
#endif

// What's "good" about this: (and "interesting")
//	- a couldn't-be-simpler 3D Vertex Array.
//	- example of using Index Buffer for 3D.
//	- demonstrates coloring triangles without
//	  colored vertices, via fragment shader.
// What's "bad": (or "dangerous!")
//	- no normals, so this can't be lit/shaded
//	  (if non-colored, you can't tell faces apart).
//  - ordering of Index Buffer dictates the order
//	  triangles render, potentially overwriting
//	  farther-away faces onto nearer ones.  Unless you:
//		1. use a Depth Buffer.
//		2. cull away-facing triangles.
//		3. coordinate index (thus render) order with
//		   camera position... and don't move camera!
//			(but rotating cube will still look bad)
// Here we'll rely entirely on #2 and not require a
//	Depth Buffer (though it's almost always desirable).
//	This works because the cube is entirely convex.
//
// SO WHAT IS THIS GOOD FOR?
//	When paired with both intended shaders:
//	- vertex shader which passes vertex position to:
//	- fragment shader, which interpolates incoming
//	  value used as color...
//	This directly ties the vertex's RGB color to its
//	normalized XYZ position.  Result: you can immediately
//	tell if a vertex is in the position you're expecting,
//	to verify the projection matrix and sanity-check
//	your expected coordinate system.  Also with
//	back-face-culling, validates your winding order.
