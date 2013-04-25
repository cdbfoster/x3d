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
#include "../api/external/X3D_Math.h"
#include "../api/external/X3D_Render.h"

#include "RenderCamera.h"

RenderCameraType RenderCamera;

X3D_RESULT X3D_UpdateRenderCamera(X3D_Camera *Camera)
{
	if (Camera == NULL)
		return X3D_FAILURE;

	memcpy(&RenderCamera.User, Camera, sizeof(X3D_Camera));

	X3D_InvertTransform(&Camera->Transform, &RenderCamera.ViewTransform);

	unsigned char AngleH = RenderCamera.User.HorizontalFOV / 2;
	unsigned char AngleV = RenderCamera.User.VerticalFOV / 2;

	short SinH = X3D_Sin(AngleH), CosH = X3D_Cos(AngleH);
	short SinV = X3D_Sin(AngleV), CosV = X3D_Cos(AngleV);

	X3D_Plane *ViewFrustum = RenderCamera.ViewFrustum;
	X3D_Vec3 PlaneNormal[6];

	PlaneNormal[X3D_FRUSTUM_NEAR].x = 0;
	PlaneNormal[X3D_FRUSTUM_NEAR].y = 0;
	PlaneNormal[X3D_FRUSTUM_NEAR].z = 256;
	ViewFrustum[X3D_FRUSTUM_NEAR].PlaneConstant = (long)RenderCamera.User.NearClip << 8;

	PlaneNormal[X3D_FRUSTUM_FAR].x = 0;
	PlaneNormal[X3D_FRUSTUM_FAR].y = 0;
	PlaneNormal[X3D_FRUSTUM_FAR].z = -256;
	ViewFrustum[X3D_FRUSTUM_FAR].PlaneConstant = -((long)RenderCamera.User.FarClip << 8);

	PlaneNormal[X3D_FRUSTUM_LEFT].x = CosH;
	PlaneNormal[X3D_FRUSTUM_LEFT].y = 0;
	PlaneNormal[X3D_FRUSTUM_LEFT].z = SinH;
	ViewFrustum[X3D_FRUSTUM_LEFT].PlaneConstant = 0;

	PlaneNormal[X3D_FRUSTUM_RIGHT].x = -CosH;
	PlaneNormal[X3D_FRUSTUM_RIGHT].y = 0;
	PlaneNormal[X3D_FRUSTUM_RIGHT].z = SinH;
	ViewFrustum[X3D_FRUSTUM_RIGHT].PlaneConstant = 0;

	PlaneNormal[X3D_FRUSTUM_TOP].x = 0;
	PlaneNormal[X3D_FRUSTUM_TOP].y = -CosV;
	PlaneNormal[X3D_FRUSTUM_TOP].z = SinV;;
	ViewFrustum[X3D_FRUSTUM_TOP].PlaneConstant = 0;

	PlaneNormal[X3D_FRUSTUM_BOTTOM].x = 0;
	PlaneNormal[X3D_FRUSTUM_BOTTOM].y = CosV;
	PlaneNormal[X3D_FRUSTUM_BOTTOM].z = SinV;
	ViewFrustum[X3D_FRUSTUM_BOTTOM].PlaneConstant = 0;

	unsigned char Plane;
	for (Plane = 0; Plane < 6; Plane++)
	{
		//X3D_TransformVec3(&Camera->Transform, &PlaneNormal[Plane], &ViewFrustum[Plane].Normal);
		X3D_MultiplyMatrixVec3(&Camera->Transform.Rotation, &PlaneNormal[Plane], &ViewFrustum[Plane].Normal);
		ViewFrustum[Plane].PlaneConstant += (long)ViewFrustum[Plane].Normal.x * Camera->Transform.Translation.x +
											(long)ViewFrustum[Plane].Normal.y * Camera->Transform.Translation.y +
											(long)ViewFrustum[Plane].Normal.z * Camera->Transform.Translation.z;

		//printf("%d, %d, %d, %ld\n", ViewFrustum[Plane].Normal.x, ViewFrustum[Plane].Normal.y, ViewFrustum[Plane].Normal.z, ViewFrustum[Plane].PlaneConstant);
	}

	return X3D_SUCCESS;
}

