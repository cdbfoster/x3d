#define _GENERIC_ARCHIVE

#include "X3D.h"

/*void X3D_DrawLine2(void *Plane, short x1, short y1, short x2, short y2, unsigned char Color)
{
	unsigned char *p1, *p2, mask1, mask2;
	short dx, dy, l, testY, inc;
	long m;
		if (X3D_Abs(x1 - x2) > X3D_Abs(y1 - y2))
		{
			if (x1 > x2)
			{	
				X3D_Switch(x1, x2);
				X3D_Switch(y1, y2);
			}
			
			if (x1 > X3D_SCREEN_WIDTH || x2 < 0)
				return;
			
			if (!X3D_ScreenTest(x1, y1) || !X3D_ScreenTest(x2, y2)) // We need to correct the endpoints of the line
			{
				dx = x2 - x1;
				dy = y2 - y1;
				m = (((long)dy) << 11) / dx;
				
				if (x1 < 0)
				{
					y1 += (short)(m * (-x1)) >> 11;
					x1 = 0;
				}
				if (x2 > X3D_SCREEN_WIDTH)
				{
					y2 -= (short)((m * (x2 - X3D_SCREEN_WIDTH)) >> 11);
					x2 = X3D_SCREEN_WIDTH;
				}
				
				if (dy)
				{
					m = ((long)(dx) << 11) / dy;
					
					if (y1 < 0)
					{
						x1 += (short)((m * (-y1)) >> 11);
						y1=0;
					}
					if (y1 > X3D_SCREEN_HEIGHT)
					{
						x1 -= (short)((m * (y1 - X3D_SCREEN_HEIGHT)) >> 11);
						y1 = X3D_SCREEN_HEIGHT;
					}
					
					if (y2 < 0)
					{
						x2 += (short)((m * (-y2)) >> 11);
						y2 = 0;
					}
					if (y2 > X3D_SCREEN_HEIGHT)
					{
						x2 -= (short)((m * (y2 - X3D_SCREEN_HEIGHT)) >> 11);
						y2 = X3D_SCREEN_HEIGHT;
					}
				}
				
				if (!X3D_ScreenTest(x1,y1)) // If the first point is still offscreen after correction, the whole line is offscreen
					return;
			}
			
			p1 = Plane + X3D_ByteOffset(x1,y1);
			p2=Plane+X3D_ByteOffset(x2,y2);
			mask1=0x80;
			mask2=0x80;
			X3D_ByteRotateR(mask1,X3D_ByteBitOffset(x1));
			X3D_ByteRotateR(mask2,X3D_ByteBitOffset(x2));
			dx=x2-x1;
			dy=y2-y1;
			l=dx>>1;
			testY=l-dx;
			if (dy<0) {dy=-dy;inc=-X3D_SCREEN_BYTESPERLINE;}
			else {inc=X3D_SCREEN_BYTESPERLINE;}
			do {
				if (Color)
				{
					*p1 |= mask1;
					*p2 |= mask2;
				}
				else
				{
					*p1 &= ~mask1;
					*p2 &= ~mask2;
				}
					
				X3D_ByteRotateR(mask1,1);
				if (mask1==0x80) p1++;
				X3D_ByteRotateL(mask2,1);
				if (mask2==0x01) p2--;
				testY+=dy;
				if (testY>0)
				{	testY-=dx;
					p1+=inc;
					p2-=inc;}
				l--;
			}while(l>=0);
		}
		else
		{
			if (y1>y2)
			{	X3D_Switch(x1,x2);
				X3D_Switch(y1,y2);}
			if (y1>X3D_SCREEN_HEIGHT || y2<0) return;
			if (!X3D_ScreenTest(x1,y1) || !X3D_ScreenTest(x2,y2)) {
			dx=x2-x1;dy=y2-y1;m=(((long)(dx))<<11)/dy;
			if (y1<0) {x1+=(short)((m*(-y1))>>11);y1=0;}
			if (y2>X3D_SCREEN_HEIGHT) {x2-=(short)((m*(y2-X3D_SCREEN_HEIGHT))>>11);y2=X3D_SCREEN_HEIGHT;}
			if (dx)
			{	m=(((long)(dy))<<11)/dx;
				if (x1<0) {y1+=(short)((m*(-x1))>>11);x1=0;}
				if (x1>X3D_SCREEN_WIDTH) {y1-=(short)((m*(x1-X3D_SCREEN_WIDTH))>>11);x1=X3D_SCREEN_WIDTH;}
				if (x2<0) {y2+=(short)((m*(-x2))>>11);x2=0;}
				if (x2>X3D_SCREEN_WIDTH) {y2-=(short)((m*(x2-X3D_SCREEN_WIDTH))>>11);x2=X3D_SCREEN_WIDTH;}}
			if (!X3D_ScreenTest(x1,y1)) return;}
			p1=Plane+X3D_ByteOffset(x1,y1);
			p2=Plane+X3D_ByteOffset(x2,y2);
			mask1=0x80;
			mask2=0x80;
			X3D_ByteRotateR(mask1,X3D_ByteBitOffset(x1));
			X3D_ByteRotateR(mask2,X3D_ByteBitOffset(x2));
			dx=x2-x1;
			dy=y2-y1;
			l=dy>>1;
			testY=l-dy;
			if (dx<0) {dx=-dx;inc=-1;}
			else {inc=1;}
			do {
				if (Color)
				{
					*p1 |= mask1;
					*p2 |= mask2;
				}
				else
				{
					*p1 &= ~mask1;
					*p2 &= ~mask2;
				}
				p1+=X3D_SCREEN_BYTESPERLINE;
				p2-=X3D_SCREEN_BYTESPERLINE;
				testY+=dx;
				if (testY>0)
				{	testY-=dy;
					if (inc==1)
					{	X3D_ByteRotateR(mask1,1);
						if (mask1==0x80) p1++;
						X3D_ByteRotateL(mask2,1);
						if (mask2==0x01) p2--;}
					else
					{	X3D_ByteRotateL(mask1,1);
						if (mask1==0x01) p1--;
						X3D_ByteRotateR(mask2,1);
						if (mask2==0x80) p2++;}}
				l--;
			}while(l>=0);
		}
}*/
