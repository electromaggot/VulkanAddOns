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


AttributeBits ModelLoader::loadModel(string nameOBJFile)
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

	// Whatever arrays tinyobj::attrib_t returns (which are non-empty) determines
	//	both which Vertex Type and shaders to use.
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
	Log(RAW, "      done; vertices: %d, redundant vertices culled: %d", vertices.count(), numRedundantVertices);
	return tinybits;
}
