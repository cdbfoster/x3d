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
#include "BackfaceCullMode.h"

X3D_RESULT Cull(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons);
X3D_RESULT NoCull(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons);

X3D_RESULT InitializeBackfaceCullMode(X3D_Parameters *Parameters)
{
	switch (Parameters->BackfaceCullMode)
	{
	case X3D_BACKFACECULL_OFF:
		Render.BackfaceCullMode_CullPolygons = NoCull;
		break;
	case X3D_BACKFACECULL_ON:
		Render.BackfaceCullMode_CullPolygons = Cull;
		break;
	default:
		return X3D_FAILURE;
	}

	return X3D_SUCCESS;
}

X3D_RESULT Cull(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons)
{

	return X3D_SUCCESS;
}

X3D_RESULT NoCull(X3D_Vertices *Vertices, X3D_Polygons *Polygons, X3D_Polygons *ResultPolygons)
{
	if (Polygons == ResultPolygons)
		return X3D_SUCCESS;

	if (ResultPolygons->Polygons == NULL)
	{
		if (!(ResultPolygons->Polygons = malloc(Polygons->PolygonCount * sizeof(X3D_Polygon))))
			return X3D_MEMORYERROR;
	}
	else
	{
		if (ResultPolygons->PolygonCount != Polygons->PolygonCount)
			if (!(ResultPolygons->Polygons = realloc(ResultPolygons->Polygons, Polygons->PolygonCount * sizeof(X3D_Polygon))))
				return X3D_MEMORYERROR;
	}

	memcpy(ResultPolygons->Polygons, Polygons->Polygons, Polygons->PolygonCount * sizeof(X3D_Polygon));
	ResultPolygons->PolygonCount = Polygons->PolygonCount;

	return X3D_SUCCESS;
}
