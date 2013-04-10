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

#ifndef X3D_RENDER_WIREFRAME_DISPLAY_MODE
#define X3D_RENDER_WIREFRAME_DISPLAY_MODE

#include "../../api/external/X3D_Utility.h"
#include "../../api/external/X3D_System.h"
#include "../../api/external/X3D_Render.h"

X3D_RESULT Wireframe_Initialize();
X3D_RESULT Wireframe_PrepareDraw(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Vertices *ResultVertices, X3D_Polygons *ResultPolygons);
X3D_RESULT Wireframe_Draw(X3D_Vertices *Vertices, X3D_Polygons *Polygons);
X3D_RESULT Wireframe_Cleanup();
X3D_RESULT Wireframe_Terminate();

#endif
