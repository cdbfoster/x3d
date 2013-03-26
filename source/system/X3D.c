#define _GENERIC_ARCHIVE

#include "X3D.h"

// Internal Variables
X3D_Parameters EngineParameters;

// Forward Declaration of Local Functions========
unsigned short ClipPolygonOnPlane(X3D_ClippedFace *Face, X3D_Vec3 *Vertices, X3D_Plane *Plane, unsigned short OverflowVertexCount, X3D_Vec3 *OverflowVertexBuffer);
short ClipEdgeOnPlane(X3D_Vec3 *Vertex1, X3D_Vec3 *Vertex2, X3D_Plane *Plane, unsigned short *OverflowVertexCount, X3D_Vec3 *OverflowVertexBuffer);
unsigned char VertexOutsideFrustum(X3D_Vec3 *Vertex, X3D_Plane *Frustum);

// Global Functions==============================
void X3D_SetEngineParameters(X3D_Parameters *Parameters)
{
	if (!Parameters)
		return;

	EngineParameters.Plane1 = Parameters->Plane1;
}

void X3D_GetEngineParameters(X3D_Parameters *Parameters)
{
	Parameters = Parameters;
}

X3D_RESULT X3D_UpdateCamera(X3D_Camera *Camera)
{
	if (!Camera)
		return X3D_FAILURE;

	// Calculate View Matrices

	short AngleX = Camera->Rotation.x;
	short AngleY = Camera->Rotation.y;
	short AngleZ = Camera->Rotation.z;

	long SinX = X3D_Sin(AngleX), CosX = X3D_Cos(AngleX);
	long SinY = X3D_Sin(AngleY), CosY = X3D_Cos(AngleY);
	long SinZ = X3D_Sin(AngleZ), CosZ = X3D_Cos(AngleZ);

	long SinXSinY = SinX * SinY;
	long CosXSinY = CosX * SinY;

	X3D_Matrix *ViewMatrix = &Camera->ViewMatrix;
	ViewMatrix->_11 = (CosY * CosZ) >> 8;
	ViewMatrix->_12 = (((CosX * SinZ) << 8) + (SinXSinY * CosZ)) >> 16;
	ViewMatrix->_13 = (((SinX * SinZ) << 8) - (CosXSinY * CosZ)) >> 16;
	ViewMatrix->_21 = (-CosY * SinZ) >> 8;
	ViewMatrix->_22 = (((CosX * CosZ) << 8) - (SinXSinY * SinZ)) >> 16;
	ViewMatrix->_23 = (((SinX * CosZ) << 8) + (CosXSinY * SinZ)) >> 16;
	ViewMatrix->_31 = SinY;
	ViewMatrix->_32 = (-SinX * CosY) >> 8;
	ViewMatrix->_33 = (CosX * CosY) >> 8;

	ViewMatrix->_11_12 = ViewMatrix->_11 * ViewMatrix->_12;
	ViewMatrix->_21_22 = ViewMatrix->_21 * ViewMatrix->_22;
	ViewMatrix->_31_32 = ViewMatrix->_31 * ViewMatrix->_32;

	X3D_Matrix *InverseViewMatrix = &Camera->InverseViewMatrix;
	InverseViewMatrix->_11 = ViewMatrix->_11;
	InverseViewMatrix->_12 = ViewMatrix->_21;
	InverseViewMatrix->_13 = ViewMatrix->_31;
	InverseViewMatrix->_21 = ViewMatrix->_12;
	InverseViewMatrix->_22 = ViewMatrix->_22;
	InverseViewMatrix->_23 = ViewMatrix->_32;
	InverseViewMatrix->_31 = ViewMatrix->_13;
	InverseViewMatrix->_32 = ViewMatrix->_23;
	InverseViewMatrix->_33 = ViewMatrix->_33;

	InverseViewMatrix->_11_12 = InverseViewMatrix->_11 * InverseViewMatrix->_12;
	InverseViewMatrix->_21_22 = InverseViewMatrix->_21 * InverseViewMatrix->_22;
	InverseViewMatrix->_31_32 = InverseViewMatrix->_31 * InverseViewMatrix->_32;

	// Calculate View Frustum

	unsigned char AngleH = Camera->HorizontalFOV;
	unsigned char AngleV = Camera->VerticalFOV;

	long SinH = X3D_Sin(AngleH), CosH = X3D_Cos(AngleH);
	long SinV = X3D_Sin(AngleV), CosV = X3D_Cos(AngleV);

	X3D_Plane *ViewFrustum = Camera->ViewFrustum;
	X3D_Vec3 PlaneNormal[6];

	PlaneNormal[X3D_FRUSTUM_NEAR].x = 0;
	PlaneNormal[X3D_FRUSTUM_NEAR].y = 0;
	PlaneNormal[X3D_FRUSTUM_NEAR].z = 256;
	ViewFrustum[X3D_FRUSTUM_NEAR].PlaneConstant = 256; // 1 << 8;

	PlaneNormal[X3D_FRUSTUM_FAR].x = 0;
	PlaneNormal[X3D_FRUSTUM_FAR].y = 0;
	PlaneNormal[X3D_FRUSTUM_FAR].z = -256;
	ViewFrustum[X3D_FRUSTUM_FAR].PlaneConstant = -38400; // 150 << 8;

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
	long XY;

	for (Plane = 0; Plane < 6; Plane++)
	{
		XY = PlaneNormal[Plane].x * PlaneNormal[Plane].y;

		ViewFrustum[Plane].Normal.x = ((InverseViewMatrix->_11 + PlaneNormal[Plane].y) * (InverseViewMatrix->_12 + PlaneNormal[Plane].x) -
																	 (InverseViewMatrix->_11_12 + XY) + (InverseViewMatrix->_13 * PlaneNormal[Plane].z)) >> 8;
		ViewFrustum[Plane].Normal.y = ((InverseViewMatrix->_21 + PlaneNormal[Plane].y) * (InverseViewMatrix->_22 + PlaneNormal[Plane].x) -
																	 (InverseViewMatrix->_21_22 + XY) + (InverseViewMatrix->_23 * PlaneNormal[Plane].z)) >> 8;
		ViewFrustum[Plane].Normal.z = ((InverseViewMatrix->_31 + PlaneNormal[Plane].y) * (InverseViewMatrix->_32 + PlaneNormal[Plane].x) -
																	 (InverseViewMatrix->_31_32 + XY) + (InverseViewMatrix->_33 * PlaneNormal[Plane].z)) >> 8;

		ViewFrustum[Plane].PlaneConstant += (long)(ViewFrustum[Plane].Normal.x) * Camera->Position.x +
																				(long)(ViewFrustum[Plane].Normal.y) * Camera->Position.y +
																				(long)(ViewFrustum[Plane].Normal.z) * Camera->Position.z;
	}

	return X3D_SUCCESS;
}

