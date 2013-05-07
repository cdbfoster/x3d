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

#ifndef X3D_RENDER_ORTHOGRAPHIC_PROJECTION_MODE
#define X3D_RENDER_ORTHOGRAPHIC_PROJECTION_MODE

#include "../../api/external/X3D_Utility.h"
#include "../../api/external/X3D_Render.h"
#include "../../api/internal/Render.h"

X3D_RESULT Orthographic_GetViewFrustum(X3D_Plane *ViewFrustum);
X3D_RESULT Orthographic_ProjectVertices(X3D_Vertices *Vertices, X3D_Vertices *ResultVertices);

#endif
