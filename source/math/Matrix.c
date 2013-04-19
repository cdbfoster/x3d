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

void X3D_MultiplyMatrixVec3(X3D_Matrix *Matrix, X3D_Vec3 *Vector, X3D_Vec3 *Result)
{
	X3D_Vec3 Temp;

	long XY = (long)Vector->x * Vector->y;
	Temp.x = ((Matrix->_11 + Vector->y) * (Matrix->_12 + Vector->x) - (Matrix->_11_12 + XY) + (Matrix->_13 * Vector->z)) >> 8;
	Temp.y = ((Matrix->_21 + Vector->y) * (Matrix->_22 + Vector->x) - (Matrix->_21_22 + XY) + (Matrix->_23 * Vector->z)) >> 8;
	Temp.z = ((Matrix->_31 + Vector->y) * (Matrix->_32 + Vector->x) - (Matrix->_31_32 + XY) + (Matrix->_33 * Vector->z)) >> 8;

	*Result = Temp;
}

void X3D_MultiplyMatrixMatrix(X3D_Matrix *MatrixA, X3D_Matrix *MatrixB, X3D_Matrix *Result)
{
	// TODO
}

void X3D_RotationMatrixXYZ(X3D_Matrix *Matrix, X3D_ANGLE AngleX, X3D_ANGLE AngleY, X3D_ANGLE AngleZ)
{
	long SinX = X3D_Sin(AngleX), CosX = X3D_Cos(AngleX);
	long SinY = X3D_Sin(AngleY), CosY = X3D_Cos(AngleY);
	long SinZ = X3D_Sin(AngleZ), CosZ = X3D_Cos(AngleZ);

	long SinXSinY = SinX * SinY;
	long CosXSinY = CosX * SinY;

	Matrix->_11 = (CosY * CosZ) >> 8;
	Matrix->_12 = ((SinXSinY * CosZ) - ((CosX * SinZ) << 8)) >> 16;
	Matrix->_13 = ((CosXSinY * CosZ) + ((SinX * SinZ) << 8)) >> 16;
	Matrix->_21 = (CosY * SinZ) >> 8;
	Matrix->_22 = ((SinXSinY * SinZ) + ((CosX * CosZ) << 8)) >> 16;
	Matrix->_23 = ((CosXSinY * SinZ) - ((SinX * CosZ) << 8)) >> 16;
	Matrix->_31 = -SinY;
	Matrix->_32 = (SinX * CosY) >> 8;
	Matrix->_33 = (CosX * CosY) >> 8;

	Matrix->_11_12 = Matrix->_11 * Matrix->_12;
	Matrix->_21_22 = Matrix->_21 * Matrix->_22;
	Matrix->_31_32 = Matrix->_31 * Matrix->_32;
}

void X3D_TransposeMatrix(X3D_Matrix *Matrix, X3D_Matrix *Result)
{
	X3D_Matrix Temp;

	Temp._11 = Matrix->_11;
	Temp._12 = Matrix->_21;
	Temp._13 = Matrix->_31;
	Temp._21 = Matrix->_12;
	Temp._22 = Matrix->_22;
	Temp._23 = Matrix->_32;
	Temp._31 = Matrix->_13;
	Temp._32 = Matrix->_23;
	Temp._33 = Matrix->_33;

	Temp._11_12 = Temp._11 * Temp._12;
	Temp._21_22 = Temp._21 * Temp._22;
	Temp._31_32 = Temp._31 * Temp._32;

	*Result = Temp;
}