char VertexOutsideFrustum(X3D_Vertex *Vertex);
void ClipPolygonOnPlane(X3D_Polygon *Polygon, X3D_Plane *Plane, unsigned short BaseVertexCount, X3D_Vertex *BaseVertices, unsigned short *OverflowVertexCount, X3D_Vertex *OverflowVertices);

X3D_RESULT FrustumCullTriangles(X3D_Vertices *Vertices, X3D_Triangles *Triangles, X3D_Vertices *ResultVertices, X3D_Polygons *ResultPolygons)
{
	char VertexState[Vertices->VertexCount];
	memset(VertexState, -1, Vertices->VertexCount * sizeof(char));

	unsigned short TrianglesToCopyCount = 0;
	unsigned short TrianglesToCopy[Triangles->TriangleCount];

	unsigned short TrianglesToClipCount = 0;
	unsigned short TrianglesToClip[Triangles->TriangleCount];

	unsigned char Triangle;
	for (Triangle = 0; Triangle < Triangles->TriangleCount; Triangle++)
	{
		// Determine whether or not the triangle is entirely inside, intersecting, or entirely outside the frustum
		unsigned char InsideFrustum = 0;	// 1 if at least one vertex of the triangle is inside the frustum
		unsigned char OutsideFrustum = 0;	// 1 if at least one vertex of the triangle is outside the frustum
		unsigned char SpanningFrustum = 0;  // 1 if two adjacent vertices were clipped by different planes

		unsigned char OldState = 0;

		unsigned char Vertex;
		for (Vertex = 0; Vertex < 3; Vertex++)
		{
			unsigned short VertexIndex = Triangles->Triangles[Triangle].Vertices[Vertex];

			if (VertexState[VertexIndex] == -1)
				VertexState[VertexIndex] = VertexOutsideFrustum(&Vertices->Vertices[VertexIndex]);

			char State = VertexState[VertexIndex];
			InsideFrustum = InsideFrustum || !State;
			OutsideFrustum = OutsideFrustum || State;
			if (Vertex > 0)
				SpanningFrustum = SpanningFrustum || (State != OldState);

			OldState = State;
		}

		if (InsideFrustum && !OutsideFrustum)
			TrianglesToCopy[TrianglesToCopyCount++] = Triangle;
		else if ((InsideFrustum && OutsideFrustum) || SpanningFrustum)
			TrianglesToClip[TrianglesToClipCount++] = Triangle;
	}

	unsigned short PolygonCount = TrianglesToCopyCount + TrianglesToClipCount;

	if (ResultPolygons->Polygons == NULL)
	{
		ResultPolygons->PolygonCount = PolygonCount;
		if (!(ResultPolygons->Polygons = malloc(PolygonCount * sizeof(X3D_Polygon))))
			return X3D_MEMORYERROR;
	}
	else
	{
		if (ResultPolygons->PolygonCount < PolygonCount)
			if (!(ResultPolygons->Polygons = realloc(ResultPolygons->Polygons, PolygonCount * sizeof(X3D_Polygon))))
				return X3D_MEMORYERROR;

		ResultPolygons->PolygonCount = PolygonCount;
	}

	unsigned short a;
	for (a = 0; a < TrianglesToCopyCount; a++)
	{
		unsigned short TriangleIndex = TrianglesToCopy[a];
		*(X3D_Triangle *)(&ResultPolygons->Polygons[a]) = Triangles->Triangles[TriangleIndex];
		ResultPolygons->Polygons[a].VertexCount = 3;
	}

	unsigned short OverflowVertexCount = 0;
	unsigned short AllocatedOverflowVertexCount = 9;  // Start with space for 9, the maximum number of intersections a single triangle can have with a six sided frustum
	X3D_Vertex *OverflowVertices = NULL;
	if (!(OverflowVertices = malloc(AllocatedOverflowVertexCount * sizeof(X3D_Vertex))))
		return X3D_MEMORYERROR;

	for (a = TrianglesToCopyCount; a < PolygonCount; a++)
	{
		X3D_Polygon *Polygon = &ResultPolygons->Polygons[a];

		unsigned short TriangleIndex = TrianglesToClip[a - TrianglesToCopyCount];
		*(X3D_Triangle *)Polygon = Triangles->Triangles[TriangleIndex];
		Polygon->VertexCount = 3;

		// Ensure available overflow space for all possible vertices that might come from clipping this triangle
		if (OverflowVertexCount + 9 > AllocatedOverflowVertexCount)
		{
			AllocatedOverflowVertexCount = OverflowVertexCount + 9;
			if (!(OverflowVertices = realloc(OverflowVertices, AllocatedOverflowVertexCount * sizeof(X3D_Vertex))))
				return X3D_MEMORYERROR;
		}

		unsigned char Plane;
		for (Plane = 0; Plane < 6; Plane++)
		{
			ClipPolygonOnPlane(Polygon, &RenderCamera.ViewFrustum[Plane], Vertices->VertexCount, Vertices->Vertices, &OverflowVertexCount, OverflowVertices);
		}
	}

	unsigned short VertexCount = Vertices->VertexCount + OverflowVertexCount;

	if (ResultVertices->Vertices == NULL)
	{
		ResultVertices->VertexCount = VertexCount;
		if (!(ResultVertices->Vertices = malloc(VertexCount * sizeof(X3D_Vertex))))
			return X3D_MEMORYERROR;
	}
	else
	{
		if (ResultVertices->VertexCount < VertexCount)
			if (!(ResultVertices->Vertices = realloc(ResultVertices->Vertices, VertexCount * sizeof(X3D_Vertex))))
				return X3D_MEMORYERROR;

		ResultVertices->VertexCount = VertexCount;
	}

	memcpy(ResultVertices->Vertices, Vertices->Vertices, Vertices->VertexCount * sizeof(X3D_Vertex));

	X3D_Vertex *OverflowStart = ResultVertices->Vertices + Vertices->VertexCount;
	memcpy(OverflowStart, OverflowVertices, OverflowVertexCount * sizeof(X3D_Vertex));

	free(OverflowVertices);
	return X3D_SUCCESS;
}

