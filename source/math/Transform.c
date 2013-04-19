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
*
*	Copyright 2013 Chris Foster
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

void X3D_InvertTransform(X3D_Transform *Transform, X3D_Transform *Result)
{
	Result->Translation = (X3D_Vec3){-Transform->Translation.x, -Transform->Translation.y, -Transform->Translation.z};

	X3D_TransposeMatrix(&Transform->Rotation, &Result->Rotation);

	float ScaleBase = pow(2.0f, (float)Transform->ScaleBitShift);
	float Scale = (float)Transform->Scale.x / ScaleBase;
	Result->Scale.x = (short)((1.0f / Scale) * ScaleBase);
	Scale = (float)Transform->Scale.y / ScaleBase;
	Result->Scale.y = (short)((1.0f / Scale) * ScaleBase);
	Scale = (float)Transform->Scale.z / ScaleBase;
	Result->Scale.z = (short)((1.0f / Scale) * ScaleBase);
}

void X3D_TransformVec3(X3D_Transform *Transform, X3D_Vec3 *Vector, X3D_Vec3 *Result)
{
	X3D_Vec3 Temp;

	X3D_MultiplyVec3Vec3Shift(Vector, &Transform->Scale, Transform->ScaleBitShift, &Temp);

	X3D_MultiplyMatrixVec3(&Transform->Rotation, &Temp, &Temp);

	X3D_AddVec3Vec3(&Temp, &Transform->Translation, Result);
}

void X3D_InvertedTransformVec3(X3D_Transform *InvertedTransform, X3D_Vec3 *Vector, X3D_Vec3 *Result)
{
	X3D_Vec3 Temp;

	X3D_AddVec3Vec3(Vector, &InvertedTransform->Translation, &Temp);

	X3D_MultiplyMatrixVec3(&InvertedTransform->Rotation, &Temp, &Temp);

	X3D_MultiplyVec3Vec3Shift(&Temp, &InvertedTransform->Scale, InvertedTransform->ScaleBitShift, Result);
}

void X3D_TransformVec3Array(X3D_Transform *Transform, unsigned short Count, X3D_Vec3 *Source, X3D_Vec3 *Destination)
{
	unsigned short a;
	for (a = 0; a < Count; a++)
		X3D_TransformVec3(Transform, &Source[a], &Destination[a]);
}

void X3D_InvertedTransformVec3Array(X3D_Transform *InvertedTransform, unsigned short Count, X3D_Vec3 *Source, X3D_Vec3 *Destination)
{
	unsigned short a;
	for (a = 0; a < Count; a++)
		X3D_InvertedTransformVec3(InvertedTransform, &Source[a], &Destination[a]);
}
