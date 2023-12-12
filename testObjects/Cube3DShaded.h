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
#include "Vertex3DTypes.h"
#include "FixedRenderable.h"
#include "MeshObject.h"


const vec3	vertex0	( 0.5f,  0.5f,  0.5f);
const vec3	vertex1	(-0.5f,  0.5f,  0.5f);
const vec3	vertex2	(-0.5f, -0.5f,  0.5f);
const vec3	vertex3	( 0.5f, -0.5f,  0.5f);

const vec3	vertex4	(-0.5f,  0.5f, -0.5f);
const vec3	vertex5	( 0.5f,  0.5f, -0.5f);
const vec3	vertex6	( 0.5f, -0.5f, -0.5f);
const vec3	vertex7	(-0.5f, -0.5f, -0.5f);

const vec3	front	( 0.0f,  0.0f,  1.0f);		// These should all be normalized, i.e. unit vectors with length 1.0f.
const vec3	back	( 0.0f,  0.0f, -1.0f);
const vec3	top		( 0.0f,  1.0f,  0.0f);
const vec3	bottom	( 0.0f, -1.0f,  0.0f);
const vec3	right	( 1.0f,  0.0f,  0.0f);
const vec3	left	(-1.0f,  0.0f,  0.0f);


const Vertex3DNormal CubeVertices[] = {	// (Purposely not rendering back-to-front/bottom-to-top since occlusion
										//	should occur minimally from backface culling (especially since this
	{ vertex0,	front	},	// 0		//	model is entirely convex) and ideally via depth-testing/Z-buffering.)
	{ vertex1,	front	},	// 1
	{ vertex2,	front	},	// 2
	{ vertex2,	front	},	// 3
	{ vertex3,	front	},	// 4
	{ vertex0,	front	},	// 5

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


// Optionally tie shaders to this 3D object (and the MVP UBO the Vertex Shader requires)
//	allowing this object to be easily instantiated by any code that #includes this file.
// Again, this is optional if you want to share this object between orther shaders or
//	share shaders between other objects.  In that case, consider the below an example.
//
class RenderableCubeShaded : public DrawableSpecifier {
	VertexDescription<Vertex3DNormal> vertexDescriptor;
	MeshObject cube3DObject = { vertexDescriptor, (void*) CubeVertices,
									  N_ELEMENTS_IN_ARRAY(CubeVertices) };
public:
	RenderableCubeShaded(UBO& refMVP)							// cube, vertex + normal buffer
		:	DrawableSpecifier(cube3DObject)						// ...this vertex buffer and  <──╮
	{															//								 │
		name	=	"CubeSolidShaded";							//								 │
		shaders = { { VERTEX,	"mvp+normal=diffuse-vert.spv"},	// This shader expects... ───────┤
					{ FRAGMENT, "intensity=color-frag.spv" } };	//								 │
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
