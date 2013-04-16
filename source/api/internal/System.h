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

#ifndef X3D_SYSTEM_INTERNAL_API
#define X3D_SYSTEM_INTERNAL_API

#include "../external/X3D_System.h"

// EngineState
#define ENGINESTATE_ALL						0x07
#define ENGINESTATE_ENGINE_INITIALIZED		0x01
#define ENGINESTATE_COLORMODE_INITILIAZED	0x02
#define ENGINESTATE_RENDER_INITIALIZED		0x04

typedef struct
{
	X3D_Parameters General;
	unsigned char State;
} EngineStateContainer;

extern EngineStateContainer EngineState;

#endif
