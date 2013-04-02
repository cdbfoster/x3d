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

#ifndef X3D_SYSTEM
#define X3D_SYSTEM

#include "X3D_Utility.h"

// Display Modes
#define X3D_DISPLAY_MODE				unsigned char
#define X3D_DISPLAY_WIREFRAME			0
#define X3D_DISPLAY_PAINTER				1
#define X3D_DISPLAY_ZBUFFER				2

// Projection Modes
#define X3D_PROJECTION_MODE				unsigned char
#define X3D_PROJECTION_PERSPECTIVE		0
#define X3D_PROJECTION_ORTHOGRAPHIC		1

// Color Modes
#define X3D_COLOR_MODE					unsigned char
#define X3D_COLOR_MONOCHROME			0
#define X3D_COLOR_GRAYSCALE				1

// Backface Cull Modes
#define X3D_BACKFACE_CULL_MODE			unsigned char
#define X3D_BACKFACECULL_OFF			0
#define X3D_BACKFACECULL_ON				1

// Outline Modes
#define X3D_OUTLINE_MODE				unsigned char
#define X3D_OUTLINE_NONE				0
#define X3D_OUTLINE_BLACK				1
#define X3D_OUTLINE_OPPOSITE			2

typedef struct {
	X3D_DISPLAY_MODE		DisplayMode;
	X3D_PROJECTION_MODE		ProjectionMode;
	X3D_COLOR_MODE			ColorMode;
	X3D_BACKFACE_CULL_MODE	BackfaceCullMode;
	X3D_OUTLINE_MODE		OutlineMode;

	void *Plane1, *Plane2; // Light and Dark planes, respectively
} X3D_Parameters;

X3D_RESULT X3D_InitializeEngine(X3D_Parameters *InitialParameters);
X3D_RESULT X3D_TerminateEngine();

X3D_RESULT X3D_SetEngineParameters(X3D_Parameters *Parameters);
X3D_RESULT X3D_GetEngineParameters(X3D_Parameters *Parameters);

#endif