X3D_RESULT X3D_Render(X3D_Camera *Camera, unsigned short VertexCount, X3D_VertexStream *VertexStream, unsigned short FaceCount, X3D_FaceStream *FaceStream)
{
	//printf("X3D_Render\n");
	if (!Camera || !VertexStream || !FaceStream)
		return X3D_FAILURE;

	X3D_Vec3 *Vertices = (X3D_Vec3 *)VertexStream;
	X3D_Face *Faces = (X3D_Face *)FaceStream;

	void *Plane1 = (EngineParameters.Plane1 ? EngineParameters.Plane1 : LCD_MEM);

	unsigned char VerticesOutsideFrustum[VertexCount];

	X3D_Plane *Frustum = Camera->ViewFrustum;

	unsigned short Vertex;
	for (Vertex = 0; Vertex < VertexCount; Vertex++)
	{
		// 0 if the vertex is inside the frustum, otherwise frustum plane number + 1
		VerticesOutsideFrustum[Vertex] = VertexOutsideFrustum(&Vertices[Vertex], Frustum);
		//printf("%d, %d, %d: %d\n",Vertices[Vertex].x, Vertices[Vertex].y, Vertices[Vertex].z, VerticesOutsideFrustum[Vertex]);
	}

	X3D_ClippedFace ClippedFaces[FaceCount];

	unsigned short OverflowVertexCount = 0;
	unsigned short AllocatedOverflowVertexCount = 9; // This is the maximum number of intersections a triangle can have with a six-sided frustum

	X3D_Vec3 *OverflowVertexBuffer = NULL;
	if (!(OverflowVertexBuffer = malloc(AllocatedOverflowVertexCount * sizeof(X3D_Vec3))))
		return X3D_MEMORYERROR;

	unsigned short Face;
	for (Face = 0; Face < FaceCount; Face++)
	{
		// Determine whether the face is entirely inside, intersecting, or entirely outside the frustum
		unsigned char InsideFrustum = 0;		// 1 if at least one vertex of the face is inside the frustum
		unsigned char OutsideFrustum = 0;		// 1 if at least one vertex of the face is outside the frustum
		unsigned char SpanningFrustum = 0;  // 1 if two adjacent vertices were clipped by different planes

		unsigned char OldVertexState = 0;
		for (Vertex = 0; Vertex < 3; Vertex++)
		{
			unsigned char VertexState = VerticesOutsideFrustum[Faces[Face].Vertices[Vertex]];
			InsideFrustum = InsideFrustum || !VertexState;
			OutsideFrustum = OutsideFrustum || VertexState;
			if (Vertex > 0) SpanningFrustum = SpanningFrustum || (VertexState != OldVertexState);

			OldVertexState = VertexState;
		}

		if (InsideFrustum && !OutsideFrustum) // Face is entirely inside the frustum
		{
			printf("Inside\n");
			// Copy the face into ClippedFaces
			ClippedFaces[Face].VertexCount = 3;
			memcpy(ClippedFaces[Face].Vertices, Faces[Face].Vertices, 3 * sizeof(unsigned short));
			ClippedFaces[Face].Color = Faces[Face].Color;
		}
		else if ((InsideFrustum && OutsideFrustum) || SpanningFrustum) // Face is intersecting the frustum
		{
			printf("Intersecting\n");
			// Copy the face into ClippedFaces for clipping
			ClippedFaces[Face].VertexCount = 3;
			memcpy(ClippedFaces[Face].Vertices, Faces[Face].Vertices, 3 * sizeof(unsigned short));
			ClippedFaces[Face].Color = Faces[Face].Color;

			// Make room for a minimum of 9 new vertices in the overflow buffer for use in clipping the current face
			// (there should never be more than 9 unused allocated vertices anyway)
			unsigned short NewOverflowVertexCount = OverflowVertexCount + 9;
			if (NewOverflowVertexCount > AllocatedOverflowVertexCount)
			{
				if (!(OverflowVertexBuffer = realloc(OverflowVertexBuffer, NewOverflowVertexCount * sizeof(X3D_Vec3))))
					return X3D_MEMORYERROR;
				AllocatedOverflowVertexCount = NewOverflowVertexCount;
			}

			unsigned char Plane;
			for (Plane = 0; Plane < 6; Plane++)
			{
				// Clip the polygon and get the new number of used vertices in the overflow buffer

				/*printf("Plane: %d\n", Plane);

				printf(" Vertex Buffer: %d\n", VertexCount);
				unsigned short x;
				for (x = 0; x < VertexCount; x++)
					printf(" %d: %d %d %d\n", x, Vertices[x].x, Vertices[x].y, Vertices[x].z);
				ngetchx();

				printf(" Overflow Buffer: %d\n", OverflowVertexCount);
				for (x = 0; x < OverflowVertexCount; x++)
					printf(" %d: %d %d %d\n", ~x, OverflowVertexBuffer[x].x, OverflowVertexBuffer[x].y, OverflowVertexBuffer[x].z);
				ngetchx();

				printf(" Face Info Before Clip: %d\n", ClippedFaces[Face].VertexCount);
				for (x = 0; x < ClippedFaces[Face].VertexCount; x++)
				{
					printf("  %d: ", ClippedFaces[Face].Vertices[x]);
					X3D_Vec3 *v = (ClippedFaces[Face].Vertices[x] >= 0 ? &Vertices[ClippedFaces[Face].Vertices[x]] : &OverflowVertexBuffer[~ClippedFaces[Face].Vertices[x]]);
					printf("%d %d %d\n", v->x, v->y, v->z);
				}
				ngetchx();*/

				OverflowVertexCount = ClipPolygonOnPlane(&ClippedFaces[Face], Vertices, &Frustum[Plane], OverflowVertexCount, OverflowVertexBuffer);

				/*printf(" Overflow Buffer: %d\n", OverflowVertexCount);
				for (x = 0; x < OverflowVertexCount; x++)
					printf(" %d: %d %d %d\n", ~x, OverflowVertexBuffer[x].x, OverflowVertexBuffer[x].y, OverflowVertexBuffer[x].z);
				ngetchx();

				printf(" Face Info After Clip: %d\n", ClippedFaces[Face].VertexCount);
				for (x = 0; x < ClippedFaces[Face].VertexCount; x++)
				{
					printf("  %d: ", ClippedFaces[Face].Vertices[x]);
					X3D_Vec3 *v = (ClippedFaces[Face].Vertices[x] >= 0 ? &Vertices[ClippedFaces[Face].Vertices[x]] : &OverflowVertexBuffer[~ClippedFaces[Face].Vertices[x]]);
					printf("%d %d %d\n", v->x, v->y, v->z);
				}
				ngetchx();*/
			}
		}
		else // Face is entirely outside the frustum
		{
			printf("Outside\n");
			ClippedFaces[Face].VertexCount = 0;
		}
	}

	X3D_Vec3 TransformedVertices[VertexCount]; // Will eventually contain screen space coordinates

	X3D_Vec3 *CameraPosition = &Camera->Position;
	X3D_Matrix *ViewMatrix = &Camera->ViewMatrix;

	short TanH = X3D_Tan(Camera->HorizontalFOV);
	short TanV = X3D_Tan(Camera->VerticalFOV);

	for (Vertex = 0; Vertex < VertexCount; Vertex++)
	{
		if (!VerticesOutsideFrustum[Vertex])
		{
			long X = Vertices[Vertex].x - CameraPosition->x;
			long Y = Vertices[Vertex].y - CameraPosition->y;
			long Z = Vertices[Vertex].z - CameraPosition->z;
			long XY = X * Y;

			X3D_Vec3 *TransformedVertex = &TransformedVertices[Vertex];
			TransformedVertex->x = ((ViewMatrix->_11 + Y) * (ViewMatrix->_12 + X) - (ViewMatrix->_11_12 + XY) + ViewMatrix->_13 * Z) >> 8;
			TransformedVertex->y = ((ViewMatrix->_21 + Y) * (ViewMatrix->_22 + X) - (ViewMatrix->_21_22 + XY) + ViewMatrix->_23 * Z) >> 8;
			TransformedVertex->z = ((ViewMatrix->_31 + Y) * (ViewMatrix->_32 + X) - (ViewMatrix->_31_32 + XY) + ViewMatrix->_33 * Z) >> 8;

			// It shouldn't be necessary to check for TransformedVertex->z > 0 because any vertex <= 0 should be clipped by the near plane.
			TransformedVertex->x = X3D_SCREEN_CENTER_X + ((long)TransformedVertex->x * X3D_SCREEN_SCALE_X) / (((long)TransformedVertex->z * TanH) >> 9);
			TransformedVertex->y = X3D_SCREEN_CENTER_Y - ((long)TransformedVertex->y * X3D_SCREEN_SCALE_Y) / (((long)TransformedVertex->z * TanV) >> 9);
		}
	}

	for (Vertex = 0; Vertex < OverflowVertexCount; Vertex++)
	{
		long X = OverflowVertexBuffer[Vertex].x - CameraPosition->x;
		long Y = OverflowVertexBuffer[Vertex].y - CameraPosition->y;
		long Z = OverflowVertexBuffer[Vertex].z - CameraPosition->z;
		long XY = X * Y;

		X3D_Vec3 *TransformedVertex = &OverflowVertexBuffer[Vertex];
		TransformedVertex->x = ((ViewMatrix->_11 + Y) * (ViewMatrix->_12 + X) - (ViewMatrix->_11_12 + XY) + ViewMatrix->_13 * Z) >> 8;
		TransformedVertex->y = ((ViewMatrix->_21 + Y) * (ViewMatrix->_22 + X) - (ViewMatrix->_21_22 + XY) + ViewMatrix->_23 * Z) >> 8;
		TransformedVertex->z = ((ViewMatrix->_31 + Y) * (ViewMatrix->_32 + X) - (ViewMatrix->_31_32 + XY) + ViewMatrix->_33 * Z) >> 8;

		//printf("Here1 %d, %d\n", OverflowVertexCount, TransformedVertex->z);

		if (TransformedVertex->z)
		{
			TransformedVertex->x = X3D_SCREEN_CENTER_X + ((long)TransformedVertex->x * X3D_SCREEN_SCALE_X) / (((long)TransformedVertex->z * TanH) >> 9);
			TransformedVertex->y = X3D_SCREEN_CENTER_Y - ((long)TransformedVertex->y * X3D_SCREEN_SCALE_Y) / (((long)TransformedVertex->z * TanV) >> 9);
		}
		else
		{
			TransformedVertex->x = 0;
			TransformedVertex->y = 0;
		}


		//printf("Here2\n");
	}

	// Z sort here

	for (Face = 0; Face < FaceCount; Face++)
	{
		unsigned char VertexCount = ClippedFaces[Face].VertexCount;
		if (!VertexCount)
			continue;
		//if (should be backface culled)
		//	continue;
		X3D_Vec2 Polygon[VertexCount];
		for (Vertex = 0; Vertex < VertexCount; Vertex++)
		{
			short VertexIndex = ClippedFaces[Face].Vertices[Vertex];
			X3D_Vec3 *CurrentVertex = (VertexIndex >= 0 ? &TransformedVertices[VertexIndex] : &OverflowVertexBuffer[~VertexIndex]);

			Polygon[Vertex] = *((X3D_Vec2 *)CurrentVertex);
			//printf("%d: %d, %d, %d\n", VertexIndex, Polygon[Vertex].x, Polygon[Vertex].y, CurrentVertex->z);
		}
		X3D_FillPolygon(Plane1, VertexCount, Polygon, X3D_COLORS_BLACK);//ClippedFaces[Face].Color);
	}
	/*
		For each face
		{
			Work out the intersection status of the face
			if face is completely inside the frustum
			{
				Copy face into ClippedFaces[Face]
			}
			else if face is intersecting the frustum
			{
				Copy face into ClippedFaces[Face]
				unsigned short NewOverflowVertexCount = OverflowVertexCount + 9;
				if (NewOverflowVertexCount > AllocatedOverflowVertexCount)
				{
					OverflowVertexBuffer = realloc(OverflowVertexBuffer, NewOverflowVertexCount * sizeof(X3D_Vec3));
					AllocatedOverflowVertexCount = NewOverflowVertexCount;
				}
				For each frustum plane
				{
					OverflowVertexCount = ClipPolygonOnPlane(&ClippedFaces[Face], &Frustum[Plane], OverflowVertexCount, OverflowVertexBuffer);
				}
			}
			else // face is completely outside frustum
			{
				ClippedFaces[Face].VertexCount = 0;
			}
		}

		X3D_Vec3 TransformedVertices[VertexCount];
		For each vertex in Vertices
		{
			if !VerticesOutsideFrustum[Vertex]
			{
				Transform to view space
				Project X and Y coordinates into screen space
			}
		}

		For each vertex in OverflowVertexBuffer
		{
			Transform to view space
			Project X and Y coordinates into screen space
		}

		long AverageZDepths[FaceCount]

		For each face in ClippedFaces
		{
			Calculate average z depth
		}
		Sort z depths

		For each face in ClippedFaces
		{
			if (!ClippedFaces[Face].VertexCount)
				continue
			if backface culled
				continue
			X3D_Vec2 Polygon[ClippedFaces[Face].VertexCount];
			For each vertex in ClippedFaces[Face]
				Copy X and Y into Polygon
			FillPolygon
		}
	*/
	free(OverflowVertexBuffer);

	//printf("Leaving X3D_Render\n");
	return X3D_SUCCESS;
}



