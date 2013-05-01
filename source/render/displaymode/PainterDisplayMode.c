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

#include "PainterDisplayMode.h"

X3D_RESULT Painter_Initialize()
{
	return X3D_SUCCESS;
}

void ZSortPolygons(unsigned short PolygonCount, long *AverageZDepths, unsigned short *SortedIndices);

X3D_RESULT Painter_Draw(X3D_Vertices *Vertices, X3D_Polygons *Polygons)
{
	long AverageZDepths[Polygons->PolygonCount];
	unsigned short SortedIndices[Polygons->PolygonCount];

	unsigned short a;
	for (a = 0; a < Polygons->PolygonCount; a++)
	{
		X3D_Polygon *Polygon = &Polygons->Polygons[a];

		AverageZDepths[a] = 0;

		unsigned short b;
		for (b = 0; b < Polygon->VertexCount; b++)
			AverageZDepths[a] += Vertices->Vertices[Polygon->Vertices[b]].z;

		AverageZDepths[a] = ((AverageZDepths[a] << 11) / Polygon->VertexCount) >> 11;
		SortedIndices[a] = a;
	}

	ZSortPolygons(Polygons->PolygonCount, AverageZDepths, SortedIndices);

	if (EngineState.General.ColorMode == X3D_COLOR_GRAYSCALE)
	{
		for (a = 0; a < Polygons->PolygonCount; a++)
		{
			X3D_Polygon *Polygon = &Polygons->Polygons[SortedIndices[a]];

			X3D_Vec2 Points[Polygon->VertexCount];
			unsigned short b;
			for (b = 0; b < Polygon->VertexCount; b++)
				Points[b] = *(X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[b]];

			X3D_FillGrayConvexPolygon(EngineState.General.Plane1, EngineState.General.Plane2, Polygon->VertexCount, Points, Polygon->Color);

			if (EngineState.General.OutlineMode != X3D_OUTLINE_NONE)
			{
				X3D_COLOR Color = X3D_COLORS_BLACK;

				if (EngineState.General.OutlineMode == X3D_OUTLINE_OPPOSITE)
					Color = X3D_COLORS_BLACK - Polygon->Color;

				unsigned char c;
				for (c = 0; c < Polygon->VertexCount - 1; c++)
				{
					X3D_Vec2 *PointA = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[c]];
					X3D_Vec2 *PointB = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[c + 1]];

					X3D_DrawGrayLine_Clipped(EngineState.General.Plane1, EngineState.General.Plane2, PointA->x, PointA->y, PointB->x, PointB->y, Color);
				}
				
				X3D_Vec2 *PointA = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[Polygon->VertexCount - 1]];
				X3D_Vec2 *PointB = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[0]];

				X3D_DrawGrayLine_Clipped(EngineState.General.Plane1, EngineState.General.Plane2, PointA->x, PointA->y, PointB->x, PointB->y, Color);
			}
		}
	}
	else
	{
		for (a = 0; a < Polygons->PolygonCount; a++)
		{
			X3D_Polygon *Polygon = &Polygons->Polygons[SortedIndices[a]];

			X3D_Vec2 Points[Polygon->VertexCount];
			unsigned short b;
			for (b = 0; b < Polygon->VertexCount; b++)
				Points[b] = *(X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[b]];

			X3D_FillConvexPolygon(EngineState.General.Plane1, Polygon->VertexCount, Points, Polygon->Color);

			if (EngineState.General.OutlineMode != X3D_OUTLINE_NONE)
			{
				X3D_COLOR Color = X3D_COLORS_BLACK;

				if (EngineState.General.OutlineMode == X3D_OUTLINE_OPPOSITE)
					Color = X3D_COLORS_BLACK - Polygon->Color;

				unsigned char c;
				for (c = 0; c < Polygon->VertexCount - 1; c++)
				{
					X3D_Vec2 *PointA = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[c]];
					X3D_Vec2 *PointB = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[c + 1]];

					X3D_DrawLine_Clipped(EngineState.General.Plane1, PointA->x, PointA->y, PointB->x, PointB->y, Color);
				}
				
				X3D_Vec2 *PointA = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[Polygon->VertexCount - 1]];
				X3D_Vec2 *PointB = (X3D_Vec2 *)&Vertices->Vertices[Polygon->Vertices[0]];

				X3D_DrawLine_Clipped(EngineState.General.Plane1, PointA->x, PointA->y, PointB->x, PointB->y, Color);
			}
		}
	}
	
	return X3D_SUCCESS;
}

void BubbleSortPolygons(unsigned short PolygonCount, long *AverageZDepths, unsigned short *SortedIndices);

void ZSortPolygons(unsigned short PolygonCount, long *AverageZDepths, unsigned short *SortedIndices)
{
	if (PolygonCount <= 1)
		return;

	BubbleSortPolygons(PolygonCount, AverageZDepths, SortedIndices);
}

void BubbleSortPolygons(unsigned short PolygonCount, long *AverageZDepths, unsigned short *SortedIndices)
{
	unsigned char SwapOccurred;
	unsigned short LastIndex = PolygonCount - 1;

	do {
		SwapOccurred = 0;

		unsigned short a;
		for (a = 0; a < LastIndex; a++)
		{
			if (AverageZDepths[a] < AverageZDepths[a + 1])
			{
				SwapOccurred = 1;

				X3D_Switch(AverageZDepths[a], AverageZDepths[a + 1]);
				X3D_Switch(SortedIndices[a], SortedIndices[a + 1]);
			}
		}

		LastIndex--;
	} while (SwapOccurred && LastIndex > 0);
}

X3D_RESULT Painter_Cleanup()
{
	return X3D_SUCCESS;
}

X3D_RESULT Painter_Terminate()
{
	return X3D_SUCCESS;
}
