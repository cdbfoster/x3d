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
#include "../api/external/X3D_Render.h"
#include "../api/internal/System.h"
#include "../api/internal/Render.h"

#include "RenderCamera.h"
#include "RenderFunctions.h"

#include "DisplayMode.h"
#include "ProjectionMode.h"
#include "BackfaceCullMode.h"

X3D_RESULT InitializeRender(X3D_Parameters *Parameters)
{
	X3D_RESULT Result;
	
	if (EngineState.General.DisplayMode != Parameters->DisplayMode)
	{
		Result = InitializeDisplayMode(Parameters);
		if (X3D_FAILED(Result))
			return Result;
	}

	/*if (EngineState.General.ProjectionMode != Parameters->ProjectionMode)
	{
		Result = InitializeProjectionMode(Parameters);
		if (X3D_FAILED(Result)
			return Result;
	}*/

	/*if (EngineState.General.BackfaceCullMode != Parameters->BackfaceCullMode)
	{
		Result = InitializeProjectionMode(Parameters);
		if (X3D_FAILED(Result)
			return Result;
	}*/
 
	return X3D_SUCCESS;
}

X3D_RESULT TerminateRender()
{
	return Render.DisplayMode_Terminate();
}

X3D_RESULT X3D_Render(X3D_Vertices *Vertices, X3D_Triangles *Triangles)
{
	if (!(EngineState.State & ENGINESTATE_RENDER_INITIALIZED))
		return X3D_FAILURE;

	if (Vertices == NULL || Triangles == NULL)
		return X3D_FAILURE;

	X3D_Vertices ClippedVertices;
	X3D_Polygons ClippedPolygons;

	X3D_RESULT Result = FrustumCullTriangles(Vertices, Triangles, &ClippedVertices, &ClippedPolygons);
	if (X3D_FAILED(Result))
		return Result;

	Result = ViewTransformVertices(&ClippedVertices, &ClippedVertices);
	if (X3D_FAILED(Result))
		return Result;

	//Result = Render.ProjectionMode_ProjectVertices(&ClippedVertices, &ClippedVertices);
	//if (X3D_FAILED(Result))
	//	return Result;

	//X3D_Polygons BackfaceCulledPolygons
	//Result = Render.BackfaceCullMode_CullPolygons(&ClippedVertices, &ClippedPolygons, &BackfaceCulledPolygons);
	//if (X3D_FAILED(Result))
	//	return Result;

	X3D_Vertices DrawVertices;
	X3D_Polygons DrawPolygons;
	Result = Render.DisplayMode_PrepareDraw(&ClippedVertices, &ClippedPolygons/*&BackfaceCulledPolygons*/, &DrawVertices, &DrawPolygons);
	if (X3D_FAILED(Result))
		return Result;

	Result = Render.DisplayMode_Draw(&DrawVertices, &DrawPolygons);
	if (X3D_FAILED(Result))
		return Result;

	Result = Render.DisplayMode_Cleanup();
	if (X3D_FAILED(Result))
		return Result;

	free(DrawPolygons.Polygons);
	free(DrawVertices.Vertices);
	//free(BackfaceCulledPolygons.Polygons);
	free(ClippedPolygons.Polygons);
	free(ClippedVertices.Vertices);

	return X3D_SUCCESS;
}
