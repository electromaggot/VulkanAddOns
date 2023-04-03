//
// gxMatrix.cpp
//	gxEngine (Vulkan game/graphics engine)
//
// See header file for main comment.
//
// Created 9/20/08 by Tadd Jensen
//	© 2023 Megaphone Games under <TBD> License (i.e. project will use other FOSS-licensed code)
//
#include "gxMatrix.h"
#include <string.h>			// for memset

#pragma GCC diagnostic ignored "-Wunused-function"


gxMatrix::gxMatrix()
{
	setIdentity();
}


void gxMatrix::setIdentity()
{
	#if NO_GLM
		memset(mtx, 0, sizeof(mtx));
		mtx_0_0 = mtx_1_1 = mtx_2_2 = mtx_3_3 = 1.0f;
	#else
		matrix = mat4(1.0f);
	#endif
}


// The following functions do a really fast rotate around a single axis, wiping out any other rotations on other axes.
//	Only use one of these if you need just one simple rotation or as the first of subsequent rotations multiplied-in.
//
void gxMatrix::setRotateXAxis(float radsAngle)
{
	float sine = sinf(radsAngle);
	float cosine = cosf(radsAngle);

	mtx_0_0 = 1.0f;		mtx_0_1 = 0.0f;		mtx_0_2 = 0.0f;
	mtx_1_0 = 0.0f;		mtx_1_1 = cosine;	mtx_1_2 = -sine;
	mtx_2_0 = 0.0f;		mtx_2_1 = sine;		mtx_2_2 = cosine;
}

void gxMatrix::setRotateYAxis(float radsAngle)
{
	float sine = sinf(radsAngle);
	float cosine = cosf(radsAngle);

	mtx_0_0 = cosine;	mtx_0_1 = 0.0f;		mtx_0_2 = sine;
	mtx_1_0 = 0.0f;		mtx_1_1 = 1.0f;		mtx_1_2 = 0.0f;
	mtx_2_0 = -sine;	mtx_2_1 = 0.0f;		mtx_2_2 = cosine;
}

void gxMatrix::setRotateZAxis(float radsAngle)
{
	float sine = sinf(radsAngle);
	float cosine = cosf(radsAngle);

	mtx_0_0 = cosine;	mtx_0_1 = -sine;	mtx_0_2 = 0.0f;
	mtx_1_0 = sine;		mtx_1_1 = cosine;	mtx_1_2 = 0.0f;
	mtx_2_0 = 0.0f;		mtx_2_1 = 0.0f;		mtx_2_2 = 1.0f;
}


// These functions are for multiple rotation operations and retain any previous rotation by multiplying it in.  This
//	can also accumulate a rotation on specific axes. Unfortunately, if you need to use too many of these, glRotatef()
//	might be better?  (Although I don't know how optimized it is and I haven't made any comparisons.)
//
void gxMatrix::mulRotateXAxis(float radsAngle)
{
	float sine = sinf(radsAngle);
	float cosine = cosf(radsAngle);

	float col1 = mtx_0_1, col2 = mtx_0_2;
	mtx_0_1 = col1 * cosine + col2 * sine;	mtx_0_2 = col2 * cosine - col1 * sine;
	col1 = mtx_1_1;							col2 = mtx_1_2;
	mtx_1_1 = col1 * cosine + col2 * sine;	mtx_1_2 = col2 * cosine - col1 * sine;
	col1 = mtx_2_1;							col2 = mtx_2_2;
	mtx_2_1 = col1 * cosine + col2 * sine;	mtx_2_2 = col2 * cosine - col1 * sine;
}

