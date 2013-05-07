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

#include "../../api/external/X3D_Utility.h"
#include "../../api/external/X3D_Math.h"
#include "../../api/external/X3D_Render.h"

#include "../RenderCamera.h"

#include "OrthographicProjectionMode.h"

X3D_RESULT Orthographic_GetViewFrustum(X3D_Plane *ViewFrustum)
{
	ViewFrustum[X3D_FRUSTUM_NEAR].Normal.x = 0;
	ViewFrustum[X3D_FRUSTUM_NEAR].Normal.y = 0;
	ViewFrustum[X3D_FRUSTUM_NEAR].Normal.z = 256;
	ViewFrustum[X3D_FRUSTUM_NEAR].PlaneConstant = (long)RenderCamera.User.NearClip << 8;

	ViewFrustum[X3D_FRUSTUM_FAR].Normal.x = 0;
	ViewFrustum[X3D_FRUSTUM_FAR].Normal.y = 0;
	ViewFrustum[X3D_FRUSTUM_FAR].Normal.z = -256;
	ViewFrustum[X3D_FRUSTUM_FAR].PlaneConstant = -((long)RenderCamera.User.FarClip << 8);

	ViewFrustum[X3D_FRUSTUM_LEFT].Normal.x = 256;
	ViewFrustum[X3D_FRUSTUM_LEFT].Normal.y = 0;
	ViewFrustum[X3D_FRUSTUM_LEFT].Normal.z = 0;
	ViewFrustum[X3D_FRUSTUM_LEFT].PlaneConstant = -((long)RenderCamera.User.HorizontalFOV << 7);

	ViewFrustum[X3D_FRUSTUM_RIGHT].Normal.x = -256;
	ViewFrustum[X3D_FRUSTUM_RIGHT].Normal.y = 0;
	ViewFrustum[X3D_FRUSTUM_RIGHT].Normal.z = 0;
	ViewFrustum[X3D_FRUSTUM_RIGHT].PlaneConstant = -((long)RenderCamera.User.HorizontalFOV << 7);

	ViewFrustum[X3D_FRUSTUM_TOP].Normal.x = 0;
	ViewFrustum[X3D_FRUSTUM_TOP].Normal.y = -256;
	ViewFrustum[X3D_FRUSTUM_TOP].Normal.z = 0;
	ViewFrustum[X3D_FRUSTUM_TOP].PlaneConstant = -((long)RenderCamera.User.VerticalFOV << 7);

	ViewFrustum[X3D_FRUSTUM_BOTTOM].Normal.x = 0;
	ViewFrustum[X3D_FRUSTUM_BOTTOM].Normal.y = 256;
	ViewFrustum[X3D_FRUSTUM_BOTTOM].Normal.z = 0;
	ViewFrustum[X3D_FRUSTUM_BOTTOM].PlaneConstant = -((long)RenderCamera.User.VerticalFOV << 7);

	return X3D_SUCCESS;
}

X3D_RESULT Orthographic_ProjectVertices(X3D_Vertices *Vertices, X3D_Vertices *ResultVertices)
{
	if (Vertices != ResultVertices && ResultVertices->Vertices == NULL)
	{
		ResultVertices->VertexCount = Vertices->VertexCount;
		if (!(ResultVertices->Vertices = malloc(ResultVertices->VertexCount * sizeof(X3D_Vertex))))
			return X3D_MEMORYERROR;
	}

	unsigned short XWidth = RenderCamera.User.HorizontalFOV >> 2;
	unsigned short YWidth = RenderCamera.User.VerticalFOV >> 2;

	unsigned short Vertex;
	for (Vertex = 0; Vertex < Vertices->VertexCount; Vertex++)
	{
		ResultVertices->Vertices[Vertex].x = X3D_SCREEN_CENTER_X + ((long)Vertices->Vertices[Vertex].x * X3D_SCREEN_SCALE_X) / XWidth;
		ResultVertices->Vertices[Vertex].y = X3D_SCREEN_CENTER_Y - ((long)Vertices->Vertices[Vertex].y * X3D_SCREEN_SCALE_Y) / YWidth;
		ResultVertices->Vertices[Vertex].z = Vertices->Vertices[Vertex].z;
	}

	return X3D_SUCCESS;
}
