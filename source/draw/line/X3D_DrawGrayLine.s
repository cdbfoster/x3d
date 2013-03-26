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

| void X3D_DrawGrayLine(void *Plane1 asm("%a0"), void *Plane2 asm("%a1"), short x1 asm("%d0"), short y1 asm("%d1"), short x2 asm("%d2"), short y2 asm("%d3"), unsigned char Color);

.text
.even

.global X3D_DrawGrayLine
X3D_DrawGrayLine:
	movem.l	%d3 - %d5 / %a2, -(%sp)
	
	cmp.w	%d0, %d2
	bhi.s	NoExchange
	exg	%d0, %d2
	exg	%d1, %d3
	
NoExchange:
	move.w	(4 * 4 + 4, %sp), %d4	| Color parameter
	
	cmp.b	(PreviousState, %pc), %d4
	beq.s	PreviousStateOkay
	
	lea	(PreviousState, %pc), %a2
	move.b	%d4, (%a2)
	
	lea	(StateChangeAnchor, %pc), %a2
	
	move.b	#0x90, %d5	| Changes a bxxx %d5, <ea> instruction into a bclr %d5, (%a0) instruction
	lsr.b	#1, %d4
	bcc.s	0f
	move.b	#0xD0, %d5	| Changes a bxxx %d5, <ea> instruction into a bset %d5, (%a0) instruction
0:
	move.b	%d5, (DrawLoop_DXGreater - StateChangeAnchor + 1, %a2)
	move.b	%d5, (DrawLoop_DYGreater - StateChangeAnchor + 1, %a2)
	
	move.b	#0x91, %d5	| Changes a bxxx %d5, <ea> instruction into a bclr %d5, (%a1) instruction
	lsr.b	#1, %d4
	bcc.s	0f
	move.b	#0xD1, %d5	| Changes a bxxx %d5, <ea> instruction into a bset %d5, (%a1) instruction
0:
	move.b	%d5, (DrawLoop_DXGreater - StateChangeAnchor + 2 + 1, %a2)
	move.b	%d5, (DrawLoop_DYGreater - StateChangeAnchor + 2 + 1, %a2)

PreviousStateOkay:
	
SetupPoints:
	move.w	%d1, %d4	|
	add.w	%d4, %d4	|
	move.w	%d4, %d5	|
	lsl.w	#4, %d4		|
	sub.w	%d5, %d4	| y1 * 30
	move.w	%d0, %d5	|
	lsr.w	#3, %d5		| x1 / 8
	add.w	%d5, %d4	|
	add.w	%d4, %a0	| Address1 = Plane1 + (y1 * 30) + (x1 /8)
	add.w	%d4, %a1	| Address2 = Plane2 + (y1 * 30) + (x1 /8)
	
	move.w	%d0, %d5
	not.w	%d5
	andi.b	#7, %d5		| Bit number of the starting point's X value
	
	sub.w	%d0, %d2	| dx
	sub.w	%d1, %d3	| dy
	
	bpl.s	0f		| if dy >= 0
	neg.w	%d3
	moveq	#-30, %d4
	bra.s	1f
0:
	moveq	#30, %d4	
1:
	cmp.w	%d3, %d2
	bls.s	DYGreater
	
|DXGreater:
	move.w	%d2, %d0	| Number of times to loop is simply dx
	move.w	%d2, %d1	|
	lsr.w	#1, %d1		|
	sub.w	%d2, %d1	| %d1 = dx / 2 - dx
	
StateChangeAnchor:
DrawLoop_DXGreater:
	bset	%d5, (%a0)
	bset	%d5, (%a1)
	subq.b	#1, %d5
	bge.s	SameByte
	moveq	#7, %d5
	addq.w	#1, %a0
	addq.w	#1, %a1
SameByte:
	add.w	%d3, %d1
	bmi.s	SameRow
	sub.w	%d2, %d1
	add.w	%d4, %a0
	add.w	%d4, %a1
SameRow:
	dbf	%d0, DrawLoop_DXGreater

Exit:
	movem.l (%sp)+, %d3 - %d5 / %a2
	rts
	
DYGreater:
	move.w	%d3, %d0	| Number of times to loop is simply dy
	move.w	%d3, %d1	|
	lsr.w	#1, %d1		|
	sub.w	%d3, %d1	| %d1 = dy / 2 - dy

DrawLoop_DYGreater:
	bset	%d5, (%a0)
	bset	%d5, (%a1)
	add.w	%d4, %a0
	add.w	%d4, %a1
	add.w	%d2, %d1
	bmi.s	SameColumn
	sub.w	%d3, %d1
	subq.b	#1, %d5
	bge.s	SameByte2
	moveq	#7, %d5
	addq.w	#1, %a0
	addq.w	#1, %a1
SameByte2:
SameColumn:
	dbf	%d0, DrawLoop_DYGreater
	bra.s	Exit
	
PreviousState:
	.byte	0x03, 0x00	| Previous Color, Padding
