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

#include "../api/external/X3D_Utility.h"
#include "../api/external/X3D_Math.h"
#include "../api/external/X3D_Render.h"

#include "RenderCamera.h"

RenderCameraType RenderCamera;

X3D_RESULT X3D_UpdateRenderCamera(X3D_Camera *Camera)
{
	if (Camera == NULL)
		return X3D_FAILURE;

	memcpy(&RenderCamera.User, Camera, sizeof(X3D_Camera));

	X3D_Matrix *InverseViewMatrix = &Camera->Transform.Rotation;

	X3D_TransposeMatrix(InverseViewMatrix, &RenderCamera.ViewMatrix);

	unsigned char AngleH = RenderCamera.User.HorizontalFOV / 2;
	unsigned char AngleV = RenderCamera.User.VerticalFOV / 2;

	long SinH = X3D_Sin(AngleH), CosH = X3D_Cos(AngleH);
	long SinV = X3D_Sin(AngleV), CosV = X3D_Cos(AngleV);

	X3D_Plane *ViewFrustum = RenderCamera.ViewFrustum;
	X3D_Vec3 PlaneNormal[6];

	PlaneNormal[X3D_FRUSTUM_NEAR].x = 0;
	PlaneNormal[X3D_FRUSTUM_NEAR].y = 0;
	PlaneNormal[X3D_FRUSTUM_NEAR].z = 256;
	ViewFrustum[X3D_FRUSTUM_NEAR].PlaneConstant = RenderCamera.User.NearClip << 8;

	PlaneNormal[X3D_FRUSTUM_FAR].x = 0;
	PlaneNormal[X3D_FRUSTUM_FAR].y = 0;
	PlaneNormal[X3D_FRUSTUM_FAR].z = -256;
	ViewFrustum[X3D_FRUSTUM_FAR].PlaneConstant = RenderCamera.User.FarClip << 8;

	PlaneNormal[X3D_FRUSTUM_LEFT].x = CosH;
	PlaneNormal[X3D_FRUSTUM_LEFT].y = 0;
	PlaneNormal[X3D_FRUSTUM_LEFT].z = SinH;
	ViewFrustum[X3D_FRUSTUM_LEFT].PlaneConstant = 0;

	PlaneNormal[X3D_FRUSTUM_RIGHT].x = -CosH;
	PlaneNormal[X3D_FRUSTUM_RIGHT].y = 0;
	PlaneNormal[X3D_FRUSTUM_RIGHT].z = SinH;
	ViewFrustum[X3D_FRUSTUM_RIGHT].PlaneConstant = 0;

	PlaneNormal[X3D_FRUSTUM_TOP].x = 0;
	PlaneNormal[X3D_FRUSTUM_TOP].y = -CosV;
	PlaneNormal[X3D_FRUSTUM_TOP].z = SinV;;
	ViewFrustum[X3D_FRUSTUM_TOP].PlaneConstant = 0;

	PlaneNormal[X3D_FRUSTUM_BOTTOM].x = 0;
	PlaneNormal[X3D_FRUSTUM_BOTTOM].y = CosV;
	PlaneNormal[X3D_FRUSTUM_BOTTOM].z = SinV;
	ViewFrustum[X3D_FRUSTUM_BOTTOM].PlaneConstant = 0;

	unsigned char Plane;
	for (Plane = 0; Plane < 6; Plane++)
	{
		X3D_MultiplyMatrixVec3(InverseViewMatrix, &PlaneNormal[Plane], &ViewFrustum[Plane].Normal);

		X3D_Vec3 *Translation = &RenderCamera.User.Transform.Translation;
		ViewFrustum[Plane].PlaneConstant += (long)(ViewFrustum[Plane].Normal.x) * Translation->x +
											(long)(ViewFrustum[Plane].Normal.y) * Translation->y +
											(long)(ViewFrustum[Plane].Normal.z) * Translation->z;
	}

	return X3D_SUCCESS;
}

X3D_RESULT FrustumCullPolygons(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Vertices *ResultVertices, X3D_Polygons *ResultPolygons)
{
	return X3D_SUCCESS;
}