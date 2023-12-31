//
// ModelLoader.h
//	Vulkan Convenience 3D Objects
//
// Encapsulate loading of a 3D model into a MeshObject, which
//	describes vertex layout, vertex buffer, and possibly index buffer.
//
// Created 9/20/23 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef ModelLoader_h
#define ModelLoader_h

#include "VerticesDynamic.h"
#include "FileSystem.h"
#include "MeshObject.h"


class ModelLoader
{
	VerticesDynamic	 vertices;
	vector<uint32_t> indices;

	FileSystem		 fileSystem;

public:
	void load(MeshObject& mesh, string nameOBJFile);
	MeshIndexType indexType();

	// LOADER-IMPLEMENTATION-SPECIFIC (currently only supported: tiny_obj_loader)

	AttributeBits loadTinyObj(string nameOBJFile);
	MeshIndexType indexTypeTinyObj();
};

#endif	// ModelLoader_h
