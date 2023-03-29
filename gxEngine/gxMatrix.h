//
// gxMatrix.h
//	gxEngine (Vulkan game/graphics engine)
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
		mat4	m4;
		float	f16[16];
		float	f4x4[4][4];
		struct // (anonymous)
		{
			// ROW 0
			union // (anonymous)
			{
				float row0[4];
				vec2 vecRotation2D;
				vec3 vecRotation3D;
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
						float ZRotNegY2;	// Z rotation (2D) NEGATIVE unit vector Y
						float ScaleX2;		// 2nd X scale component if rotated
						float mtx_1_0;
					};
					union // (anonymous)	scale and/or rotate:
					{
						float ZRotX2;		// Z rotation (2D) unit vector X
						float ScaleY;		// scaling-only (no rotation) Y
						float ScaleY2;		// 2nd Y scale component if rotated
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
						float ScaleZ;
						float mtx_2_2;
					};
					float mtx_2_3;		// should always be 0
				};
			};
			// ROW 3
			union // (anonymous)
			{
				float row3[4];
				vec2 ptPosition2D;
				vec3 ptPosition3D;
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
	
	gxMatrix()
	{
		SetIdentity();
	}
	
	void SetIdentity()
	{
		m4 = mat4(1.0f);
	}

	/*TJ_TODO: final disposition of the following, remains TBD...
	void RotateXAxisOnly(float radsAngle);
	void RotateYAxisOnly(float radsAngle);
	void RotateZAxisOnly(float radsAngle);

	void RotateXAxisMult(float radsAngle);
	void RotateYAxisMult(float radsAngle);
	void RotateZAxisMult(float radsAngle);

	void Matrix3DMultiply(GLfloat (&m1)[16], GLfloat (&m2)[16], GLfloat (&result)[16]);

	void MultiplyBy(GLfloat (&m2)[16]);
	void TranslateBy(GLfloat xlX, GLfloat xlY, GLfloat xlZ);


	gxPoint2D* position2D() { return (gxPoint2D*) &ptPosition2D; }
	gxPoint3D* position3D() { return (gxPoint3D*) &ptPosition3D; }

	GXpoint2D* Position2D() { return (GXpoint2D*) &ptPosition2D; }
	GXpoint3D* Position3D() { return (GXpoint3D*) &ptPosition3D; }

	gxVector2D* rotation2D() { return (gxVector2D*) &vecRotation2D; }
	gxVector3D* rotation3D() { return (gxVector3D*) &vecRotation3D; }

	GXvector2D* Rotation2D() { return (GXvector2D*) &vecRotation2D; }
	GXvector3D* Rotation3D() { return (GXvector3D*) &vecRotation3D; }*/
};

#endif	// gxMatrix_h
