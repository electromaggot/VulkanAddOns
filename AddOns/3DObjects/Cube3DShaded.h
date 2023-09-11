//
// Cube3DShaded.cpp
//	Vulkan Convenience 3D Objects
//
// A hard-coded solid (cube being obviously 3D)
//	for test, demo, example, or any other purpose.
// Incorporates Normals, so can be shaded*.
// Can't use Indices, since each Vertex requires three different Normals.
//	(Index only work with a Normal shared across all triangles including the
//	 Vertex, which smooth-shades across them all.)
// Couples to Vertex shader that accepts Model-View-Projection so it looks 3D.
//  * - and includes a tie-in to a simplistic DIFFUSE SHADING method.
//
// Built in OpenGL-compatible coordinates (right-handed) so Winding is
//	counter-clockwise, by our default should point front faces' normals outward.
//
// Created 7/15/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VertexBasedObject.h"
#include "Vertex3D.h"
#include "Vertex3DwNormal.h"
#include "FixedRenderable.h"


const Vertex3D	vertex0 = Vertex3D( 0.5f,  0.5f,  0.5f);
const Vertex3D	vertex1 = Vertex3D(-0.5f,  0.5f,  0.5f);
const Vertex3D	vertex2 = Vertex3D(-0.5f, -0.5f,  0.5f);
const Vertex3D	vertex3 = Vertex3D( 0.5f, -0.5f,  0.5f);

const Vertex3D	vertex4 = Vertex3D(-0.5f,  0.5f, -0.5f);
const Vertex3D	vertex5 = Vertex3D( 0.5f,  0.5f, -0.5f);
const Vertex3D	vertex6 = Vertex3D( 0.5f, -0.5f, -0.5f);
const Vertex3D	vertex7 = Vertex3D(-0.5f, -0.5f, -0.5f);

const Vertex3D	front	= Vertex3D( 0.0f,  0.0f,  1.0f);	// These should all
const Vertex3D	back	= Vertex3D( 0.0f,  0.0f, -1.0f);	//	be normalized,
const Vertex3D	top		= Vertex3D( 0.0f,  1.0f,  0.0f);	//	i.e. unit vectors
const Vertex3D	bottom	= Vertex3D( 0.0f, -1.0f,  0.0f);	//	with length
const Vertex3D	right	= Vertex3D( 1.0f,  0.0f,  0.0f);	//	of 1.0f.
const Vertex3D	left	= Vertex3D(-1.0f,  0.0f,  0.0f);


const Vertex3DwNormal CubeVertices[] = {

	{ vertex0,	front	},	// 0		// (Purposely not rendering
	{ vertex1,	front	},	// 1		//	back-to-front/bottom-to-top
	{ vertex2,	front	},	// 2		//	since occlusion should occur
	{ vertex2,	front	},	// 3		//	minimally from backface culling
	{ vertex3,	front	},	// 4		//	(especially since this model is
	{ vertex0,	front	},	// 5		//	 entirely convex) and ideally
										//	via depth-testing/Z-buffering.)
	{ vertex5,	top		},	// 6
	{ vertex4,	top		},	// 7
	{ vertex1,	top		},	// 8
	{ vertex1,	top		},	// 9
	{ vertex0,	top		},	// 10
	{ vertex5,	top		},	// 11

	{ vertex5,	right	},	// 12
	{ vertex0,	right	},	// 13
	{ vertex3,	right	},	// 14
	{ vertex3,	right	},	// 15
	{ vertex6,	right	},	// 16
	{ vertex5,	right	},	// 17

	{ vertex1,	left	},	// 18
	{ vertex4,	left	},	// 19
	{ vertex7,	left	},	// 20
	{ vertex7,	left	},	// 21
	{ vertex2,	left	},	// 22
	{ vertex1,	left	},	// 23

	{ vertex3,	bottom	},	// 24
	{ vertex2,	bottom	},	// 25
	{ vertex7,	bottom	},	// 26
	{ vertex7,	bottom	},	// 27
	{ vertex6,	bottom	},	// 28
	{ vertex3,	bottom	},	// 29

	{ vertex4,	back	},	// 30
	{ vertex5,	back	},	// 31
	{ vertex6,	back	},	// 32
	{ vertex6,	back	},	// 33
	{ vertex7,	back	},	// 34
	{ vertex4,	back	}	// 35
};

static VertexBasedObject Cube3DObject = {

	VertexDescriptor3DwNormal,
	(void*) CubeVertices,
	N_ELEMENTS_IN_ARRAY(CubeVertices),
	0,	// first vertex
	1,	// instance count
	0,	// first instance
};


// Optionally tie shaders to this 3D object (and the MVP UBO the Vertex Shader requires)
//	allowing this object to be easily instantiated by any code that #includes this file.
// Again, this is optional if you want to share this object between orther shaders or
//	share shaders between other objects.  In that case, consider the below an example.
//
class RenderableCube : public Renderable {						// cube, vertex + normal buffer
public:
	RenderableCube(UBO& refMVP)
		:	Renderable(Cube3DObject)							// ...this vertex buffer and  <──╮
	{															//								 │
		shaders = { { VERTEX,	"mvpNormal2diffuse-vert.spv"},	// This shader expects... ───────┤
					{ FRAGMENT, "vertColor4out-frag.spv"   } };	//								 │
		pUBOs = { refMVP };										//	...this uniform buffer.  <───╯
	}
};



#if ASCII_ART_EXPLAINER

// Cube modeled for OpenGL

			4----------5
			|\         |\
			| 1----------0 (+,+,+)
			| |        | |
	(-,-,-) 7-|--------6 |
			 \|         \|
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
