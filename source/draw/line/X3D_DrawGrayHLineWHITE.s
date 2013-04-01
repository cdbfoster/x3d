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

| void X3D_DrawGrayHLineWHITE(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));

.text
.even

.include "../../api/internal/ScreenConstants.s"

.global X3D_DrawGrayHLineWHITE
X3D_DrawGrayHLineWHITE:
	move.l	%d3, -(%sp)
	cmp.w	%d0, %d1	| Make sure x1 is less than x2
	bge.s	NoExchange
	exg	%d0, %d1

NoExchange:
	tst.w	%d1		| Make sure the line is on-screen
	blt.s	Exit		|
	cmpi.w	#X3D_SCREEN_WIDTH, %d0	|
	bge.s	Exit		|

	tst.w	%d0		| Clip x1 if necessary
	bge.s	X1Valid		|
	moveq	#0, %d0		|

X1Valid:
	cmpi.w	#X3D_SCREEN_WIDTH, %d1	| Clip x2 if necessary
	blt.s	X2Valid		|
	move.w	#X3D_SCREEN_MAX_X, %d1	|

X2Valid:
	move.w	%d0, %d3
	lsr.w	#4, %d3			| %d3 = x1 / 16 - The word offset of the word containing x1
	move.w	%d3, %d2		| Copy the word number into %d2
	add.w	%d3, %d3		| %d3 = The byte offset of the word containing x1
	adda.w	%d3, %a0		| %a0 now starts at the word containing x1
	adda.w	%d3, %a1		| %a1 now starts at the word containing x1

	andi.w	#0xF, %d0			| Get the starting mask
	add.w	%d0, %d0			|
	move.w	(StartMask, %pc, %d0.w), %d0	|

	move.w	%d1, %d3			| Save x2

	andi.w	#0xF, %d1			| Get the ending mask
	add.w	%d1, %d1			|
	move.w	(EndMask, %pc, %d1.w), %d1	|

	addq.w	#1, %d2
	lsl.w	#4, %d2			| %d2 = 16 * (x1 / 16) + 16
	sub.w	%d2, %d3		| %d3 = x2 - (16 * (x1 / 16) + 16) - The number of pixels left to draw after the starting mask
	bge.s	DrawStart		| Branch if there are remaining pixels after the starting mask.
	or.w	%d0, %d1		| If the number of remaining pixels after the starting mask is less than zero, the end of the line is before the starting mask ends, so the starting and ending masks must be combined.
	and.w	%d1, (%a0)		| Draw the combined mask
	and.w	%d1, (%a1)		|
	move.l	(%sp)+, %d3		| Restore %d3
	rts

DrawStart:
	and.w	%d0, (%a0)+	| Draw the starting mask
	and.w	%d0, (%a1)+	|
	moveq	#0, %d0
	move.w	#32, %d2
	sub.w	%d2, %d3
	blt.s	WrapUp		| If there are less than 32 pixels left to draw

DrawLoop:
	move.l	%d0, (%a0)+	| Draw 32 pixels
	move.l	%d0, (%a1)+	|
	sub.w	%d2, %d3
	bge.s	DrawLoop	| Loop while there are more than 32 pixels left to draw

WrapUp:
	cmpi.w	#-16, %d3
	blt.s	DrawEnd
	move.w	%d0, (%a0)+
	move.w	%d0, (%a1)+

DrawEnd:
	and.w	%d1, (%a0)	| Draw the ending mask
	and.w	%d1, (%a1)	|

Exit:
	move.l	(%sp)+, %d3	| Restore d3
	rts

StartMask:
	.word	0x0000
	.word	0x8000
	.word	0xC000
	.word	0xE000
	.word	0xF000
	.word	0xF800
	.word	0xFC00
	.word	0xFE00
	.word	0xFF00
	.word	0xFF80
	.word	0xFFC0
	.word	0xFFE0
	.word	0xFFF0
	.word	0xFFF8
	.word	0xFFFC
	.word	0xFFFE

EndMask:
	.word	0x7FFF
	.word	0x3FFF
	.word	0x1FFF
	.word	0x0FFF
	.word	0x07FF
	.word	0x03FF
	.word	0x01FF
	.word	0x00FF
	.word	0x007F
	.word	0x003F
	.word	0x001F
	.word	0x000F
	.word	0x0007
	.word	0x0003
	.word	0x0001
	.word	0x0000
