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

#ifndef X3D
#define X3D
// Constants ====================================
// Engine States
//#define X3D_STATE_ENGINESETUP			0x01
//#define X3D_STATE_PAINTERSETUP		0x02
//#define X3D_STATE_ZBUFFERSETUP		0x04

// Display Modes
//#define X3D_DISPLAY_WIREFRAME			0
//#define X3D_DISPLAY_PAINTER			1
//#define X3D_DISPLAY_ZBUFFER			2

// Projection Modes
//#define X3D_PROJECTION_PERSPECTIVE	0
//#define X3D_PROJECTION_ORTHOGRAPHIC	1

// Color Modes
//#define X3D_COLOR_MONOCHROME			0
//#define X3D_COLOR_GRAYSCALE			1

// Backface Cull Modes
//#define X3D_BACKFACECULL_OFF			0
//#define X3D_BACKFACECULL_ON			1

// Outline Modes
//#define X3D_OUTLINE_NONE				0
//#define X3D_OUTLINE_BLACK				1
//#define X3D_OUTLINE_OPPOSITE			2

// Colors
#define X3D_COLORS_WHITE				0
#define X3D_COLORS_LGRAY				1
#define X3D_COLORS_DGRAY				2
#define X3D_COLORS_BLACK				3

// Screen Parameters
#define X3D_SCREEN_WIDTH				159
#define X3D_SCREEN_HEIGHT				99
#define X3D_SCREEN_CENTER_X				80
#define X3D_SCREEN_CENTER_Y 			50
#define X3D_SCREEN_SCALE_X				80
#define X3D_SCREEN_SCALE_Y				80
#define X3D_SCREEN_BYTESPERLINE			30
#define X3D_SCREEN_WORDSPERLINE			15

// Error Codes and Checking
#define X3D_RESULT						short
#define X3D_SUCCESS						0
#define X3D_FAILURE						-1
#define X3D_MEMORYERROR					-2
#define X3D_FAILED(x)					(((x) != X3D_SUCCESS) ? 1 : 0)

#define	X3D_UNDEFINED					32767

// Frustum Planes
#define X3D_FRUSTUM_NEAR				0
#define X3D_FRUSTUM_FAR					1
#define X3D_FRUSTUM_LEFT				2
#define X3D_FRUSTUM_RIGHT				3
#define X3D_FRUSTUM_TOP					4
#define X3D_FRUSTUM_BOTTOM				5

// Macros =======================================
// Byte and Word manipulation
#define X3D_ByteRotateR(x, y)		asm("ror.b %2, %0" : "=d" (x) : "0" (x), "d" (y))
#define X3D_ByteRotateL(x, y)		asm("rol.b %2, %0" : "=d" (x) : "0" (x), "d" (y))
#define X3D_ByteOffset(x, y)		(((y << 4) - y) * 2 + (x >> 3))	// ((y * X3D_BYTESPERLINE) + (x / 8))  Gives the offset of the byte containing (x, y)
#define X3D_ByteBitOffset(x)		(x & 0x07)	// Gives the bit offset of x within its containing byte
#define X3D_WordRotateR(x, y)		asm("ror.w %2, %0" : "=d" (x) : "0" (x), "d" (y))
#define X3D_WordRotateL(x, y)		asm("rol.w %2, %0" : "=d" (x) : "0" (x), "d" (y))
#define X3D_WordOffset(x, y)		(((y << 4) - y) + (x >> 4))	// ((y * X3D_WORDSPERLINE) + (x / 16))  Gives the offset of the word containing (x, y)
#define X3D_WordBitOffset(x)		(x & 0x0F)	//	Gives the bit offset of x within its containing word

#define X3D_Abs(x)					(((x) < 0) ? -(x) : (x))
#define X3D_Sign(x)					(((x) < 0) ? -1 : 1)
#define X3D_SetPix(plane, x, y)		(*((unsigned char *)(plane) + X3D_ByteOffset(x, y)) |= (0x80 >> X3D_ByteMask(x)))
#define X3D_ClrPix(plane, x, y)		(*((unsigned char *)(plane) + X3D_ByteOffset(x, y)) &= ~(0x80 >> X3D_ByteMask(x)))
#define X3D_ScreenTest(x, y)		(x >= 0 && x < X3D_SCREEN_WIDTH && y >= 0 && y < X3D_SCREEN_HEIGHT)
#define X3D_Switch(x, y)			asm("exg %0, %1" : "=d" (x), "=d" (y) : "0" (x), "1" (y))
#define X3D_WrapAngle(x)			((x) & 0x01FF)
#define X3D_Wrap(x, y)				(((x) >= (y)) ? ((x) - (y)) : (x))

//#define X3D_SetEngineShift(x)		(X3D.EngineShift = (x))
//#define X3D_SetEngineFlag(Flag)	(X3D.EngineState |= Flag)
//#define X3D_ClearEngineFlag(Flag)	(X3D.EngineState &= ~Flag)
//#define X3D_GetEngineFlag(Flag)	(X3D.EngineState & Flag)

