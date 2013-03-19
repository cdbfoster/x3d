|void X3D_DrawGrayHLineLGRAY(short *Address1 asm("%a0"), short *Address2 asm("%a1"), short x1 asm("%d0"), short x2 asm("%d1"));

.text
.even

.globl X3D_DrawGrayHLineLGRAY
X3D_DrawGrayHLineLGRAY:
	move.l	%d3,-(%sp)	|Save d3
	cmp.w	%d0,%d1		|Make sure x1 is less than x2
	bge.s	0f
	exg	%d0,%d1
0:
	tst.w	%d0		|Make sure x1>=0
	bge.s	1f
	moveq	#0,%d0
1:
	cmpi.w	#160,%d1	|Make sure x2<160
	blt.s	2f
	move.w	#159,%d1
2:
	move.w	%d0,%d3
	lsr.w	#4,%d3		|d3=x1/16 - The word offset of the word containing x1
	move.w	%d3,%d2		|Copy the word number into d2
	add.w	%d3,%d3		|d3= The byte offset of the word containing x1
	adda.w	%d3,%a0
	adda.w	%d3,%a1		|a0 and a1 now start at the word containing x1
	addq.w	#1,%d2
	lsl.w	#4,%d2		|d2=16*(x1/16)+16
	andi.w	#0xF,%d0	|Get the starting mask
	add.w	%d0,%d0
	move.w	Start(%pc,%d0.w),%d0
	move.w	%d1,%d3		|Save x2
	andi.w	#0xF,%d1	|Get the ending mask
	add.w	%d1,%d1
	move.w	End(%pc,%d1.w),%d1
	sub.w	%d2,%d3		|d3=x2-(16*(x1/16)+16) - The number of pixels left to draw after the starting mask
	bge.s	3f		|Branch if there are remaining pixels after the starting mask.
	and.w	%d0,%d1		|If the number of remaining pixels after the starting mask is less than zero, the end of the line is before the starting mask ends, i.e. the starting and ending masks must be combined.
	or.w	%d1,(%a0)	|Draw the combined mask
	not.w	%d1
	and.w	%d1,(%a1)
	move.l	(%sp)+,%d3	|Restore d3
	rts
3:	
	or.w	%d0,(%a0)+	|Draw the starting mask
	not.w	%d0
	and.w	%d0,(%a1)+
	move.l	#0xFFFFFFFF,%d0
	move.w	#32,%d2
	sub.w	%d2,%d3		
	blt.s	5f
4:
	move.l	%d0,(%a0)+	|Draw 32 pixels
	clr.l	(%a1)+
	sub.w	%d2,%d3
	bge.s	4b		|Loop until it's time to draw the ending mask
5:
	cmpi.w	#-16,%d3
	blt.s	6f
	move.w	%d0,(%a0)+
	clr.w	(%a1)+
6:
	or.w	%d1,(%a0)	|Draw the ending mask
	not.w	%d1
	and.w	%d1,(%a1)
	move.l	(%sp)+,%d3	|Restore d3
	rts

Start:
	dc.w	0xFFFF
	dc.w	0x7FFF
	dc.w	0x3FFF
	dc.w	0x1FFF
	dc.w	0x0FFF
	dc.w	0x07FF
	dc.w	0x03FF
	dc.w	0x01FF
	dc.w	0x00FF
	dc.w	0x007F
	dc.w	0x003F
	dc.w	0x001F
	dc.w	0x000F
	dc.w	0x0007
	dc.w	0x0003
	dc.w	0x0001
	
End:
	dc.w	0x8000
	dc.w	0xC000
	dc.w	0xE000
	dc.w	0xF000
	dc.w	0xF800
	dc.w	0xFC00
	dc.w	0xFE00
	dc.w	0xFF00
	dc.w	0xFF80
	dc.w	0xFFC0
	dc.w	0xFFE0
	dc.w	0xFFF0
	dc.w	0xFFF8
	dc.w	0xFFFC
	dc.w	0xFFFE
	dc.w	0xFFFF
