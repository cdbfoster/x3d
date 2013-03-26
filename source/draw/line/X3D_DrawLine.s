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

| void X3D_DrawLine(void *Plane asm("%a0"), short x1 asm("%d0"), short y1 asm("%d1"), short x2 asm("%d2"), short y2 asm("%d3"), unsigned char Color);

.text
.even

.global X3D_DrawLine
X3D_DrawLine:
	movem.l	%d3 - %d7 / %a2, -(%sp)
	
	cmp.w	%d0, %d2
	bhi.s	NoExchange
	exg	%d0, %d2
	exg	%d1, %d3
	
NoExchange:
	move.w	(6 * 4 + 4, %sp), %d4	| Color parameter
	
	cmp.b	(PreviousState, %pc), %d4
	beq.s	PreviousStateOkay
	
	lea	(PreviousState, %pc), %a2
	move.b	%d4, (%a2)
	
	lea	(ColorData, %pc), %a1
	lsr.w	#1, %d4
	lsl.w	#2, %d4
	adda.w	%d4, %a1
	
	move.b	(%a1)+, %d4
	
	lea	(StateChangeAnchor, %pc), %a2
	move.b	%d4, (DrawLoop_DXGreater - StateChangeAnchor, %a2)	| Masking operator using %d6
	move.b	%d4, (DrawLoop_DYGreater - StateChangeAnchor, %a2)	|
	move.b	%d4, (DrawCenter - StateChangeAnchor + 4, %a2)		|
	addq.b	#2, %d4
	move.b	%d4, (DrawLoop_DXGreater - StateChangeAnchor + 2, %a2)	| Masking operator using %d7
	move.b	%d4, (DrawLoop_DYGreater - StateChangeAnchor + 2, %a2)	|
	move.b	(%a1)+, %d4
	move.b	%d4, (DrawLoop_DXGreater - StateChangeAnchor + 6, %a2)	| Branch condition for the left mask
	move.b	%d4, (DrawLoop_DYGreater - StateChangeAnchor + 20, %a2)	|
	move.b	%d4, (DrawLoop_DXGreater - StateChangeAnchor + 12, %a2)	| Branch condition for the right mask
	move.b	%d4, (DrawLoop_DYGreater - StateChangeAnchor + 26, %a2)	|
	move.w	(%a1), %d4
	move.w	%d4, (SetupPoints - StateChangeAnchor + 24, %a2)	| Mask setter for left mask
	addi.w	#0x0200, %d4
	move.w	%d4, (SetupPoints - StateChangeAnchor + 52, %a2)	| Mask setter for right mask
	
PreviousStateOkay:
	move.l	%a0, %a1

SetupPoints:
	move.w	%d1, %d4
	add.w	%d4, %d4
	move.w	%d4, %d5
	lsl.w	#4, %d4
	sub.w	%d5, %d4	| y1 * 30
	add.w	%d4, %a0	| Plane + (y1 * 30)
	move.w	%d0, %d4
	lsr.w	#3, %d4		| x1 / 8
	add.w	%d4, %a0	| Address1 = Plane + (y1 * 30) + (x1 /8)
	
	move.w	%d0, %d4
	andi.b	#7, %d4		| This instruction is 4 bytes long
	moveq	#-128, %d6
	ror.b	%d4, %d6	| Mask1
	
	move.w	%d3, %d4
	add.w	%d4, %d4
	move.w	%d4, %d5
	lsl.w	#4, %d4
	sub.w	%d5, %d4
	add.w	%d4, %a1
	move.w	%d2, %d4
	lsr.w	#3, %d4
	add.w	%d4, %a1	| Address2 = Plane + (y2 * 30) + (x2 / 8)
	
	move.w	%d2, %d4
	andi.b	#7, %d4		| This instruction is 4 bytes long
	moveq	#-128, %d7
	ror.b	%d4, %d7	| Mask2
	
	sub.w	%d0, %d2	| dx
	sub.w	%d1, %d3	| dy
	
	bpl.s	0f
	neg.w	%d3
	moveq	#-30, %d4
	bra.s	1f
