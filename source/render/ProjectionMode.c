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

#include "../api/external/X3D_Utility.h"
#include "../api/external/X3D_System.h"

#include "RenderFunctions.h"
#include "ProjectionMode.h"

#include "projectionmode/PerspectiveProjectionMode.h"
//#include "projectionmode/OrthographicProjectionMode.h"

X3D_RESULT InitializeProjectionMode(X3D_Parameters *Parameters)
{
	switch (Parameters->ProjectionMode)
	{
	case X3D_PROJECTION_PERSPECTIVE:
		Render.ProjectionMode_ProjectVertices = Perspective_ProjectVertices;
		break;
	case X3D_PROJECTION_ORTHOGRAPHIC:
	default:
		return X3D_FAILURE;
	}

	return X3D_SUCCESS;
}
