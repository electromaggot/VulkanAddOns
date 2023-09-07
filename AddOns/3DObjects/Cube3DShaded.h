//
// Cube3DShaded.cpp
//	Vulkan Convenience 3D Objects
//
// A hard-coded solid (cube being obviously 3D)
//	for test, demo, example, or any other purpose.
// Includes Normals, so can be shaded. 
// Can't use Indices, since each Vertex requires three
//	different Normals.  (Index only work with a Normal
//	shared across all triangles including the Vertex,
//	which smooth-shades across them all.)
// Couple to a Vertex shader that accepts a
//	Model-View-Projection so it looks 3D.
// Winding is counter-clockwise, which by our default
//	should make the front faces' normals face outward.
// Per Vulkan defaults: negative-Y is up and screen
//								extents appear as:
//   (-1,-1)		  (1,-1)
//		+---------------+
//		|       |       |
//		|-------+(0,0)--|
//		|		|		|
//		+---------------+
//	 (-1,1)			  (1,1)
//
// Created 7/15/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VertexBasedObject.h"
#include "Vertex3D.h"
#include "Vertex3DwNormal.h"


const Vertex3D	vertex0 = Vertex3D(-0.5f, -0.5f, -0.5f);
const Vertex3D	vertex1 = Vertex3D(-0.5f,  0.5f, -0.5f);
const Vertex3D	vertex2 = Vertex3D( 0.5f,  0.5f, -0.5f);
const Vertex3D	vertex3 = Vertex3D( 0.5f, -0.5f, -0.5f);

const Vertex3D	vertex4 = Vertex3D( 0.5f, -0.5f,  0.5f);
const Vertex3D	vertex5 = Vertex3D( 0.5f,  0.5f,  0.5f);
const Vertex3D	vertex6 = Vertex3D(-0.5f,  0.5f,  0.5f);
const Vertex3D	vertex7 = Vertex3D(-0.5f, -0.5f,  0.5f);

const Vertex3D	front	= Vertex3D( 0.5f, -0.5f,  0.5f);
const Vertex3D	back	= Vertex3D( 0.5f,  0.5f,  0.5f);
const Vertex3D	top		= Vertex3D( 0.5f, -0.5f,  0.5f);
const Vertex3D	bottom	= Vertex3D( 0.5f, -0.5f,  0.5f);
const Vertex3D	left	= Vertex3D( 0.5f,  0.5f,  0.5f);
const Vertex3D	right	= Vertex3D( 0.5f,  0.5f,  0.5f);


const Vertex3DwNormal CubeVertices[] = {

	{ vertex0,	front	},	// 0
	{ vertex1,	front	},	// 1
	{ vertex2,	front	},	// 2
	{ vertex2,	front	},	// 3
	{ vertex3,	front	},	// 4
	{ vertex0,	front	},	// 5

	{ vertex7,	top		},	// 6
	{ vertex0,	top		},	// 7
	{ vertex3,	top		},	// 8
	{ vertex3,	top		},	// 9
	{ vertex4,	top		},	// 10
	{ vertex7,	top		}	// 11

	{ vertex7,	left	},	// 12
	{ vertex6,	left	},	// 13
	{ vertex1,	left	},	// 14
	{ vertex1,	left	},	// 15
	{ vertex0,	left	},	// 16
	{ vertex7,	left	}	// 17

	{ vertex3,	right	},	// 18
	{ vertex2,	right	},	// 19
	{ vertex5,	right	},	// 20
	{ vertex5,	right	},	// 21
	{ vertex4,	right	},	// 22
	{ vertex3,	right	}	// 23

	{ vertex1,	bottom	},	// 24
	{ vertex6,	bottom	},	// 25
	{ vertex5,	bottom	},	// 26
	{ vertex5,	bottom	},	// 27
	{ vertex2,	bottom	},	// 28
	{ vertex1,	bottom	}	// 29

	{ vertex4,	back	},	// 30
	{ vertex5,	back	},	// 31
	{ vertex6,	back	},	// 32
	{ vertex6,	back	},	// 33
	{ vertex7,	back	},	// 34
	{ vertex4,	back	}	// 35
};


VertexBasedObject Cube = {

	VertexDescriptor3DwNormal,
	(void*) CubeVertices,
	N_ELEMENTS_IN_ARRAY(CubeVertices),
	0,	// first vertex
	1,	// instance count
	0,	// first instance
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
