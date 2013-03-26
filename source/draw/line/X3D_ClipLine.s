|	This file is part of X3D Engine.
|
|	X3D Engine is free software: you can redistribute it and/or modify
|	it under the terms of the GNU Lesser General Public License as published by
|	the Free Software Foundation, either version 3 of the License, or
|	(at your option) any later version.
|
|	X3D Engine is distributed in the hope that it will be useful,
|	but WITHOUT ANY WARRANTY; without even the implied warranty of
|	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|	GNU Lesser General Public License for more details.
|
|	You should have received a copy of the GNU Lesser General Public License
|	along with X3D Engine.  If not, see <http://www.gnu.org/licenses/>.

| X3D_RESULT X3D_ClipLine(short x1 asm("%d2"), short y1 asm("%d3"), short x2 asm("%d0"), short y2 asm("%d1"), short *ClippedCoordinates asm("%a0"));

.text
.even

.set X3D_SCREEN_WIDTH,	159
.set X3D_SCREEN_HEIGHT,	99

.global X3D_ClipLine
X3D_ClipLine:
	movem.l	%d3 - %d7, -(%sp)
	
	bsr	GetClippingCode		| Just barely missed the opportunity to use a short branch here... sigh.
	exg	%d0, %d2
	exg	%d1, %d3
	exg	%d4, %d5
	bsr.s	GetClippingCode
	move.l	%d4, %d6
	and.l	%d5, %d6
	bne	LineRejected		| If both points are clipped by the same side, reject the line
	
CheckClippingCodes:
	tst.l	%d4
	bne.s	ClipBottom
	tst.l	%d5
	beq	LineAccepted
	exg	%d0, %d2
	exg	%d1, %d3
	exg	%d4, %d5
	
ClipBottom:
	tst.b	%d4
	beq.s	ClipTop
	move.w	%d2, %d6
	sub.w	%d0, %d6
	move.w	%d1, %d7
	subi.w	#X3D_SCREEN_HEIGHT, %d7
	muls.w	%d7, %d6
	sub.w	%d3, %d1
	divs.w	%d1, %d6
	add.w	%d6, %d0
	move.w	#X3D_SCREEN_HEIGHT, %d1
	bsr.s	GetClippingCode
	bra.s	CheckClippingCodes

ClipTop:
	tst.w	%d4
	bpl.s	ClipLeft		| We check Left before Right because there's no nice way to test the third byte of d4.
	move.w	%d2, %d6
	sub.w	%d0, %d6
	muls.w	%d1, %d6
	sub.w	%d3, %d1
	divs.w	%d1, %d6
	add.w	%d6, %d0
	clr.w	%d1
	bsr.s	GetClippingCode
	bra.s	CheckClippingCodes

ClipLeft:
	tst.l	%d4
	bpl.s	ClipRight
	move.w	%d3, %d6
	sub.w	%d1, %d6
	muls.w	%d0, %d6		| d0 is negative
	sub.w	%d2, %d0
	divs.w	%d0, %d6
	add.w	%d6, %d1
	clr.w	%d0
	bsr.s	GetClippingCode
	bra.s	CheckClippingCodes

ClipRight:
	
	move.w	%d3, %d6
	sub.w	%d1, %d6		| dy
	move.w	%d0, %d7
	subi.w	#X3D_SCREEN_WIDTH, %d7	| x1 - ScreenWidth
	muls.w	%d7, %d6		| (x1 - ScreenWidth) * dy
	sub.w	%d2, %d0		| dx
	divs.w	%d0, %d6		| (x1 - ScreenWidth) * (dy / dx)
	add.w	%d6, %d1		| y1 += (x1 - ScreenWidth) * (dy / dx)
	move.w	#X3D_SCREEN_WIDTH, %d0	| x1 = ScreenWidth
	bsr.s	GetClippingCode
	bra.s	CheckClippingCodes

GetClippingCode:	
	| Gets the Cohen-Sutherland clipping code for (d0, d1) and stores it in d4
	| Code Format:
	| Left Clip	FF 00 00 00
	| Right Clip	00 FF 00 00
	| Top Clip	00 00 FF 00
	| Bottom Clip	00 00 00 FF

	tst.w	%d0				| Test Left Clip
	smi.b	%d4
	move.b	%d4, -(%sp)			| Swap bytes of the first word of d4 by taking advantage of the 
	move.w	(%sp)+, %d4			| fact that the stack is word-aligned.
	cmpi.w	#X3D_SCREEN_WIDTH + 1, %d0	| Test Right Clip
	sge.b	%d4
	swap.w	%d4
	tst.w	%d1				| Test Top Clip
	smi.b	%d4
	move.b	%d4, -(%sp)
	move.w	(%sp)+, %d4
	cmpi.w	#X3D_SCREEN_HEIGHT + 1, %d1	| Test Bottom Clip
	sge.b	%d4
	rts

LineRejected:
	movem.w	%d0 - %d3, (%a0)
	moveq	#-1, %d0		| Return value of X3D_FAILURE
	bra.s	Exit
	
LineAccepted:
	movem.w	%d0 - %d3, (%a0)
	moveq	#0, %d0			| Return value of X3D_SUCCESS
	
Exit:
	movem.l	(%sp)+, %d3 - %d7
	rts
