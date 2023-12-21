//
// ModelLoader.cpp
//	Vulkan Convenience 3D Objects
//
// Main comment in header file.
//
// Created 9/20/23 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "ModelLoader.h"
#include "ModelDefSpec.h"
#include "Vertex3DTypes.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <glm/gtx/hash.hpp>
#include <unordered_map>


typedef Vertex3DNormalTextureColor	CatchAllVertexType;

namespace std {
	template<> struct hash<CatchAllVertexType> {
		size_t operator()(CatchAllVertexType const& vertex) const {
			auto posn = hash<vec3>()(vertex.position);
			auto norm = hash<vec3>()(vertex.normal);
			auto texc = hash<vec2>()(vertex.texCoord);
			auto colr = hash<vec4>()(vertex.color);
			return ((posn ^ (norm << 1)) >> 1) ^ (colr ^ (texc << 1));
		}
	};
}


void ModelLoader::load(MeshObject& mesh, string nameOBJFile)
{
	AttributeBits attribits = loadTinyObj(nameOBJFile);

	mesh.vertexType.initialize(attribits);

	mesh.vertices	 = vertices.pBytes;
	mesh.vertexCount = vertices.count();
	mesh.indices	 = &indices[0];
	mesh.indexCount	 = (uint32_t) indices.size();
}

MeshIndexType ModelLoader::indexType()
{
	return indexTypeTinyObj();
}


// LOADER-IMPLEMENTATION-SPECIFIC (currently only supported: tiny_obj_loader)


MeshIndexType ModelLoader::indexTypeTinyObj()
{
	return MESH_LARGE_INDEX;	// tiny_obj_loader always returns 32-bit indices
}

AttributeBits ModelLoader::loadTinyObj(string nameOBJFile)
{
	tinyobj::attrib_t tiny;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	string warn, err;

	string fullPath = fileSystem.ModelFileFullPath(nameOBJFile);
	const char* charPath = fullPath.c_str();
	Log(RAW, "Load: model - file: %s", charPath);

	if (!tinyobj::LoadObj(&tiny, &shapes, &materials, &warn, &err, charPath)) {
		Log(RAW, "      FAILED! err \"" + err + "\" warn: " + warn);
		return 0;
	}

	// Whatever arrays tinyobj::attrib_t returns (which are non-empty) determines both
	//	which Vertex Type and shaders to use.  (wait, see related note at end of file)
	AttributeBits tinybits = ((! tiny.vertices.empty())	 ? Attribits[POSITION] : 0)
						   | ((! tiny.normals.empty())	 ? Attribits[NORMAL]   : 0)
						   | ((! tiny.texcoords.empty()) ? Attribits[TEXCOORD] : 0)
						   | ((! tiny.colors.empty())	 ? Attribits[COLOR]    : 0);

	vertices.setAttributes(tinybits);

	std::unordered_map<CatchAllVertexType, uint32_t> uniqueVertices = {};
	int numRedundantVertices = 0;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			CatchAllVertexType vertex = {};

			if (! tiny.vertices.empty()) {
				int iVec3 = 3 * index.vertex_index;
				vertex.position = { tiny.vertices[iVec3 + 0], tiny.vertices[iVec3 + 1], tiny.vertices[iVec3 + 2] };
			}
			// else { should indicate error }

			if (! tiny.normals.empty()) {
				int iVec3 = 3 * index.normal_index;
				vertex.normal = { tiny.normals[iVec3 + 0], tiny.normals[iVec3 + 1], tiny.normals[iVec3 + 2] };
			}

			if (! tiny.texcoords.empty()) {
				int iVec2 = 2 * index.texcoord_index;
				vertex.texCoord = { tiny.texcoords[iVec2 + 0], tiny.texcoords[iVec2 + 1] };
			}

			if (! tiny.colors.empty()) {
				int iVec4 = 4 * index.vertex_index;
				vertex.color = { tiny.colors[iVec4 + 0], tiny.colors[iVec4 + 1], tiny.colors[iVec4 + 2], 1.0f };
			}
			else
				vertex.color = { 1.0f, 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.count());
				vertices.push_back(vertex);
			} else {
				++numRedundantVertices;
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
	vertices.exactResize();
	Log(RAW, "      done; vertices: %d, redundant vertices culled: %d", vertices.count(), numRedundantVertices);
	return tinybits;
}


/* DEV NOTE regarding choice of SHADERS
Once the model is parsed, we know which Vertex attributes it specifies, such as normals or texture coordinates.
 So above, we dynamically choose a Vertex Type containing those attributes, then fill the Vertex Array accordingly.
What we don't do ("yet" anyway) is programmatically apply a Vertex Shader to match that Vertex format, which is a
 critical must-have.  The Vertex Shader HAS TO match the format of the Vertex Buffer provided to it.  So:
TODO: Unless DrawableSpec.shaders is specified, automatically choose/apply Shaders to match the Vertex format.
	  Obviously if the DrawableSpec *does* supply these, it will override.
Also, the Model's OBJ file may indicate, specifically name, a texure file.  So:
TODO: If the OBJ references a Texture File that DrawableSpec.textures does not, add to that array and open it.
	  Thus a user can use an OBJ file without having to look inside it in order to specify these things.
These two additions may become required once users can import and select an OBJ via GUI from the web.
*/
