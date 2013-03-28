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
} Matrix;

typedef struct {
	X3D_Vec3 Normal;
	long PlaneConstant;
} GeometricPlane;

#define X3D_Abs(x)					(((x) < 0) ? -(x) : (x))
#define X3D_Sign(x)					(((x) < 0) ? -1 : 1)
#define X3D_WrapAngle(x)			((x) & 0x01FF)

extern const short X3D_SinTable[512];
extern const short X3D_TanTable[512];

#define X3D_Sin(x)	(X3D_SinTable[X3D_WrapAngle(x)])
#define X3D_Cos(x)	(X3D_SinTable[X3D_WrapAngle((x) + 128)])
#define X3D_Tan(x)	(X3D_TanTable[X3D_WrapAngle(x)])

#endif
