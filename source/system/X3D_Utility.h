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

#ifndef X3D_UTILITY
#define X3D_UTILITY

#include "ScreenConstants.h"

// Byte and Word manipulation
#define X3D_ByteRotateR(x, y)		asm("ror.b %2, %0" : "=d" (x) : "0" (x), "d" (y))
#define X3D_ByteRotateL(x, y)		asm("rol.b %2, %0" : "=d" (x) : "0" (x), "d" (y))
#define X3D_ByteOffset(x, y)		(((y << 4) - y) * 2 + (x >> 3))	// ((y * X3D_BYTESPERLINE) + (x / 8))  Gives the offset of the byte containing (x, y)
#define X3D_ByteBitOffset(x)		(x & 0x07)	// Gives the bit offset of x within its containing byte
#define X3D_WordRotateR(x, y)		asm("ror.w %2, %0" : "=d" (x) : "0" (x), "d" (y))
#define X3D_WordRotateL(x, y)		asm("rol.w %2, %0" : "=d" (x) : "0" (x), "d" (y))
#define X3D_WordOffset(x, y)		(((y << 4) - y) + (x >> 4))	// ((y * X3D_WORDSPERLINE) + (x / 16))  Gives the offset of the word containing (x, y)
#define X3D_WordBitOffset(x)		(x & 0x0F)	//	Gives the bit offset of x within its containing word

#define X3D_SetPix(plane, x, y)		(*((unsigned char *)(plane) + X3D_ByteOffset(x, y)) |= (0x80 >> X3D_ByteMask(x)))
#define X3D_ClrPix(plane, x, y)		(*((unsigned char *)(plane) + X3D_ByteOffset(x, y)) &= ~(0x80 >> X3D_ByteMask(x)))
#define X3D_ScreenTest(x, y)		(x >= 0 && x < X3D_SCREEN_WIDTH && y >= 0 && y < X3D_SCREEN_HEIGHT)
#define X3D_Switch(x, y)			asm("exg %0, %1" : "=d" (x), "=d" (y) : "0" (x), "1" (y))
#define X3D_Wrap(x, y)				(((x) >= (y)) ? ((x) - (y)) : (x))

#endif