char VertexOutsideFrustum(X3D_Vertex *Vertex)
{
	unsigned char Plane;
	for (Plane = 0; Plane < 6; Plane++)
	{
		if ((long)RenderCamera.ViewFrustum[Plane].Normal.x * Vertex->x +
			(long)RenderCamera.ViewFrustum[Plane].Normal.y * Vertex->y +
			(long)RenderCamera.ViewFrustum[Plane].Normal.z * Vertex->z < RenderCamera.ViewFrustum[Plane].PlaneConstant)
			return Plane + 1;
	}

	return 0;
}

void ClipEdgeOnPlane(X3D_Vertex *Vertex1, X3D_Vertex *Vertex2, X3D_Plane *Plane, unsigned short *OverflowVertexCount, X3D_Vertex *OverflowVertices);

void ClipPolygonOnPlane(X3D_Polygon *Polygon, X3D_Plane *Plane, unsigned short BaseVertexCount, X3D_Vertex *BaseVertices, unsigned short *OverflowVertexCount, X3D_Vertex *OverflowVertices)
{
	unsigned char VertexCount = Polygon->VertexCount;
	unsigned short VertexIndices[VertexCount + 1];

	memcpy(VertexIndices, Polygon->Vertices, VertexCount * sizeof(unsigned short));
	VertexIndices[VertexCount] = VertexIndices[0];

	unsigned char NewVertexCount = 0;
	unsigned short NewVertexIndices[9];

	// 1 if on the positive side of Plane, 0 otherwise
	unsigned char VertexOkay[VertexCount + 1];
	unsigned char Index;
	for (Index = 0; Index < VertexCount; Index++)
	{
		X3D_Vertex *Vertex = &(VertexIndices[Index] < BaseVertexCount ? BaseVertices[VertexIndices[Index]] : OverflowVertices[VertexIndices[Index] - BaseVertexCount]);
		VertexOkay[Index] = ((long)Plane->Normal.x * Vertex->x +
							 (long)Plane->Normal.y * Vertex->y +
							 (long)Plane->Normal.z * Vertex->z >= Plane->PlaneConstant ? 1 : 0);
	}
	VertexOkay[VertexCount] = VertexOkay[0];

	for (Index = 0; Index < VertexCount; Index++)
	{
		unsigned short Index1 = VertexIndices[Index];
		unsigned short Index2 = VertexIndices[Index + 1];

		if (VertexOkay[Index])
		{
			NewVertexIndices[NewVertexCount++] = Index1;

			if (!VertexOkay[Index + 1])
			{
				X3D_Vertex *Vertex1 = &(Index1 < BaseVertexCount ? BaseVertices[Index1] : OverflowVertices[Index1 - BaseVertexCount]);
				X3D_Vertex *Vertex2 = &(Index2 < BaseVertexCount ? BaseVertices[Index2] : OverflowVertices[Index2 - BaseVertexCount]);

				ClipEdgeOnPlane(Vertex1, Vertex2, Plane, OverflowVertexCount, OverflowVertices);
				NewVertexIndices[NewVertexCount++] = (BaseVertexCount + *OverflowVertexCount) - 1;
			}
		}
		else if (VertexOkay[Index + 1])
		{
			X3D_Vertex *Vertex1 = &(Index1 < BaseVertexCount ? BaseVertices[Index1] : OverflowVertices[Index1 - BaseVertexCount]);
			X3D_Vertex *Vertex2 = &(Index2 < BaseVertexCount ? BaseVertices[Index2] : OverflowVertices[Index2 - BaseVertexCount]);

			ClipEdgeOnPlane(Vertex2, Vertex1, Plane, OverflowVertexCount, OverflowVertices);
			NewVertexIndices[NewVertexCount++] = (BaseVertexCount + *OverflowVertexCount) - 1;
		}
	}
	
	Polygon->VertexCount = NewVertexCount;
	memcpy(Polygon->Vertices, NewVertexIndices, NewVertexCount * sizeof(unsigned short));
}

