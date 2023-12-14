//
// TestModel.cpp
//	Vulkan Convenience 3D Objects
//
// Hard-coded specification of a test model OBJ file, in this case
//	the dodecahedron model data included in the bundle.
//	This model originated from OpenGL; render in Vulkan accordingly.
//	There's no texturing or vertex color, so just use shader with
//	normal-based/calculated/simplistic shading.
//
// Created 7/15/20 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "FixedRenderable.h"
#include "Vertex3DTypes.h"
#include "MeshObject.h"

#include "ModelLoader.h"


class DrawableObject : public DrawableSpecifier {

	VertexDescriptionDynamic vertexDescriptor;
	MeshObject object3D = { vertexDescriptor };

public:
	DrawableObject(UBO& refMVP)
		:	DrawableSpecifier(object3D)
	{
		ModelLoader  load(mesh, "viking_room.obj");
		object3D.indexType = MESH_LARGE_INDEX;	// temporary, tiny_obj_loader always returns 32-bit indices

		shaders = { { VERTEX,	"uv,mvp+norm=diffuv-vert.spv"},
					{ FRAGMENT, "textuv+intens-frag.spv" } };
		pUBOs = { refMVP };
		textures = { { "viking_room.png" }, { } };
		customize = SHOW_BACKFACES;
	}
};
