/*
*	This file is part of X3D Engine.
*
*	X3D Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU Lesser General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	X3D Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU Lesser General Public License for more details.
*
*	You should have received a copy of the GNU Lesser General Public License
*	along with X3D Engine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef X3D_MATH
#define X3D_MATH

typedef struct {
	short x, y;
} X3D_Vec2;

typedef struct {
	short x, y, z;
} X3D_Vec3;

typedef struct {
	long _11, _12, _13;
	long _21, _22, _23;
	long _31, _32, _33;
	long _11_12, _21_22, _31_32;
} X3D_Matrix;

typedef struct {
	X3D_Vec3 Normal;
	long PlaneConstant;
} X3D_Plane;

typedef struct {
	X3D_Vec3 Translation;
	X3D_Matrix Rotation;
	X3D_Vec3 Scale;
	unsigned char ScaleBitShift;
} X3D_Transform;

#define X3D_Abs(x)					(((x) < 0) ? -(x) : (x))
#define X3D_Sign(x)					(((x) < 0) ? -1 : 1)
#define X3D_WrapAngle(x)			((x) & 0x01FF)

extern const short X3D_SinTable[512];
extern const short X3D_TanTable[512];

#define X3D_ANGLE	short
#define X3D_Sin(x)	(X3D_SinTable[X3D_WrapAngle(x)])
#define X3D_Cos(x)	(X3D_SinTable[X3D_WrapAngle((x) + 128)])
#define X3D_Tan(x)	(X3D_TanTable[X3D_WrapAngle(x)])

// Vector Operations
inline void X3D_AddVec3Vec3(X3D_Vec3 *VectorA, X3D_Vec3 *VectorB, X3D_Vec3 *Result);
inline void X3D_SubtractVec3Vec3(X3D_Vec3 *VectorA, X3D_Vec3 *VectorB, X3D_Vec3 *Result);
inline void X3D_MultiplyVec3Vec3(X3D_Vec3 *VectorA, X3D_Vec3 *VectorB, X3D_Vec3 *Result);

// Matrix Operations
inline void X3D_MultiplyMatrixVec3(X3D_Matrix *Matrix, X3D_Vec3 *Vector, X3D_Vec3 *Result);
inline void X3D_MultiplyMatrixMatrix(X3D_Matrix *MatrixA, X3D_Matrix *MatrixB, X3D_Matrix *Result);
inline void X3D_RotationMatrixXYZ(X3D_Matrix *Matrix, X3D_ANGLE AngleX, X3D_ANGLE AngleY, X3D_ANGLE AngleZ);
inline void X3D_TransposeMatrix(X3D_Matrix *Matrix, X3D_Matrix *Result);
// Add some other matrix operations

// Transform Operations
inline void X3D_CreateTransform(X3D_Transform *Transform,
								short TranslationX, short TranslationY, short TranslationZ,
								X3D_ANGLE AngleX, X3D_ANGLE AngleY, X3D_ANGLE AngleZ,
								float ScaleX, float ScaleY, float ScaleZ);
// Source and Destination may point to the same location
inline void X3D_TransformVec3(X3D_Vec3 *Vector, X3D_Vec3 *Result, X3D_Transform *Transform);
inline void X3D_TransformVertices(unsigned short VertexCount, X3D_Vec3 *Source, X3D_Vec3 *Destination, X3D_Transform *Transform);

#endif
