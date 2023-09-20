//
// Cube3DTextured.cpp
//	Vulkan Convenience 3D Objects
//
// A hard-coded solid (cube being obviously 3D)
//	for test, demo, example, or any other purpose.
// Incorporates Textures and implies Normals for shading.
// Couples to Vertex shader that accepts Model-View-Projection so it looks 3D
//  and for simplicity ties-in to the most basic Diffuse Shading method.
//
// Built in OpenGL-compatible coordinates (right-handed) so Winding is
//	counter-clockwise, by our default should point front faces' normals outward.
//
// Created 7/15/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "MeshObject.h"
#include "Vertex3D.h"
#include "Vertex3DTextured.h"
#include "FixedRenderable.h"

typedef vec2	Point2D;


const Vertex3D	vertex0 = Vertex3D( 0.5f,  0.5f,  0.5f);
const Vertex3D	vertex1 = Vertex3D(-0.5f,  0.5f,  0.5f);
const Vertex3D	vertex2 = Vertex3D(-0.5f, -0.5f,  0.5f);
const Vertex3D	vertex3 = Vertex3D( 0.5f, -0.5f,  0.5f);

const Vertex3D	vertex4 = Vertex3D(-0.5f,  0.5f, -0.5f);
const Vertex3D	vertex5 = Vertex3D( 0.5f,  0.5f, -0.5f);
const Vertex3D	vertex6 = Vertex3D( 0.5f, -0.5f, -0.5f);
const Vertex3D	vertex7 = Vertex3D(-0.5f, -0.5f, -0.5f);

const Point2D	txUpLf	= Point2D(0.0f, 0.0f);
const Point2D	txUpRt	= Point2D(1.0f, 0.0f);
const Point2D	txDnLf	= Point2D(0.0f, 1.0f);
const Point2D	txDnRt	= Point2D(1.0f, 1.0f);

const Vertex3D	front	= Vertex3D( 0.0f,  0.0f,  1.0f);	// These should all
const Vertex3D	back	= Vertex3D( 0.0f,  0.0f, -1.0f);	//	be normalized,
const Vertex3D	top		= Vertex3D( 0.0f,  1.0f,  0.0f);	//	i.e. unit vectors
const Vertex3D	bottom	= Vertex3D( 0.0f, -1.0f,  0.0f);	//	with length
const Vertex3D	right	= Vertex3D( 1.0f,  0.0f,  0.0f);	//	of 1.0f.
const Vertex3D	left	= Vertex3D(-1.0f,  0.0f,  0.0f);


const Vertex3DTextured CubeVertices[] = {

	{ vertex0,	txUpRt,	front	},	// 0		// (Purposely not rendering
	{ vertex1,	txUpLf,	front	},	// 1		//	back-to-front/bottom-to-top
	{ vertex2,	txDnLf,	front	},	// 2		//	since occlusion should occur
	{ vertex2,	txDnLf,	front	},	// 3		//	minimally from backface culling
	{ vertex3,	txDnRt,	front	},	// 4		//	(especially since this model is
	{ vertex0,	txUpRt,	front	},	// 5		//	 entirely convex) and ideally
										//	via depth-testing/Z-buffering.)
	{ vertex5,	txUpRt,	top		},	// 6
	{ vertex4,	txUpLf,	top		},	// 7
	{ vertex1,	txDnLf,	top		},	// 8
	{ vertex1,	txDnLf,	top		},	// 9
	{ vertex0,	txDnRt,	top		},	// 10
	{ vertex5,	txUpRt,	top		},	// 11

	{ vertex5,	txUpRt,	right	},	// 12
	{ vertex0,	txUpLf,	right	},	// 13
	{ vertex3,	txDnLf,	right	},	// 14
	{ vertex3,	txDnLf,	right	},	// 15
	{ vertex6,	txDnRt,	right	},	// 16
	{ vertex5,	txUpRt,	right	},	// 17

	{ vertex1,	txUpRt,	left	},	// 18
	{ vertex4,	txUpLf,	left	},	// 19
	{ vertex7,	txDnLf,	left	},	// 20
	{ vertex7,	txDnLf,	left	},	// 21
	{ vertex2,	txDnRt,	left	},	// 22
	{ vertex1,	txUpRt,	left	},	// 23

	{ vertex3,	txUpRt,	bottom	},	// 24
	{ vertex2,	txUpLf,	bottom	},	// 25
	{ vertex7,	txDnLf,	bottom	},	// 26
	{ vertex7,	txDnLf,	bottom	},	// 27
	{ vertex6,	txDnRt,	bottom	},	// 28
	{ vertex3,	txUpRt,	bottom	},	// 29

	{ vertex4,	txUpRt,	back	},	// 30
	{ vertex5,	txUpLf,	back	},	// 31
	{ vertex6,	txDnLf,	back	},	// 32
	{ vertex6,	txDnLf,	back	},	// 33
	{ vertex7,	txDnRt,	back	},	// 34
	{ vertex4,	txUpRt,	back	}	// 35
};

static MeshObject Cube3DObject = {

	VertexDescriptor3DTextured,
	(void*) CubeVertices,
	N_ELEMENTS_IN_ARRAY(CubeVertices),
	0,	// first vertex
	1,	// instance count
	0,	// first instance
};


// Optionally tie shaders to this 3D object (and the MVP UBO the Vertex Shader requires)
//	allowing this object to be easily instantiated by any code that #includes this file.
// Again, this is optional, so this class can be ignored and defined locally if you want
//	to share this object between other shaders or share shaders between similar objects.
//	In that case, consider the below a usage example that the compiler will thus ignore.
//
class RenderableCube : public Renderable {						// cube, vertex + normal buffer
public:															//		+ texture sampler
	RenderableCube(UBO& refMVP)
		:	Renderable(Cube3DObject)							// ...this vertex buffer and  <──╮
	{															//								 │
		shaders = { { VERTEX,	"uv,mvp+norm=diffuv-vert.spv"},	// This shader expects... ───────┤
					{ FRAGMENT, "textuv+intens-frag.spv" } };	//								 │
		pUBOs = { refMVP };										//  ...this uniform buffer,  <───╯
		textureSpecs = { { "C4Crate.png" }, { } };				//  ...and textures too, array
	}									 // ^^^						<--	"null" terminated!
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
