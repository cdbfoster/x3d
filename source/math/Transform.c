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

#define _GENERIC_ARCHIVE

#include <tigcclib.h>
#include "../api/external/X3D_Math.h"

void X3D_CreateTransform(X3D_Transform *Transform,
						 short TranslationX, short TranslationY, short TranslationZ,
						 X3D_ANGLE AngleX, X3D_ANGLE AngleY, X3D_ANGLE AngleZ,
						 float ScaleX, float ScaleY, float ScaleZ)
{
	Transform->Translation = (X3D_Vec3){TranslationX, TranslationY, TranslationZ};

	X3D_RotationMatrixXYZ(&Transform->Rotation, AngleX, AngleY, AngleZ);

	Transform->Scale = (X3D_Vec3){(short)(ScaleX * 256), (short)(ScaleY * 256), (short)(ScaleZ * 256)};
	Transform->ScaleBitShift = 8;
}

void X3D_TransformVector(X3D_Vec3 *Vector, X3D_Vec3 *Result, X3D_Transform *Transform)
{
	X3D_Vec3 Temp;

	X3D_MultiplyVec3Vec3(Vector, &Transform->Scale, &Temp);
	unsigned char Shift = Transform->ScaleBitShift;
	Temp.x >>= Shift;
	Temp.y >>= Shift;
	Temp.z >>= Shift;

	X3D_MultiplyMatrixVec3(&Transform->Rotation, &Temp, &Temp);

	X3D_AddVec3Vec3(&Temp, &Transform->Translation, Result);
}

void X3D_TransformVertices(unsigned short VertexCount, X3D_Vec3 *Source, X3D_Vec3 *Destination, X3D_Transform *Transform)
{
	unsigned short a;
	for (a = 0; a < VertexCount; a++)
		X3D_TransformVec3(&Source[a], &Destination[a], Transform);
}
