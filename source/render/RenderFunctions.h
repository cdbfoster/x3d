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

#ifndef X3D_RENDER_RENDER_FUNCTIONS
#define X3D_RENDER_RENDER_FUNCTIONS

#include "../api/external/X3D_Utility.h"
#include "../api/external/X3D_Render.h"
#include "../api/internal/Render.h"

typedef struct {
	X3D_RESULT (*DisplayMode_Initialize)();
	X3D_RESULT (*DisplayMode_Draw)(X3D_Vertices *Vertices, X3D_Polygons *Polygons);
	X3D_RESULT (*DisplayMode_Cleanup)();
	X3D_RESULT (*DisplayMode_Terminate)();
	X3D_RESULT (*ProjectionMode_ProjectVertices)(X3D_Vertices *Vertices, X3D_Vertices *ResultVertices);
	X3D_RESULT (*BackfaceCullMode_CullPolygons)(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons);

	void *DisplayMode_Data;
} RenderFunctions;

extern RenderFunctions Render;

#endif