void ClipEdgeOnPlane(X3D_Vertex *Vertex1, X3D_Vertex *Vertex2, X3D_Plane *Plane, unsigned short *OverflowVertexCount, X3D_Vertex *OverflowVertices)
{
	//      PlaneConstant - PlaneNormal . Vertex1
	// t = ---------------------------------------
	// 		    PlaneNormal . (Vertex2 - Vertex1)

	long t = 0;

	short dx = Vertex2->x - Vertex1->x;
	short dy = Vertex2->y - Vertex1->y;
	short dz = Vertex2->z - Vertex1->z;

	long Denominator = (long)Plane->Normal.x * dx + (long)Plane->Normal.y * dy + (long)Plane->Normal.z * dz;
	if (Denominator)
		t = ((Plane->PlaneConstant - ((long)Plane->Normal.x * Vertex1->x + (long)Plane->Normal.y * Vertex1->y + (long)Plane->Normal.z * Vertex1->z)) << 8) / Denominator;

	short NewVertexIndex = (*OverflowVertexCount)++;
	X3D_Vertex *NewVertex = &OverflowVertices[NewVertexIndex];

	// NewVertex = Lerp(Vertex1, Vertex2, t);
	NewVertex->x = Vertex1->x + ((t * dx) >> 8);
	NewVertex->y = Vertex1->y + ((t * dy) >> 8);
	NewVertex->z = Vertex1->z + ((t * dz) >> 8);
}

X3D_RESULT ViewTransformVertices(X3D_Vertices *Vertices, X3D_Vertices *ResultVertices)
{
	if (Vertices != ResultVertices && ResultVertices->Vertices == NULL)
	{
		ResultVertices->VertexCount = Vertices->VertexCount;
		if (!(ResultVertices->Vertices = malloc(ResultVertices->VertexCount * sizeof(X3D_Vertex))))
			return X3D_MEMORYERROR;
	}
	
	X3D_InvertedTransformVec3Array(&RenderCamera.ViewTransform, Vertices->VertexCount, Vertices->Vertices, ResultVertices->Vertices);

	return X3D_SUCCESS;
}