void gxMatrix::mulRotateYAxis(float radsAngle)
{
	float sine = sinf(radsAngle);
	float cosine = cosf(radsAngle);

	float col0 = mtx_0_0, col2 = mtx_0_2;
	mtx_0_0 = col0 * cosine - col2 * sine;	mtx_0_2 = col0 * sine + col2 * cosine;
	col0 = mtx_1_0;							col2 = mtx_1_2;
	mtx_1_0 = col0 * cosine - col2 * sine;	mtx_1_2 = col0 * sine + col2 * cosine;
	col0 = mtx_2_0;							col2 = mtx_2_2;
	mtx_2_0 = col0 * cosine - col2 * sine;	mtx_2_2 = col0 * sine + col2 * cosine;

	//	col0 = mtx_3_0;						col2 = mtx_3_2;
	//	mtx_3_1 = col0 * cosine - col2 * sine;	mtx_3_2 = col0 * sine + col2 * cosine;
}

void gxMatrix::mulRotateZAxis(float radsAngle)
{
	float sine = sinf(radsAngle);
	float cosine = cosf(radsAngle);

	float col0 = mtx_0_0, col1 = mtx_0_1;
	mtx_0_0 = col0 * cosine + col1 * sine;	mtx_0_1 = col1 * cosine - col0 * sine;
	col0 = mtx_1_0;							col1 = mtx_1_1;
	mtx_1_0 = col0 * cosine + col1 * sine;	mtx_1_1 = col1 * cosine - col0 * sine;
	col0 = mtx_2_0;							col1 = mtx_2_1;
	mtx_2_0 = col0 * cosine + col1 * sine;	mtx_2_1 = col1 * cosine - col0 * sine;
}


// OpenGL ES hardware accelerates Vector * Matrix but not Matrix * Matrix
/*
These defines, the fast sine function, and the vectorized version of the
matrix multiply function below are based on the Matrix4Mul method from
the vfp-math-library. This code has been modified, and is subject to
the original license terms and ownership as follow:

VFP math library for the iPhone / iPod touch

Copyright (c) 2007-2008 Wolfgang Engel and Matthias Grundmann
http://code.google.com/p/vfpmathlibrary/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising
from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must
not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
*/
static inline float fastAbs(float x) { return (x < 0) ? -x : x; }
static inline float fastSinf(float x)
{
	// fast sin function; maximum error is 0.001
	const float P = 0.225f;

	x = x * (float) M_1_PI;
	#ifndef PLATFORM_WINDOWS
		int k = (int) round(x);
	#else // Microsoft after 30 years, slow to add round()... 
		int k = (int) ((x > 0.0f) ? (x + 0.5f) : (x - 0.5f));
	#endif
	x = x - k;

	float y = (4 - 4 * fastAbs(x)) * x;

	y = P * (y * fastAbs(y) - y) + y;

	return (k & 1) ? -y : y;
}

#ifdef PLATFORM_IOS_DEVICE
#define VFP_CLOBBER_S0_S31	"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8",  \
							"s9", "s10", "s11", "s12", "s13", "s14", "s15", "s16",  \
							"s17", "s18", "s19", "s20", "s21", "s22", "s23", "s24",  \
							"s25", "s26", "s27", "s28", "s29", "s30", "s31"
#define VFP_VECTOR_LENGTH(VEC_LENGTH)	"fmrx    r0, fpscr                         \n\t" \
										"bic     r0, r0, #0x00370000               \n\t" \
										"orr     r0, r0, #0x000" #VEC_LENGTH "0000 \n\t" \
										"fmxr    fpscr, r0                         \n\t"
#define VFP_VECTOR_LENGTH_ZERO	"fmrx    r0, fpscr            \n\t" \
								"bic     r0, r0, #0x00370000  \n\t" \
								"fmxr    fpscr, r0            \n\t" 
