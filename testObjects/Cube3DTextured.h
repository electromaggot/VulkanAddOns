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

const vec2	txUpLf	( 0.0f, 0.0f);
const vec2	txUpRt	( 1.0f, 0.0f);
const vec2	txDnLf	( 0.0f, 1.0f);
const vec2	txDnRt	( 1.0f, 1.0f);

const vec3	front	( 0.0f,  0.0f,  1.0f);		// These should all be normalized, i.e. unit vectors with length 1.0f.
const vec3	back	( 0.0f,  0.0f, -1.0f);
const vec3	top		( 0.0f,  1.0f,  0.0f);
const vec3	bottom	( 0.0f, -1.0f,  0.0f);
const vec3	right	( 1.0f,  0.0f,  0.0f);
const vec3	left	(-1.0f,  0.0f,  0.0f);


const Vertex3DNormalTexture CubeVertices[] = {	// (Purposely not rendering back-to-front/bottom-to-top since occlusion
												//	should occur minimally from backface culling (especially since this
	{ vertex0,	front,	txUpRt	},	// 0		//	model is entirely convex) and ideally via depth-testing/Z-buffering.)
	{ vertex1,	front,	txUpLf	},	// 1
	{ vertex2,	front,	txDnLf	},	// 2
	{ vertex2,	front,	txDnLf	},	// 3
	{ vertex3,	front,	txDnRt	},	// 4
	{ vertex0,	front,	txUpRt	},	// 5
	{ vertex5,	top,	txUpRt	},	// 6
	{ vertex4,	top,	txUpLf	},	// 7
	{ vertex1,	top,	txDnLf	},	// 8
	{ vertex1,	top,	txDnLf	},	// 9
	{ vertex0,	top,	txDnRt	},	// 10
	{ vertex5,	top,	txUpRt	},	// 11
	{ vertex5,	right,	txUpRt	},	// 12
	{ vertex0,	right,	txUpLf	},	// 13
	{ vertex3,	right,	txDnLf	},	// 14
	{ vertex3,	right,	txDnLf	},	// 15
	{ vertex6,	right,	txDnRt	},	// 16
	{ vertex5,	right,	txUpRt	},	// 17
	{ vertex1,	left,	txUpRt	},	// 18
	{ vertex4,	left,	txUpLf	},	// 19
	{ vertex7,	left,	txDnLf	},	// 20
	{ vertex7,	left,	txDnLf	},	// 21
	{ vertex2,	left,	txDnRt	},	// 22
	{ vertex1,	left,	txUpRt	},	// 23
	{ vertex3,	bottom,	txUpRt	},	// 24
	{ vertex2,	bottom,	txUpLf	},	// 25
	{ vertex7,	bottom,	txDnLf	},	// 26
	{ vertex7,	bottom,	txDnLf	},	// 27
	{ vertex6,	bottom,	txDnRt	},	// 28
	{ vertex3,	bottom,	txUpRt	},	// 29
	{ vertex4,	back,	txUpRt	},	// 30
	{ vertex5,	back,	txUpLf	},	// 31
	{ vertex6,	back,	txDnLf	},	// 32
	{ vertex6,	back,	txDnLf	},	// 33
	{ vertex7,	back,	txDnRt	},	// 34
	{ vertex4,	back,	txUpRt	}	// 35
};


// Optionally tie shaders to this 3D object (and the MVP UBO the Vertex Shader requires)
//	allowing this object to be easily instantiated by any code that #includes this file.
// Again, this is optional, so this class can be ignored and defined locally if you want
//	to share this object between other shaders or share shaders between similar objects.
//	In that case, consider the below a usage example that the compiler will thus ignore.
//
class RenderableCubeTextured : public DrawableSpecifier, public gxMatrix
{
	VertexDescription<Vertex3DNormalTexture> vertexDescriptor;
	MeshObject cube3DObject = { vertexDescriptor, (void*) CubeVertices, N_ELEMENTS_IN_ARRAY(CubeVertices) };
	UBO uboMatrix;
public:															// cube, vertex + normal buffer
	RenderableCubeTextured(UBO& refMVP)							//			+ texture sampler
		:	DrawableSpecifier(cube3DObject),					// ...this vertex buffer and  <──╮
			uboMatrix(matrix)									//								 │
	{															//								 │
		name	=	"BoomBox";									//								 │
		shaders	= { { VERTEX,	"ubos+uvnorm=diffuv-vert.spv"},	// This shader expects... ───────┤
					{ FRAGMENT, "textuv2+intens-frag.spv" } };	//								 │
		pUBOs = { refMVP, uboMatrix };							//  ...these uniform buffers, <──╯
		textures = { { "C4Crate.png" } };						//  ...and textures too (as an
	}															//	   array, here with default properties).
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
		  |  CCW  |  |,´  |
		  |       |  ·--  |
 -x <-----|-------+-------|-----> +x
		  |  __.  |       |
		  |  ,´|  |  CCW  |
(-.5,-.5) `-´     |      /  (.5,-.5)
		  o >-----------´ o
		 2        |        3
				  v
				   -y
#endif
