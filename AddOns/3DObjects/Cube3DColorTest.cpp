//
// Cube3DColorTest.cpp
//	Vulkan Convenience 3D Objects
//
// A very-specific-purpose 3D cube for testing,
//	demonstration, and especially learning.
// Further "what"/"why" at bottom of this file.
//
// Created 7/15/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VertexBasedObject.h"
#include "Vertex3D.h"


const Vertex3D CubeVertices[] = {	// add 0.5f to
									//	x= y= z=
	{ -0.5f, -0.5f, -0.5f },	// 0	r0 g0 b0	black
	{ -0.5f,  0.5f, -0.5f },	// 1	r0 g1 b0	green
	{  0.5f,  0.5f, -0.5f },	// 2	r1 g1 b0	yellow
	{  0.5f, -0.5f, -0.5f },	// 3	r1 g0 b0	red

	{  0.5f, -0.5f,  0.5f },	// 4	r1 g0 b1	magenta
	{  0.5f,  0.5f,  0.5f },	// 5	r0 g0 b1	blue
	{ -0.5f,  0.5f,  0.5f },	// 6	r0 g1 b1	cyan
	{ -0.5f, -0.5f,  0.5f }		// 7	r1 g1 b1	white
};

const IndexBufferIndexType CubeIndices[] = {

	4, 5, 6, 6, 7, 4,		// rear		// rendering back-to-front
	1, 6, 5, 5, 2, 1,		// bottom	//	(& bottom-to-top)
	3, 2, 5, 5, 4, 3,		// right
	7, 6, 1, 1, 0, 7,		// left		// however, sides won't look
	7, 0, 3, 3, 4, 7,		// top		//	correct unless culled!
	0, 1, 2, 2, 3, 0,		// front
};

VertexBasedObject Cube = {

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



#if ASCII_ART
			  7----------4
			 /|         /|
	(-,-,-) 0----------3 |
			| |        | |
			| 6--------|-5 (+,+,+)
			|/         |/
			1----------2

// FRONT FACE			WINDING
				-y
				  ^
		 0        |        3
		  o ,-----------. o
(-.5,-.5)   \     |      \  (.5,-.5)
		  v  `.|  |  CCW  `
		  |   -'  |       |
 -x <-----|-------+-------|-----> +x
		  |       |  __   |
		  .  CCW  |  |.   ^
 (-.5,.5)  \      |    `.   (.5,.5)
		  o `-----------' o
		 1        |        2
				  v
				   +y
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