#endif
//extern "C"{
/*static inline*/ void gxMatrix::matrix3DMultiply(float(&m1)[16], float(&m2)[16], float(&result)[16]) // inline so not really pushing these on stack, but beware later
{																					//TJ_TODO: clean up later...  didn't I already do this? seems awfully familiar
	#ifdef PLATFORM_IOS_DEVICE
		__asm__ __volatile__( VFP_VECTOR_LENGTH(3)

			// Interleaving loads and adds/muls for faster calculation.
			// Let A:=src_ptr_1, B:=src_ptr_2, then
			// function computes A*B as (B^T * A^T)^T.

			// Load the whole matrix into memory.
			"fldmias  %2, {s8-s23}    \n\t"
			// Load first column to scalar bank.
			"fldmias  %1!, {s0-s3}    \n\t"
			// First column times matrix.
			"fmuls s24, s8, s0        \n\t"
			"fmacs s24, s12, s1       \n\t"

			// Load second column to scalar bank.
			"fldmias %1!,  {s4-s7}    \n\t"

			"fmacs s24, s16, s2       \n\t"
			"fmacs s24, s20, s3       \n\t"
			// Save first column.
			"fstmias  %0!, {s24-s27}  \n\t"

			// Second column times matrix.
			"fmuls s28, s8, s4        \n\t"
			"fmacs s28, s12, s5       \n\t"

			// Load third column to scalar bank.
			"fldmias  %1!, {s0-s3}    \n\t"

			"fmacs s28, s16, s6       \n\t"
			"fmacs s28, s20, s7       \n\t"
			// Save second column.
			"fstmias  %0!, {s28-s31}  \n\t"

			// Third column times matrix.
			"fmuls s24, s8, s0        \n\t"
			"fmacs s24, s12, s1       \n\t"

			// Load fourth column to scalar bank.
			"fldmias %1,  {s4-s7}    \n\t"

			"fmacs s24, s16, s2       \n\t"
			"fmacs s24, s20, s3       \n\t"
			// Save third column.
			"fstmias  %0!, {s24-s27}  \n\t"

			// Fourth column times matrix.
			"fmuls s28, s8, s4        \n\t"
			"fmacs s28, s12, s5       \n\t"
			"fmacs s28, s16, s6       \n\t"
			"fmacs s28, s20, s7       \n\t"
			// Save fourth column.
			"fstmias  %0!, {s28-s31}  \n\t"

			VFP_VECTOR_LENGTH_ZERO
			: "=r" (result), "=r" (m2)
			: "r" (m1), "0" (result), "1" (m2)
			: "r0", "cc", "memory", VFP_CLOBBER_S0_S31
		);
	#else

		result[0]  = m1[0] * m2[0]  + m1[4] * m2[1]  + m1[8]  * m2[2]  + m1[12] * m2[3];
		result[1]  = m1[1] * m2[0]  + m1[5] * m2[1]  + m1[9]  * m2[2]  + m1[13] * m2[3];
		result[2]  = m1[2] * m2[0]  + m1[6] * m2[1]  + m1[10] * m2[2]  + m1[14] * m2[3];
		result[3]  = m1[3] * m2[0]  + m1[7] * m2[1]  + m1[11] * m2[2]  + m1[15] * m2[3];

		result[4]  = m1[0] * m2[4]  + m1[4] * m2[5]  + m1[8]  * m2[6]  + m1[12] * m2[7];
		result[5]  = m1[1] * m2[4]  + m1[5] * m2[5]  + m1[9]  * m2[6]  + m1[13] * m2[7];
		result[6]  = m1[2] * m2[4]  + m1[6] * m2[5]  + m1[10] * m2[6]  + m1[14] * m2[7];
		result[7]  = m1[3] * m2[4]  + m1[7] * m2[5]  + m1[11] * m2[6]  + m1[15] * m2[7];

		result[8]  = m1[0] * m2[8]  + m1[4] * m2[9]  + m1[8]  * m2[10] + m1[12] * m2[11];
		result[9]  = m1[1] * m2[8]  + m1[5] * m2[9]  + m1[9]  * m2[10] + m1[13] * m2[11];
		result[10] = m1[2] * m2[8]  + m1[6] * m2[9]  + m1[10] * m2[10] + m1[14] * m2[11];
		result[11] = m1[3] * m2[8]  + m1[7] * m2[9]  + m1[11] * m2[10] + m1[15] * m2[11];

		result[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
		result[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
		result[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
		result[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
	#endif
}

void gxMatrix::multiplyBy(float(&m2)[16])
{
	float m0[16];

	m0[0]  = mtx[0] * m2[0]	 + mtx[4] * m2[1]  +  mtx[8] * m2[2]  + mtx[12] * m2[3];
	m0[1]  = mtx[1] * m2[0]	 + mtx[5] * m2[1]  +  mtx[9] * m2[2]  + mtx[13] * m2[3];
	m0[2]  = mtx[2] * m2[0]	 + mtx[6] * m2[1]  + mtx[10] * m2[2]  + mtx[14] * m2[3];
	m0[3]  = mtx[3] * m2[0]	 + mtx[7] * m2[1]  + mtx[11] * m2[2]  + mtx[15] * m2[3];

	m0[4]  = mtx[0] * m2[4]	 + mtx[4] * m2[5]  +  mtx[8] * m2[6]  + mtx[12] * m2[7];
	m0[5]  = mtx[1] * m2[4]	 + mtx[5] * m2[5]  +  mtx[9] * m2[6]  + mtx[13] * m2[7];
	m0[6]  = mtx[2] * m2[4]	 + mtx[6] * m2[5]  + mtx[10] * m2[6]  + mtx[14] * m2[7];
	m0[7]  = mtx[3] * m2[4]	 + mtx[7] * m2[5]  + mtx[11] * m2[6]  + mtx[15] * m2[7];

	m0[8]  = mtx[0] * m2[8]	 + mtx[4] * m2[9]  +  mtx[8] * m2[10] + mtx[12] * m2[11];
	m0[9]  = mtx[1] * m2[8]	 + mtx[5] * m2[9]  +  mtx[9] * m2[10] + mtx[13] * m2[11];
	m0[10] = mtx[2] * m2[8]	 + mtx[6] * m2[9]  + mtx[10] * m2[10] + mtx[14] * m2[11];
	m0[11] = mtx[3] * m2[8]	 + mtx[7] * m2[9]  + mtx[11] * m2[10] + mtx[15] * m2[11];

	m0[12] = mtx[0] * m2[12] + mtx[4] * m2[13] +  mtx[8] * m2[14] + mtx[12] * m2[15];
	m0[13] = mtx[1] * m2[12] + mtx[5] * m2[13] +  mtx[9] * m2[14] + mtx[13] * m2[15];
	m0[14] = mtx[2] * m2[12] + mtx[6] * m2[13] + mtx[10] * m2[14] + mtx[14] * m2[15];
	m0[15] = mtx[3] * m2[12] + mtx[7] * m2[13] + mtx[11] * m2[14] + mtx[15] * m2[15];

	memcpy(mtx, m0, sizeof(m0));
}

// Essentially, multiply by an identity matrix with translation components set.  This means
//	that a lot of the zero compnents can go away! and the 1.0 ones are greatly simplified.
//
void gxMatrix::translateBy(float xlX, float xlY, float xlZ)
{
	mtx[12] += mtx[0] * xlX + mtx[4] * xlY +  mtx[8] * xlZ;
	mtx[13] += mtx[1] * xlX + mtx[5] * xlY +  mtx[9] * xlZ;
	mtx[14] += mtx[2] * xlX + mtx[6] * xlY + mtx[10] * xlZ;
	mtx[15] += mtx[3] * xlX + mtx[7] * xlY + mtx[11] * xlZ;
}

/*static inline void Matrix3DSetIdentity(Matrix3D matrix)
{
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0.0;
	matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0;
	matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0;
}
static inline void Matrix3DSetTranslation(Matrix3D matrix, float xTranslate, float yTranslate, float zTranslate)
{
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0.0;
	matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0;
	matrix[11] = 0.0;
	matrix[12] = xTranslate;
	matrix[13] = yTranslate;
	matrix[14] = zTranslate;
}
static inline void Matrix3DSetScaling(Matrix3D matrix, float xScale, float yScale, float zScale)
{
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0.0;
	matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0;
	matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0;
	matrix[0] = xScale;
	matrix[5] = yScale;
	matrix[10] = zScale;
	matrix[15] = 1.0;
}
static inline void Matrix3DSetUniformScaling(Matrix3D matrix, float scale)
{
	Matrix3DSetScaling(matrix, scale, scale, scale);
}
static inline void Matrix3DSetXRotationUsingRadians(Matrix3D matrix, float degrees)
{
	matrix[0] = matrix[15] = 1.0;
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0.0;
	matrix[7] = matrix[8] = 0.0;
	matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0;

	matrix[5] = cosf(degrees);
	matrix[6] = -fastSinf(degrees);
	matrix[9] = -matrix[6];
	matrix[10] = matrix[5];
}
static inline void Matrix3DSetXRotationUsingDegrees(Matrix3D matrix, float degrees)
{
	Matrix3DSetXRotationUsingRadians(matrix, degrees * M_PI / 180.0);
}
static inline void Matrix3DSetYRotationUsingRadians(Matrix3D matrix, float degrees)
{
	matrix[0] = cosf(degrees);
	matrix[2] = fastSinf(degrees);
	matrix[8] = -matrix[2];
	matrix[10] = matrix[0];
	matrix[1] = matrix[3] = matrix[4] = matrix[6] = matrix[7] = 0.0;
	matrix[9] = matrix[11] = matrix[13] = matrix[12] = matrix[14] = 0.0;
	matrix[5] = matrix[15] = 1.0;
}
static inline void Matrix3DSetYRotationUsingDegrees(Matrix3D matrix, float degrees)
{
	Matrix3DSetYRotationUsingRadians(matrix, degrees * M_PI / 180.0);
}
static inline void Matrix3DSetZRotationUsingRadians(Matrix3D matrix, float degrees)
{
	matrix[0] = cosf(degrees);
	matrix[1] = fastSinf(degrees);
	matrix[4] = -matrix[1];
	matrix[5] = matrix[0];
	matrix[2] = matrix[3] = matrix[6] = matrix[7] = matrix[8] = 0.0;
	matrix[9] = matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0;
	matrix[10] = matrix[15] = 1.0;
}
static inline void Matrix3DSetZRotationUsingDegrees(Matrix3D matrix, float degrees)
{
	Matrix3DSetZRotationUsingRadians(matrix, degrees * M_PI / 180.0);
}
static inline void Matrix3DSetRotationByRadians(Matrix3D matrix, float angle, float x, float y, float z)
{
	float mag = sqrtf((x*x) + (y*y) + (z*z));
	if (mag == 0.0)
	{
		x = 1.0;
		y = 0.0;
		z = 0.0;
	}
	else if (mag != 1.0)
	{
		x /= mag;
		y /= mag;
		z /= mag;
	}

	float c = cosf(angle);
	float s = fastSinf(angle);
	matrix[3] = matrix[7] = matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0;
	matrix[15] = 1.0;


	matrix[0] = (x*x)*(1 - c) + c;
	matrix[1] = (y*x)*(1 - c) + (z*s);
	matrix[2] = (x*z)*(1 - c) - (y*s);
	matrix[4] = (x*y)*(1 - c) - (z*s);
	matrix[5] = (y*y)*(1 - c) + c;
	matrix[6] = (y*z)*(1 - c) + (x*s);
	matrix[8] = (x*z)*(1 - c) + (y*s);
	matrix[9] = (y*z)*(1 - c) - (x*s);
	matrix[10] = (z*z)*(1 - c) + c;

}
static inline void Matrix3DSetRotationByDegrees(Matrix3D matrix, float angle, float x, float y, float z)
{
	Matrix3DSetRotationByRadians(matrix, angle * M_PI / 180.0, x, y, z);
}
static inline void Matrix3DSetShear(Matrix3D matrix, float xShear, float yShear)
{
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
	matrix[1] = matrix[2] = matrix[3] = 0.0;
	matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0;
	matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0;
	matrix[1] = xShear;
	matrix[4] = yShear;
}*/