0:
	moveq	#30, %d4	
1:
	cmp.w	%d3, %d2
	bcs.s	DYGreater	| dy > dx
	
|DXGreater:
	move.w	%d2, %d0
	subq.w	#1, %d0
	lsr.w	#1, %d0		| Number of times to loop without drawing the center
	move.w	%d2, %d1
	lsr.w	#1, %d1
	scc.b	%d5		| Set a byte of d5 if dx is even.
	sub.w	%d2, %d1	| %d1 = dx / 2 - dx
	
StateChangeAnchor:
	
DrawLoop_DXGreater:
	or.b	%d6, (%a0)
	or.b	%d7, (%a1)
	
	ror.b	#1, %d6
	bcc.s	0f
	addq.w	#1, %a0
0:
	rol.b	#1, %d7
	bcc.s	1f
	subq.w	#1, %a1
1:
	add.w	%d3, %d1
	bmi.s	2f
	sub.w	%d2, %d1
	add.w	%d4, %a0
	sub.w	%d4, %a1
2:
	dbf	%d0, DrawLoop_DXGreater
	
DrawCenter:
	tst.b	%d5
	beq.s	0f
	or.b	%d6, (%a0)
0:
	movem.l	(%sp)+, %d3 - %d7 / %a2
	rts

DYGreater:
	move.w	%d3, %d0
	subq.w	#1, %d0
	lsr.w	#1, %d0		| Number of times to loop without drawing the center
	move.w	%d3, %d1
	lsr.w	#1, %d1
	scc.b	%d5		| Set a byte of d5 if dy is even.
	sub.w	%d3, %d1
	
	lea	(DrawLoop_DYGreater + 18, %pc), %a2
	
	tst.w	%d5
	bpl.s	PositiveSlope
	
	move.w	#0xE31E, (%a2)		|	rol.b	#1, %d6
	move.w	#0x5348, (4, %a2)	|	subq.w	#1, %a0
	move.w	#0xE21F, (6, %a2)	|	ror.b	#1, %d7
	move.w	#0x5249, (10,%a2)	|	addq.w	#1, %a1
	
	exg	%a0, %a1
	exg	%d6, %d7
	bra.s	DrawLoop_DYGreater
	
PositiveSlope:

	move.w	#0xE21E, (%a2)		|	ror.b	#1, %d6
	move.w	#0x5248, (4, %a2)	|	addq.w	#1, %a0
	move.w	#0xE31F, (6, %a2)	|	rol.b	#1, %d7
	move.w	#0x5349, (10, %a2)	|	subq.w	#1, %a1

DrawLoop_DYGreater:
	or.b	%d6, (%a0)
	or.b	%d7, (%a1)
	
	lea	(30, %a0), %a0	| These instructions are each four bytes long
	lea	(-30, %a1), %a1	|
	add.w	%d2, %d1
	bmi.s	1f
	sub.w	%d3, %d1
	ror.b	#1, %d6
	bcc.s	0f
	addq.w	#1, %a0
0:	
	rol.b	#1, %d7
	bcc.s	1f
	subq.w	#1, %a1
1:
	dbf	%d0, DrawLoop_DYGreater
	bra.s	DrawCenter

ColorData:
	|X3D_COLORS_WHITE
	.byte	0xCD, 0x65	|	and.b	%d6, <ea>	,	bcs.x	<ea>
	.word	0x7C7F		|	moveq	#127, %d6
	
	|X3D_COLORS_BLACK
	.byte	0x8D, 0x64	|	or.b	%d6, <ea>	,	bcc.x	<ea>
	.word	0x7C80		|	moveq	#-128, %d6
	
PreviousState:
	.byte	0x03, 0x00	| Previous Color, Padding