// Local Functions===============================
unsigned short ClipPolygonOnPlane(X3D_ClippedFace *Face, X3D_Vec3 *Vertices, X3D_Plane *Plane, unsigned short OverflowVertexCount, X3D_Vec3 *OverflowVertexBuffer)
{
	//printf("ClipPolygonOnPlane\n");
	unsigned char VertexCount = Face->VertexCount;
	short *VertexIndices = Face->Vertices;

	unsigned char VerticesInsideFrustum[VertexCount]; // 1 if on the positive side of the plane, otherwise 0

	unsigned char NewVertexCount = 0;
	short NewVertexIndices[9];

	unsigned short Vertex;
	for (Vertex = 0; Vertex < VertexCount; Vertex++)
	{
		X3D_Vec3 *CurrentVertex = (VertexIndices[Vertex] >= 0 ? &Vertices[VertexIndices[Vertex]] : &OverflowVertexBuffer[~VertexIndices[Vertex]]);
		VerticesInsideFrustum[Vertex] = ((long)Plane->Normal.x * CurrentVertex->x +
																		 (long)Plane->Normal.y * CurrentVertex->y +
																		 (long)Plane->Normal.z * CurrentVertex->z >= Plane->PlaneConstant ? 1 : 0);
	}

	// Wrap the ends so each edge of the face is (Vertex, Vertex + 1)
	VertexIndices[VertexCount] = VertexIndices[0];
	VerticesInsideFrustum[VertexCount] = VerticesInsideFrustum[0];

	// For each edge of the face
	for (Vertex = 0; Vertex < VertexCount; Vertex++)
	{
		short Vertex1Index = VertexIndices[Vertex];
		short Vertex2Index = VertexIndices[Vertex + 1];

		if (VerticesInsideFrustum[Vertex] && VerticesInsideFrustum[Vertex + 1]) // If the start and end point of the edge are good, save the end point
			NewVertexIndices[NewVertexCount++] = Vertex2Index;
		else // something's outside of the frustum...
		{
			// Get the edge's vertices from either Vertices or the overflow buffer (negative indices reference the overflow buffer)
			X3D_Vec3 *Vertex1 = (Vertex1Index >= 0 ? &Vertices[Vertex1Index] : &OverflowVertexBuffer[~Vertex1Index]);
			X3D_Vec3 *Vertex2 = (Vertex2Index >= 0 ? &Vertices[Vertex2Index] : &OverflowVertexBuffer[~Vertex2Index]);

			if (VerticesInsideFrustum[Vertex] && !VerticesInsideFrustum[Vertex + 1]) // If the end is bad, clip the edge and save the new end
				NewVertexIndices[NewVertexCount++] = ClipEdgeOnPlane(Vertex1, Vertex2, Plane, &OverflowVertexCount, OverflowVertexBuffer);
			else if (!VerticesInsideFrustum[Vertex] && VerticesInsideFrustum[Vertex + 1]) // If the start is bad, clip the edge and save the new start and the old end
			{
				NewVertexIndices[NewVertexCount++] = ClipEdgeOnPlane(Vertex2, Vertex1, Plane, &OverflowVertexCount, OverflowVertexBuffer);
				NewVertexIndices[NewVertexCount++] = Vertex2Index;
			}
		}
	}

	// Copy the new data into the face
	Face->VertexCount = NewVertexCount;
	memcpy(Face->Vertices, NewVertexIndices, NewVertexCount * sizeof(short));

	//printf("Leaving ClipPolygonOnPlane\n");
	return OverflowVertexCount;
}

