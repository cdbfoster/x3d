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

#ifndef X3D_RENDER
#define X3D_RENDER

#include "X3D_Utility.h"
#include "X3D_Math.h"
#include "X3D_Draw.h"

typedef struct {
	X3D_Transform Transform; // Camera's orientation in the world.  Technically the inverse view transform (View to World).
	X3D_ANGLE HorizontalFOV;
	X3D_ANGLE VerticalFOV;
	unsigned short NearClip, FarClip;
} X3D_Camera;

#define X3D_Vertex	X3D_Vec3

typedef struct {
	unsigned short VertexCount;
	X3D_Vertex *Vertices;
} X3D_Vertices;

typedef struct {
	X3D_COLOR Color;
	unsigned short Vertices[3];
} X3D_Triangle;

typedef struct {
	unsigned short TriangleCount;
	X3D_Triangle *Triangles;
} X3D_Triangles;

X3D_RESULT X3D_UpdateRenderCamera(X3D_Camera *Camera);

X3D_RESULT X3D_Render(X3D_Vertices *Vertices, X3D_Triangles *Triangles);

#endif