#define X3D_VertexStream		short
#define X3D_FaceStream			short

// Types ========================================
typedef struct {
	short x, y;
} X3D_Vec2;

typedef struct {
	short x, y, z;
} X3D_Vec3;

typedef struct {
	unsigned short Vertices[3];
	unsigned char Color;
} X3D_Face;

// Make private
typedef struct {
	unsigned short VertexCount;
	short Vertices[10];
	unsigned char Color;
} X3D_ClippedFace;

typedef struct {
	X3D_Vec3 Position;
	X3D_Vec3 Rotation;
	unsigned char HorizontalFOV, VerticalFOV; // Must be less than 256 (180 degrees)
} X3D_Camera;

// Separate into a public camera and a private camera
/*typedef struct {
	X3D_Vec3 Position;
	X3D_Vec3 Rotation;
	X3D_Matrix ViewMatrix;
	X3D_Matrix InverseViewMatrix;
	unsigned char HorizontalFOV, VerticalFOV; // Half angles.  Must be less than 128 (90 degrees)
	X3D_Plane ViewFrustum[6];
} X3D_Camera;*/

// Make private
typedef struct {
	unsigned char Color;
	long ZDepth;
	unsigned short PointCount;
	X3D_Vec2 Points[9];
} X3D_Painter_ScreenPolygon;

// State Struct
// Separate into public and private
typedef struct {
	unsigned char EngineState; // System use only.  Will not be read.
	unsigned char EngineShift;
	unsigned char DisplayMode;
	unsigned char ProjectionMode;
	unsigned char ColorMode;
	unsigned char BackfaceCullMode;
	unsigned char OutlineMode;

	void *Plane1, *Plane2; // Light and Dark planes, respectively
} X3D_Parameters;

extern void a();

// Functions ====================================
// System ===
extern void X3D_SetEngineParameters(X3D_Parameters *Parameters);
extern void X3D_GetEngineParameters(X3D_Parameters *Parameters);

// Camera ===
extern X3D_RESULT X3D_UpdateCamera(X3D_Camera *Camera);

// Render ===
extern X3D_RESULT X3D_Render(X3D_Camera *Camera, unsigned short VertexCount, X3D_VertexStream *VertexStream, unsigned short FaceCount, X3D_FaceStream *FaceStream);

// Draw ===
// Line Drawing
extern X3D_RESULT X3D_ClipLine(short x1 asm("%d2"), short y1 asm("%d3"), short x2 asm("%d0"), short y2 asm("%d1"), short *ClippedCoordinates asm("%a0"));
extern void X3D_DrawLine(void *Plane asm("%a0"), short x1 asm("%d0"), short y1 asm("%d1"), short x2 asm("%d2"), short y2 asm("%d3"), unsigned char Color);
extern void X3D_DrawLine_Clipped(void *Plane asm("%a0"), short x1 asm("%d2"), short y1 asm("%d3"), short x2 asm("%d0"), short y2 asm("%d1"), unsigned char Color);
extern void X3D_DrawGrayLine(void *Plane1 asm("%a0"), void *Plane2 asm("%a1"), short x1 asm("%d0"), short y1 asm("%d1"), short x2 asm("%d2"), short y2 asm("%d3"), unsigned char Color);
extern void X3D_DrawGrayLine_Clipped(void *Plane1 asm("%a0"), void *Plane2 asm("%a1"), short x1 asm("%d2"), short y1 asm("%d3"), short x2 asm("%d0"), short y2 asm("%d1"), unsigned char Color);
extern void X3D_DrawHLineWHITE(short *Address asm("%a0"), short x1 asm("%d0"), short x2 asm("%d1"));
extern void X3D_DrawHLineBLACK(short *Address asm("%a0"), short x1 asm("%d0"), short x2 asm("%d1"));
extern void X3D_DrawGrayHLineWHITE(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));
extern void X3D_DrawGrayHLineLGRAY(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));
extern void X3D_DrawGrayHLineDGRAY(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));
extern void X3D_DrawGrayHLineBLACK(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));
// Polygon Drawing
extern void X3D_FillConvexPolygon(unsigned char *Plane, unsigned char PointCount, X3D_Vec2 *Points, unsigned char Color);
extern void X3D_FillGrayConvexPolygon(unsigned char *Plane1, unsigned char *Plane2, unsigned char PointCount, X3D_Vec2 *Points, unsigned char Color);
// Screen Operations
extern void X3D_ClearScreen(void *Plane asm("%a0"));
extern void X3D_FillScreen(void *Plane asm("%a0"));
extern void X3D_CopyScreen(void *Source asm("%a0"),void *Destination asm("%a1"));

// Math ===
extern const short X3D_SinTable[512];
extern const short X3D_TanTable[512];
#define X3D_Sin(x)	(X3D_SinTable[X3D_WrapAngle(x)])
#define X3D_Cos(x)	(X3D_SinTable[X3D_WrapAngle((x) + 128)])
#define X3D_Tan(x)	(X3D_TanTable[X3D_WrapAngle(x)])

#endif