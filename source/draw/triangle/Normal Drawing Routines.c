#define _GENERIC_ARCHIVE

#include "X3D.h"

// Clean up all of these functions some day... some very sad day... TODAY

void X3D_FillPolygon(unsigned char *Plane, unsigned char PointCount, X3D_Vec2 *Points, unsigned char Color)
{
	void (*DrawHLine)(short *address asm("%a0"),short x1 asm("%d0"),short x2 asm("%d1"));
	
	if (Color)	DrawHLine=X3D_DrawHLineBLACK;
	else 				DrawHLine=X3D_DrawHLineWHITE;
	
	short a,b;
	short Start=0, YMin = Points[0].y, YMax = YMin;
	
	for (a=1;a<PointCount;a++)
	{
		if (Points[a].y<YMin)
		{
			YMin=Points[a].y;
			Start=a;
		}
		if (Points[a].y > YMax)
			YMax = Points[a].y;
	}
	
	if (YMax < 0 || YMin > X3D_SCREEN_HEIGHT)
		return;
	
	X3D_Vec2	Reordered[PointCount+1];
	for (a=0;a<PointCount;a++)
	{
		Reordered[a]=Points[X3D_Shift(a+Start,PointCount)];
	}
	Reordered[PointCount]=Reordered[0];
	
	short Slopes[PointCount];
	short dy;
	for (a=0;a<PointCount;a++)
	{
		dy=Reordered[a+1].y-Reordered[a].y;
		if (dy)	Slopes[a]=((Reordered[a+1].x-Reordered[a].x)<<7)/dy;
		else		Slopes[a]=X3D_UNDEFINED;
	}
	
	short YPos=Reordered[0].y,YEnd;
	short x1=Reordered[0].x<<7,x2=x1;
	short inc1,inc2;
	unsigned char *address=Plane+(YPos<<5)-(YPos<<1);
	
	a=0;b=PointCount-1;
	do
	{
		inc1=Slopes[a];
		inc2=Slopes[b];
		if (inc1==X3D_UNDEFINED)			x1=Reordered[++a].x<<7;
		else if (inc2==X3D_UNDEFINED)	x2=Reordered[b--].x<<7;
		else
		{
			if (Reordered[a+1].y>Reordered[b].y)	{YEnd=Reordered[b--].y;}
			else	{YEnd=Reordered[++a].y;}
			for (;YPos<YEnd;YPos++,x1+=inc1,x2+=inc2,address+=X3D_SCREEN_BYTESPERLINE)
				if (YPos >= 0 && YPos <= X3D_SCREEN_HEIGHT)
						DrawHLine((short *)address,x1>>7,x2>>7);
		}
	}while(a<b);
	
	if (YPos<=X3D_SCREEN_HEIGHT)	DrawHLine((short *)address,x1>>7,x2>>7);
}

void X3D_FillGrayPolygon(unsigned char *Plane1, unsigned char *Plane2, unsigned char PointCount, X3D_Vec2 *Points, unsigned char Color)
{
	void (*DrawHLine)(short *address1 asm("%a0"),short *address2 asm("%a1"),short x1 asm("%d0"),short x2 asm("%d1"));
	
	switch (Color)
	{	case X3D_COLORS_WHITE:
			DrawHLine=X3D_DrawGrayHLineWHITE;break;
		case X3D_COLORS_LGRAY:
			DrawHLine=X3D_DrawGrayHLineLGRAY;break;
		case X3D_COLORS_DGRAY:
			DrawHLine=X3D_DrawGrayHLineDGRAY;break;
		default:
			DrawHLine=X3D_DrawGrayHLineBLACK;break;}
	
	short a,b;
	short Start=0,YMin=Points[0].y;
	
	for (a=1;a<PointCount;a++)
	{
		if (Points[a].y<YMin)
		{
			YMin=Points[a].y;
			Start=a;
		}
	}
	
	X3D_Vec2	Reordered[PointCount+1];
	for (a=0;a<PointCount;a++)
	{
		Reordered[a]=Points[X3D_Shift(a+Start,PointCount)];
	}
	Reordered[PointCount]=Reordered[0];
	
	short Slopes[PointCount];
	short dy;
	for (a=0;a<PointCount;a++)
	{
		dy=Reordered[a+1].y-Reordered[a].y;
		if (dy)	Slopes[a]=((Reordered[a+1].x-Reordered[a].x)<<7)/dy;
		else		Slopes[a]=X3D_UNDEFINED;
	}
	
	short YPos=Reordered[0].y,YEnd;
	short x1=Reordered[0].x<<7,x2=x1;
	short inc1,inc2;
	unsigned char *address1=Plane1+(YPos<<5)-(YPos<<1);
	unsigned char *address2=Plane2+(YPos<<5)-(YPos<<1);
	
	a=0;b=PointCount-1;
	do
	{
		inc1=Slopes[a];
		inc2=Slopes[b];
		if (inc1==X3D_UNDEFINED)			x1=Reordered[++a].x<<7;
		else if (inc2==X3D_UNDEFINED)	x2=Reordered[b--].x<<7;
		else
		{
			if (Reordered[a+1].y>Reordered[b].y)	{YEnd=Reordered[b--].y;}
			else	{YEnd=Reordered[++a].y;}
			for (;YPos<YEnd;YPos++,x1+=inc1,x2+=inc2,address1+=X3D_SCREEN_BYTESPERLINE,address2+=X3D_SCREEN_BYTESPERLINE)
				if (YPos >= 0 && YPos <= X3D_SCREEN_HEIGHT)	DrawHLine((short *)address1,(short *)address2,x1>>7,x2>>7);
		}
	}while(a<b);
	
	if (YPos<=X3D_SCREEN_HEIGHT)	DrawHLine((short *)address1,(short *)address2,x1>>7,x2>>7);
}