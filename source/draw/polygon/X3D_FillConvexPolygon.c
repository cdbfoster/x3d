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

#include "../../X3D.h"

void X3D_FillConvexPolygon(unsigned char *Plane, unsigned char PointCount, X3D_Vec2 *Points, unsigned char Color)
{
	void (*DrawHLine)(short *Address asm("%a0"), short x1 asm("%d0"), short x2 asm("%d1"));

	if (Color >= X3D_COLORS_DGRAY)
		DrawHLine = X3D_DrawHLineBLACK;
	else
		DrawHLine = X3D_DrawHLineWHITE;

	short a, b;
	short StartIndex = 0, YMin = Points[0].y, YMax = YMin;

	// Determine the highest and lowest point, putting the index of the highest point in StartIndex
	for (a = 1; a < PointCount; a++)
	{
		if (Points[a].y < YMin)
		{
			YMin = Points[a].y;
			StartIndex = a;
		}
		if (Points[a].y > YMax)
			YMax = Points[a].y;
	}

	// Trivially reject if possible
	if (YMax < 0 || YMin > X3D_SCREEN_HEIGHT)
		return;

	X3D_Vec2 Reordered[PointCount + 1];

	// Reordered will contain all of the points in the polygon, but shifted around so that the highest point is first
	for (a = 0; a < PointCount; a++)
		Reordered[a] = Points[X3D_Wrap(a + StartIndex, PointCount)];
	// The last point in Reordered is the same as the first.  Redundant for efficiency.
	Reordered[PointCount] = Reordered[0];

	short Slopes[PointCount];
	short dy;

	// Slopes[a] equals the slope of the line segment between Reordered[a] and Reordered[a + 1]
	for (a = 0; a < PointCount; a++)
	{
		dy = Reordered[a + 1].y - Reordered[a].y;
		if (dy)
			Slopes[a] = ((Reordered[a + 1].x - Reordered[a].x) << 7) / dy;
		else
			Slopes[a] = X3D_UNDEFINED;
	}

	short YPos = Reordered[0].y, YEnd;
	short x1 = Reordered[0].x << 7, x2 = x1;
	short Slope1, Slope2;
	unsigned char *Address = Plane + (YPos << 5) - (YPos << 1);

	a = 0; b = PointCount - 1;
	do
	{
		Slope1 = Slopes[a];
		Slope2 = Slopes[b];
		if (Slope1 == X3D_UNDEFINED)
			x1 = Reordered[++a].x << 7;
		else if (Slope2 == X3D_UNDEFINED)
			x2 = Reordered[b--].x << 7;
		else
		{
			if (Reordered[a + 1].y > Reordered[b].y)
				YEnd = Reordered[b--].y;
			else
				YEnd = Reordered[++a].y;

			for (;YPos < YEnd; YPos++, x1 += Slope1, x2 += Slope2, Address += X3D_SCREEN_BYTESPERLINE)
				if (YPos >= 0 && YPos <= X3D_SCREEN_HEIGHT)
						DrawHLine((short *)Address, x1 >> 7, x2 >> 7);
		}
	} while(a < b);

	// Draw the last line
	if (YPos <= X3D_SCREEN_HEIGHT)
		DrawHLine((short *)Address, x1 >> 7, x2 >> 7);
}
