//
// gxMatrix.h
//	gxEngine - Vulkan game/graphics classes
//
// While gxEngine embraces GLM, extend and overlay its mat4 to allow:
//	- all gxObject3Ds to derive from this and have a matrix at their core.
//	- syntactic conveniences like  .x  or  .ptPosition3D
//	- use this very matrix directly as the M in MVP (model-view-projection).
//
// Created 9/20/08 by Tadd Jensen
//	© 2023 Megaphone Games under <TBD> License (i.e. project will use other FOSS-licensed code)
//
#ifndef gxMatrix_h
#define gxMatrix_h

#include "VulkanMath.h"

#ifndef M_PI						// if not already defined by SDL,
	#define _USE_MATH_DEFINES		//	for M_PI on Windows via:
#endif
#include <math.h>					// for sin, cos


class gxMatrix
{
public:
	union // (anonymous)
	{
		mat4	matrix;
		float	mtx[16];
		float	f4x4[4][4];
		struct // (anonymous)
		{
			// ROW 0
			union // (anonymous)
			{
				float row0[4];
				vec2 rotation2D;
				vec3 rotation3D;
				struct // (anonymous)
				{
					union // (anonymous)	scale and/or rotate:
					{
						float zRotX1;		// Z rotation (2D) unit vector X
						float scaleX;		// pure scaling (no rotation) X
						float scaleX1;		// 1st X scale component if rotated
						float mtx_0_0;
					};
					union // (anonymous)	scale and/or rotate:
					{
						float zRotY1;		// Z rotation (2D) unit vector Y
						float scaleY1;		// 1st Y scale component if rotated
						float mtx_0_1;
					};
					float mtx_0_2;
					float mtx_0_3;		// should always be 0
				};
			};
			// ROW 1
			union // (anonymous)
			{
				float row1[4];
				struct // (anonymous)
				{
					union // (anonymous)	scale and/or rotate:
					{
						float zRotNegY2;	// Z rotation (2D) NEGATIVE unit vector Y
						float scaleX2;		// 2nd X scale component if rotated
						float mtx_1_0;
					};
					union // (anonymous)	scale and/or rotate:
					{
						float zRotX2;		// Z rotation (2D) unit vector X
						float scaleY;		// scaling-only (no rotation) Y
						float scaleY2;		// 2nd Y scale component if rotated
						float mtx_1_1;
					};
					float mtx_1_2;
					float mtx_1_3;		// should always be 0
				};
			};
			// ROW 2
			union // (anonymous)
			{
				float row2[4];
				struct // (anonymous)
				{
					float mtx_2_0;
					float mtx_2_1;
					union // (anonymous)	scale Z
					{
						float scaleZ;
						float mtx_2_2;
					};
					float mtx_2_3;		// should always be 0
				};
			};
			// ROW 3
			union // (anonymous)
			{
				float row3[4];
				vec2 position2D;
				vec3 position3D;
				struct // (anonymous)
				{
					union // (anonymous)	translate X*
					{
						float x;
						float mtx_3_0;							// (* - IMPORTANT! Beware that these only
					};											//		apply translation correctly on an
					union // (anonymous)	translate Y*		//		IDENTITY (non-transformed) matrix,
					{											//		otherwise they must be multiplied
						float y;								//		into said matrix.)
						float mtx_3_1;
					};
					union // (anonymous)	translate Z*
					{
						float z;
						float mtx_3_2;
					};
					float mtx_3_3;		// should always be 1
				};
			};
		};
	};
	
	gxMatrix();

	void setIdentity();

	void setScale(float unitFactor);

	void setRotateXAxis(float radsAngle);
	void setRotateYAxis(float radsAngle);
	void setRotateZAxis(float radsAngle);

	void mulRotateXAxis(float radsAngle);
	void mulRotateYAxis(float radsAngle);
	void mulRotateZAxis(float radsAngle);

	void matrix3DMultiply(float (&m1)[16], float (&m2)[16], float (&result)[16]);

	void multiplyBy(float (&m2)[16]);
	void translateBy(float xlX, float xlY, float xlZ);
};

#endif	// gxMatrix_h
