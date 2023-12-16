//
// STUB tiny_obj_loader.h
//
// Only if you don't need 'model.obj' loading.  But if you do, replace this file with:
//
//	$ wget --backups=1 https://raw.githubusercontent.com/tinyobjloader/tinyobjloader/release/tiny_obj_loader.h
//
// from `https://github.com/tinyobjloader/tinyobjloader.git`
//

class tinyobj
{
public:
	struct attrib_t {
		vector<float> vertices;
		vector<float> normals;
		vector<float> texcoords;
		vector<float> colors;
	};
	struct index_t {
		int vertex_index;
		int normal_index;
		int texcoord_index;
	};
	struct mesh_t {
		vector<index_t> indices;
	};
	struct shape_t {
		mesh_t mesh;
	};
	typedef int material_t;

	static bool LoadObj(attrib_t* a, vector<shape_t>* s, vector<material_t>* m,
						string* w, string* e, const char* c) {
		*e = "The 'Model.OBJ' loader is stubbed-out. Please see 'tiny_obj_loader.h' and download the real one!";
		*w = "So you won't see the model actually display."
		return false;
	};
}; 