short ClipEdgeOnPlane(X3D_Vec3 *Vertex1, X3D_Vec3 *Vertex2, X3D_Plane *Plane, unsigned short *OverflowVertexCount, X3D_Vec3 *OverflowVertexBuffer)
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

	short NewVertexIndex = *OverflowVertexCount;
	X3D_Vec3 *NewVertex = &OverflowVertexBuffer[NewVertexIndex];

	// NewVertex = Lerp(Vertex1, Vertex2, t);
	NewVertex->x = Vertex1->x + ((t * dx) >> 8);
	NewVertex->y = Vertex1->y + ((t * dy) >> 8);
	NewVertex->z = Vertex1->z + ((t * dz) >> 8);

	(*OverflowVertexCount)++;
	return ~NewVertexIndex;
}

unsigned char VertexOutsideFrustum(X3D_Vec3 *Vertex, X3D_Plane *Frustum)
{
	unsigned char Plane;
	for (Plane = 0; Plane < 6; Plane++)
	{
		//printf("%d, %d, %d\n", Vertex->x, Vertex->y, Vertex->z);
		if (((long)Frustum[Plane].Normal.x * Vertex->x +
				 (long)Frustum[Plane].Normal.y * Vertex->y +
				 (long)Frustum[Plane].Normal.z * Vertex->z) < Frustum[Plane].PlaneConstant)
			return Plane + 1;
	}
	return 0;
}
