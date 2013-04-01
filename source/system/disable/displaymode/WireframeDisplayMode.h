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

#ifndef X3D_SYSTEM_DISPLAY_MODE_WIREFRAME
#define X3D_SYSTEM_DISPLAY_MODE_WIREFRAME

#include "../X3D_System.h"
#include "DisplayMode.h"

X3D_RESULT WireframeInitialize();

X3D_RESULT WireframePrepareDraw(unsigned short VertexCount, X3D_Vec3 *Vertices, unsigned short FaceCount, X3D_Face *Faces);

X3D_RESULT WireframeDraw(unsigned short VertexCount, X3D_Vec3 *Vertices, unsigned short FaceCount, X3D_Face *Faces);

X3D_RESULT WireframeCleanup();

X3D_RESULT WireframeTerminate();

DisplayMode WireframeDisplayMode;

#endif
