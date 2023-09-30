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

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/gtx/hash.hpp>

#include <unordered_map>


namespace std {
	template<> struct hash<Vertex3DNormalTexture> {
		size_t operator()(Vertex3DNormalTexture const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
	template<> struct hash<Vertex3DTextureColor> {
		size_t operator()(Vertex3DTextureColor const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec4>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}


void ModelLoader::loadModel(string nameOBJFile)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
						  fileSystem.ModelFileFullPath(nameOBJFile).c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex3DNormalTexture, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex3DNormalTexture vertex = {};

			if (! attrib.vertices.empty()) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};
			}
			// else { should indicate error }

			if (! attrib.normals.empty()) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};
			}

			if (! attrib.texcoords.empty()) {
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			/*if (! attrib.colors.empty()) {
				vertex.color = {
					attrib.colors[4 * index.vertex_index + 0],
					attrib.colors[4 * index.vertex_index + 1],
					attrib.colors[4 * index.vertex_index + 2],
					1.0f
				};
			}
			else
				vertex.color = {1.0f, 1.0f, 1.0f, 1.0f};*/

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
