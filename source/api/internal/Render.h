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

#ifndef X3D_RENDER_INTERNAL_API
#define X3D_RENDER_INTERNAL_API

#include "../external/X3D_System.h"
#include "../external/X3D_Utility.h"
#include "../external/X3D_Draw.h"

typedef struct {
	X3D_COLOR Color;
	unsigned short Vertices[9];
	unsigned char VertexCount;
} X3D_Polygon;

typedef struct {
	unsigned short PolygonCount;
	X3D_Polygon *Polygons;
} X3D_Polygons;

X3D_RESULT InitializeRender(X3D_Parameters *InitialParameters);
X3D_RESULT TerminateRender();

#endif
