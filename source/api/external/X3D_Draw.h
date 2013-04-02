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

#ifndef X3D_DRAW
#define X3D_DRAW

#include "X3D_Utility.h"
#include "X3D_Math.h"

#define X3D_SetPix(plane, x, y)		(*((unsigned char *)(plane) + X3D_ByteOffset(x, y)) |= (0x80 >> X3D_ByteMask(x)))
#define X3D_ClrPix(plane, x, y)		(*((unsigned char *)(plane) + X3D_ByteOffset(x, y)) &= ~(0x80 >> X3D_ByteMask(x)))
#define X3D_ScreenTest(x, y)		(x >= 0 && x < X3D_SCREEN_WIDTH && y >= 0 && y < X3D_SCREEN_HEIGHT)

// Colors
#define X3D_COLOR			unsigned char
#define X3D_COLORS_WHITE	0
#define X3D_COLORS_LGRAY	1
#define X3D_COLORS_DGRAY	2
#define X3D_COLORS_BLACK	3

// Line Drawing
X3D_RESULT X3D_ClipLine(short x1 asm("%d2"), short y1 asm("%d3"), short x2 asm("%d0"), short y2 asm("%d1"), short *ClippedCoordinates asm("%a0"));
void X3D_DrawLine(void *Plane asm("%a0"), short x1 asm("%d0"), short y1 asm("%d1"), short x2 asm("%d2"), short y2 asm("%d3"), X3D_COLOR Color);
void X3D_DrawLine_Clipped(void *Plane asm("%a0"), short x1 asm("%d2"), short y1 asm("%d3"), short x2 asm("%d0"), short y2 asm("%d1"), X3D_COLOR Color);
void X3D_DrawGrayLine(void *Plane1 asm("%a0"), void *Plane2 asm("%a1"), short x1 asm("%d0"), short y1 asm("%d1"), short x2 asm("%d2"), short y2 asm("%d3"), X3D_COLOR Color);
void X3D_DrawGrayLine_Clipped(void *Plane1 asm("%a0"), void *Plane2 asm("%a1"), short x1 asm("%d2"), short y1 asm("%d3"), short x2 asm("%d0"), short y2 asm("%d1"), X3D_COLOR Color);
void X3D_DrawHLineWHITE(short *Address asm("%a0"), short x1 asm("%d0"), short x2 asm("%d1"));
void X3D_DrawHLineBLACK(short *Address asm("%a0"), short x1 asm("%d0"), short x2 asm("%d1"));
void X3D_DrawGrayHLineWHITE(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));
void X3D_DrawGrayHLineLGRAY(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));
void X3D_DrawGrayHLineDGRAY(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));
void X3D_DrawGrayHLineBLACK(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));

// Polygon Drawing
void X3D_FillConvexPolygon(unsigned char *Plane, unsigned char PointCount, X3D_Vec2 *Points, X3D_COLOR Color);
void X3D_FillGrayConvexPolygon(unsigned char *Plane1, unsigned char *Plane2, unsigned char PointCount, X3D_Vec2 *Points, X3D_COLOR Color);

// Screen Operations
void X3D_ClearScreen(void *Plane asm("%a0"));
void X3D_FillScreen(void *Plane asm("%a0"));
void X3D_CopyScreen(void *Source asm("%a0"),void *Destination asm("%a1"));

#endif
