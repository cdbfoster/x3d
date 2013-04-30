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
#include "DisplayMode.h"

#include "displaymode/WireframeDisplayMode.h"
#include "displaymode/PainterDisplayMode.h"
//#include "displaymode/ZBufferDisplayMode.h"

X3D_RESULT InitializeDisplayMode(X3D_Parameters *Parameters)
{
	switch (Parameters->DisplayMode)
	{
	case X3D_DISPLAY_WIREFRAME:
		Render.DisplayMode_Initialize = Wireframe_Initialize;
		Render.DisplayMode_Draw = Wireframe_Draw;
		Render.DisplayMode_Cleanup = Wireframe_Cleanup;
		Render.DisplayMode_Terminate = Wireframe_Terminate;
		break;
	case X3D_DISPLAY_PAINTER:
		Render.DisplayMode_Initialize = Painter_Initialize;
		Render.DisplayMode_Draw = Painter_Draw;
		Render.DisplayMode_Cleanup = Painter_Cleanup;
		Render.DisplayMode_Terminate = Painter_Terminate;
		break;
	case X3D_DISPLAY_ZBUFFER:
	default:
		return X3D_FAILURE;
	}

	return X3D_SUCCESS;
}
