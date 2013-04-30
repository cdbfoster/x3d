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
#include "../api/external/X3D_System.h"

#include "RenderFunctions.h"
#include "BackfaceCullMode.h"

X3D_RESULT Cull(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons);
X3D_RESULT NoCull(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons);

X3D_RESULT InitializeBackfaceCullMode(X3D_Parameters *Parameters)
{
	switch (Parameters->BackfaceCullMode)
	{
	case X3D_BACKFACECULL_OFF:
		Render.BackfaceCullMode_CullPolygons = NoCull;
		break;
	case X3D_BACKFACECULL_ON:
		Render.BackfaceCullMode_CullPolygons = Cull;
		break;
	default:
		return X3D_FAILURE;
	}

	return X3D_SUCCESS;
}

X3D_RESULT Cull(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons)
{
	unsigned short OkayCount = 0;
	unsigned char Okay[Polygons->PolygonCount];

	unsigned short a;
	for (a = 0; a < Polygons->PolygonCount; a++)
	{
		X3D_Vec2 *v0 = (X3D_Vec2 *)&Vertices->Vertices[Polygons->Polygons[a].Vertices[0]];
		X3D_Vec2 *v1 = (X3D_Vec2 *)&Vertices->Vertices[Polygons->Polygons[a].Vertices[1]];
		X3D_Vec2 *v2 = (X3D_Vec2 *)&Vertices->Vertices[Polygons->Polygons[a].Vertices[2]];

		X3D_Vec2 e0 = (X3D_Vec2){v0->x - v1->x, v0->y - v1->y};
		X3D_Vec2 e1 = (X3D_Vec2){v2->x - v1->x, v2->y - v1->y};
		Okay[a] = ((e0.x * e1.y < e0.y * e1.x) ? 1 : 0);

		if (Okay[a] == 1)
			OkayCount++;
	}

	if (OkayCount == 0)
	{
		ResultPolygons->PolygonCount = 0;
		return X3D_SUCCESS;
	}

	if (ResultPolygons->Polygons == NULL)
	{
		if (!(ResultPolygons->Polygons = malloc(OkayCount * sizeof(X3D_Polygon))))
			return X3D_MEMORYERROR;
	}
	else
	{
		if (ResultPolygons->PolygonCount != OkayCount)
			if (!(ResultPolygons->Polygons = realloc(ResultPolygons->Polygons, OkayCount * sizeof(X3D_Polygon))))
				return X3D_MEMORYERROR;
	}

	ResultPolygons->PolygonCount = OkayCount;

	unsigned short b = 0;
	for (a = 0; a < Polygons->PolygonCount; a++)
	{
		if (Okay[a])
			ResultPolygons->Polygons[b++] = Polygons->Polygons[a];
	}

	return X3D_SUCCESS;
}

X3D_RESULT NoCull(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons)
{
	if (ResultPolygons->Polygons == NULL)
	{
		if (!(ResultPolygons->Polygons = malloc(Polygons->PolygonCount * sizeof(X3D_Polygon))))
			return X3D_MEMORYERROR;
	}
	else
	{
		if (ResultPolygons->PolygonCount != Polygons->PolygonCount)
			if (!(ResultPolygons->Polygons = realloc(ResultPolygons->Polygons, Polygons->PolygonCount * sizeof(X3D_Polygon))))
				return X3D_MEMORYERROR;
	}

	memcpy(ResultPolygons->Polygons, Polygons->Polygons, Polygons->PolygonCount * sizeof(X3D_Polygon));
	ResultPolygons->PolygonCount = Polygons->PolygonCount;

	return X3D_SUCCESS;
}
