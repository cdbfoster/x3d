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
#include "../api/external/X3D_Draw.h"
#include "../api/external/X3D_System.h"
#include "../api/internal/System.h"

#define PLANE1_OWNED	0x01
#define PLANE2_OWNED	0x02
unsigned char PlaneOwnership;

X3D_RESULT X3D_InitializeEngine(X3D_Parameters *InitialParameters)
{
	if (EngineState.State & ENGINESTATE_ENGINE_INITIALIZED)
		return X3D_SUCCESS;
	
	if (InitialParameters == NULL)
		return X3D_FAILURE;
	memcpy(&EngineState.General, InitialParameters, sizeof(X3D_Parameters));

	if (!(EngineState.State & ENGINESTATE_COLORMODE_INITIALIZED))
	{
		PlaneOwnership &= ~PLANE1_OWNED;
		if (EngineState.General.Plane1 == NULL)
		{
			void *Plane = NULL;
			if (!(Plane = malloc(X3D_SCREEN_BUFFERSIZE)))
				return X3D_MEMORYERROR;
			X3D_ClearScreen(Plane);
			
			EngineState.General.Plane1 = Plane;
			PlaneOwnerShip |= PLANE1_OWNED;
		}

		PlaneOwnership &= ~PLANE2_OWNED;
		if (EngineState.General.ColorMode == X3D_COLOR_GRAYSCALE)
		{
			if (EngineState.General.Plane2 == NULL)
			{
				void *Plane = NULL;
				if (!(Plane = malloc(X3D_SCREEN_BUFFERSIZE)))
					return X3D_MEMORYERROR;
				X3D_ClearScreen(Plane);
				
				EngineState.General.Plane2 = Plane;
				PlaneOwnerShip |= PLANE2_OWNED;
			}

			GrayOn();
		}

		EngineState.State |= ENGINESTATE_COLORMODE_INITIALIZED;
	}

	if (!(EngineState.State & ENGINESTATE_RENDER_INITIALIZED))
	{
		X3D_RESULT Result = InitializeRender(&EngineState.General);
		if (X3D_FAILED(Result))
			return Result;
		
		EngineState.State |= ENGINESTATE_RENDER_INITIALIZED;
	}

	EngineState.State |= ENGINESTATE_ENGINE_INITIALIZED;

	return X3D_SUCCESS;
}

X3D_RESULT X3D_TerminateEngine()
{
	if (!(EngineState.State & ENGINESTATE_ENGINE_INITIALIZED))
		return X3D_SUCCESS;

	if (EngineState.State & ENGINESTATE_RENDER_INITIALIZED)
	{
		if (!X3D_FAILED(TerminateRender())
			EngineState.State &= ~ENGINESTATE_RENDER_INITIALIZED;
	}

	if (EngineState.State & ENGINESTATE_COLORMODE_INITIALIZED)
	{
		if (EngineState.General.ColorMode == X3D_COLOR_GRAYSCALE)
			GrayOff();

		if (PlaneOwnership & PLANE1_OWNED)
		{
			free(EngineState.General.Plane1);
			PlaneOwnership &= ~PLANE1_OWNED;
		}
		if (PlaneOwnership & PLANE2_OWNED)
		{
			free(EngineState.General.Plane2);
			PlaneOwnership &= ~PLANE2_OWNED;
		}

		EngineState.State &= ~ENGINESTATE_COLORMODE_INITIALIZED;
	}

	EngineState.State &= ~ENGINESTATE_ENGINE_INITIALIZED;

	if (EngineState.State & ENGINESTATE_ALL)
		return X3D_FAILURE;

	return X3D_SUCCESS;
}

X3D_RESULT X3D_SetEngineParameters(X3D_Parameters *Parameters)
{
	if (!(EngineState.State & ENGINESTATE_ENGINE_INITIALIZED))
		return X3D_FAILURE;

	// Check color mode and planes
	if (EngineState.General.ColorMode != Parameters->ColorMode)
	{
		EngineState.State &= ~ENGINESTATE_COLORMODE_INITIALIZED;

		if (Parameters->ColorMode == X3D_COLOR_MONOCHROME)
		{
			GrayOff();
			
			if (PlaneOwnership & PLANE2_OWNED)
			{
				free(EngineState.General.Plane2);
				PlaneOwnership &= ~PLANE2_OWNED;
			}
			EngineState.General.Plane2 = NULL;
		}
		else
			GrayOn();

		EngineState.General.ColorMode = Parameters->ColorMode;
	}

	if (Parameters->Plane1 != NULL)
	{
		if (PlaneOwnership & PLANE1_OWNED)
		{
			free(EngineState.General.Plane1);
			PlaneOwnership &= ~PLANE1_OWNED;
		}
		EngineState.General.Plane1 = Parameters->Plane1;
	}
	else if (EngineState.General.Plane1 == NULL)
	{
		void *Plane = NULL;
		if (!(Plane = malloc(X3D_SCREEN_BUFFERSIZE)))
			return X3D_MEMORYERROR;
		X3D_ClearScreen(Plane);
		
		EngineState.General.Plane1 = Plane;
		PlaneOwnerShip |= PLANE1_OWNED;
	}

	if (EngineState.General.ColorMode == X3D_COLOR_GRAYSCALE)
	{
		if (Parameters->Plane2 != NULL)
		{
			if (PlaneOwnership & PLANE2_OWNED)
			{
				free(EngineState.General.Plane2);
				PlaneOwnership &= ~PLANE2_OWNED;
			}
			EngineState.General.Plane2 = Parameters->Plane2;
		}
		else if (EngineState.General.Plane2 == NULL)
		{
			void *Plane = NULL;
			if (!(Plane = malloc(X3D_SCREEN_BUFFERSIZE)))
				return X3D_MEMORYERROR;
			X3D_ClearScreen(Plane);
			
			EngineState.General.Plane2 = Plane;
			PlaneOwnerShip |= PLANE2_OWNED;
		}
	}

	EngineState.State |= ENGINESTATE_COLORMODE_INITIALIZED;

	// Check render parameters
	if (EngineState.General.DisplayMode != Parameters->DisplayMode ||
		EngineState.General.ProjectionMode != Parameters->ProjectionMode ||
		EngineState.General.BackfaceCullMode != Parameters->BackfaceCullMode)
	{
		EngineState.State &= ~ENGINESTATE_RENDER_INITIALIZED;

		X3D_RESULT Result = InitializeRender(&EngineState.General);
		if (X3D_FAILED(Result))
			return Result;

		EngineState.General.DisplayMode = Parameters->DisplayMode;
		EngineState.General.ProjectionMode = Parameters->ProjectionMode;
		EngineState.General.BackfaceCullMode = Parameters->BackfaceCullMode;
		
		EngineState.State |= ENGINESTATE_RENDER_INITIALIZED;
	}

	EngineState.General.OutlineMode = Parameters->OutlineMode;

	return X3D_SUCCESS;
}

X3D_RESULT X3D_GetEngineParameters(X3D_Parameters *Parameters)
{
	if (!(EngineState.State & ENGINESTATE_ENGINE_INITIALIZED))
		return X3D_FAILURE;

	memcpy(Parameters, EngineState.General, sizeof(X3D_Parameters));

	return X3D_SUCCESS;
}
