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

#include "../../VertexTypes/Vertex3DTypes.h"

#include "Universal.h"
#include "FileSystem.h"


class ModelLoader
{
	FileSystem	fileSystem;
public: // for now
	vector<Vertex3DNormalTexture> vertices;	//TJ_TODO: too special-purpose... this could get tricky
	vector<uint32_t> indices;

public:
	void loadModel(string nameOBJFile);

};
