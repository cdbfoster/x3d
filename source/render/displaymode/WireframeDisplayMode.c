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
#include "../../api/external/X3D_Render.h"
#include "../../api/external/X3D_Draw.h"
#include "../../api/internal/Render.h"
#include "../../api/internal/System.h"

#include "WireframeDisplayMode.h"

X3D_RESULT Wireframe_Initialize()
{
	return X3D_SUCCESS;
}

X3D_RESULT Wireframe_Draw(X3D_Vertices *Vertices, X3D_Polygons *Polygons)
{
	if (EngineState.General.ColorMode == X3D_COLOR_GRAYSCALE)
	{
		unsigned short a;
		for (a = 0; a < Polygons->PolygonCount; a++)
		{
			X3D_Polygon *Polygon = &Polygons->Polygons[a];
			
			unsigned char b;
			for (b = 0; b < Polygon->VertexCount - 1; b++)
			{
				X3D_Vec2 *PointA = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[b]];
				X3D_Vec2 *PointB = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[b + 1]];

				X3D_DrawGrayLine_Clipped(EngineState.General.Plane1, EngineState.General.Plane2, PointA->x, PointA->y, PointB->x, PointB->y, Polygon->Color);
			}
			
			X3D_Vec2 *PointA = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[Polygon->VertexCount - 1]];
			X3D_Vec2 *PointB = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[0]];

			X3D_DrawGrayLine_Clipped(EngineState.General.Plane1, EngineState.General.Plane2, PointA->x, PointA->y, PointB->x, PointB->y, Polygon->Color);
		}
	}
	else
	{
		unsigned short a;
		for (a = 0; a < Polygons->PolygonCount; a++)
		{
			X3D_Polygon *Polygon = &Polygons->Polygons[a];
			
			unsigned char b;
			for (b = 0; b < Polygon->VertexCount - 1; b++)
			{
				X3D_Vec2 *PointA = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[b]];
				X3D_Vec2 *PointB = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[b + 1]];

				X3D_DrawLine_Clipped(EngineState.General.Plane1, PointA->x, PointA->y, PointB->x, PointB->y, Polygon->Color);
			}
			
			X3D_Vec2 *PointA = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[Polygon->VertexCount - 1]];
			X3D_Vec2 *PointB = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[0]];

			X3D_DrawLine_Clipped(EngineState.General.Plane1, PointA->x, PointA->y, PointB->x, PointB->y, Polygon->Color);
		}
	}
	return X3D_SUCCESS;
}

X3D_RESULT Wireframe_Cleanup()
{
	return X3D_SUCCESS;
}

X3D_RESULT Wireframe_Terminate()
{
	return X3D_SUCCESS;
}
