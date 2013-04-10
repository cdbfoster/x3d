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

void X3D_AddVec3Vec3(X3D_Vec3 *VectorA, X3D_Vec3 *VectorB, X3D_Vec3 *Result)
{
	Result->x = VectorA->x + VectorB->x;
	Result->y = VectorA->y + VectorB->y;
	Result->z = VectorA->z + VectorB->z;
}

void X3D_SubtractVec3Vec3(X3D_Vec3 *VectorA, X3D_Vec3 *VectorB, X3D_Vec3 *Result)
{
	Result->x = VectorA->x - VectorB->x;
	Result->y = VectorA->y - VectorB->y;
	Result->z = VectorA->z - VectorB->z;
}

void X3D_MultiplyVec3Vec3(X3D_Vec3 *VectorA, X3D_Vec3 *VectorB, X3D_Vec3 *Result)
{
	Result->x = VectorA->x * VectorB->x;
	Result->y = VectorA->y * VectorB->y;
	Result->z = VectorA->z * VectorB->z;
}
