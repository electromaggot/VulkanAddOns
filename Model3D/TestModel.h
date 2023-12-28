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


class RenderableTestModel : public DrawableSpecifier {

	VertexDescriptionDynamic vertexDescriptor;
	MeshObject object3D = { vertexDescriptor };
	ModelLoader model;

public:
	RenderableTestModel(UBO& refMVP)
		:	DrawableSpecifier(object3D)
	{
		name	=	"viking_room.obj";			// object's "name" can arbitrarily be same as its filename
		model.load(mesh, name);
		object3D.indexType = model.indexType();

		shaders = { { VERTEX,	"uv,mvp+norm=diffuv-vert.spv"},
					{ FRAGMENT, "textuv+intens-frag.spv" } };
		pUBOs = { refMVP };
		textures = { { "viking_room.png" }, { } };
		customize = SHOW_BACKFACES;
	}
};
