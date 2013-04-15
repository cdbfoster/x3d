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

#ifndef X3D_RENDER_CAMERA
#define X3D_RENDER_CAMERA

#include "../api/external/X3D_Utility.h"
#include "../api/external/X3D_Math.h"
#include "../api/external/X3D_Render.h"

// Frustum Planes
#define X3D_FRUSTUM_NEAR				0
#define X3D_FRUSTUM_FAR					1
#define X3D_FRUSTUM_LEFT				2
#define X3D_FRUSTUM_RIGHT				3
#define X3D_FRUSTUM_TOP					4
#define X3D_FRUSTUM_BOTTOM				5

typedef struct {
	X3D_Camera User;
	X3D_Matrix ViewMatrix; // User camera actually contains the inverse view matrix (View to World).
	X3D_Plane ViewFrustum[6];
} RenderCameraType;

extern RenderCameraType RenderCamera;

X3D_RESULT FrustumCullPolygons(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Vertices *ResultVertices, X3D_Polygons *ResultPolygons);

#endif
