//
// ModelDefSpec.h - 3D Model Definition Specifier
//	Vulkan Add-Ons
//
// Model may be parsed directly from a definition file (for a format
//	that is supported), created from some indirect definition (like
//	a truetype font), or generated procedurally.
//
// Created 9/15/22 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#ifndef ModelDefSpec_h
#define ModelDefSpec_h

enum SpecType {
	UNSPECIFIED,
	FONT_3D,
	OBJ_FILE_TINY,				// use TinyOBJLoader.h
	OBJ_FILE_FAST,				// use our own faster OBJ loader
	OBJ_FILE = OBJ_FILE_TINY	// default to this loader
};

struct ModelDefSpec {
	string		filename = "";
	SpecType	type	 = UNSPECIFIED;

	bool isRequested() {
		return ! filename.empty() && type != UNSPECIFIED;
	}
};

#endif	// ModelDefSpec_h
