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

#include "PerspectiveProjectionMode.h"

X3D_RESULT Perspective_ProjectVertices(X3D_Vertices *Vertices, X3D_Vertices *ResultVertices)
{
	if (Vertices != ResultVertices && ResultVertices->Vertices == NULL)
	{
		ResultVertices->VertexCount = Vertices->VertexCount;
		if (!(ResultVertices->Vertices = malloc(ResultVertices->VertexCount * sizeof(X3D_Vertex))))
			return X3D_MEMORYERROR;
	}
	
	short TanH = X3D_Tan(RenderCamera.User.HorizontalFOV / 2);
	short TanV = X3D_Tan(RenderCamera.User.VerticalFOV / 2);

	unsigned short Vertex;
	for (Vertex = 0; Vertex < Vertices->VertexCount; Vertex++)
	{
		if (Vertices->Vertices[Vertex].z != 0)
		{
			ResultVertices->Vertices[Vertex].x = X3D_SCREEN_CENTER_X + ((long)Vertices->Vertices[Vertex].x * X3D_SCREEN_SCALE_X) / (((long)Vertices->Vertices[Vertex].z * TanH) >> 9);
			ResultVertices->Vertices[Vertex].y = X3D_SCREEN_CENTER_Y - ((long)Vertices->Vertices[Vertex].y * X3D_SCREEN_SCALE_Y) / (((long)Vertices->Vertices[Vertex].z * TanV) >> 9);
			ResultVertices->Vertices[Vertex].z = Vertices->Vertices[Vertex].z;
		}
		else
		{
			ResultVertices->Vertices[Vertex].x = Vertices->Vertices[Vertex].x;
			ResultVertices->Vertices[Vertex].y = Vertices->Vertices[Vertex].y;
			ResultVertices->Vertices[Vertex].z = 0;
		}

	}

	return X3D_SUCCESS;
}
