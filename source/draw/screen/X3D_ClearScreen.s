|void X3D_ClearScreen(void *Plane asm("%a0"));

|Clears 3000 bytes of plane

.text
.even

.globl X3D_ClearScreen
X3D_ClearScreen:
	movem.l	%d3-%d7/%a2-%a5,-(%sp)
	lea	(0xBB8,%a0),%a0
	moveq	#0,%d0
	moveq	#0,%d1
	moveq	#0,%d2
	moveq	#0,%d3
	moveq	#0,%d4
	moveq	#0,%d5
	moveq	#0,%d6
	movea.l	%d0,%a1
	movea.l	%d0,%a2
	movea.l	%d0,%a3
	movea.l	%d0,%a4
	movea.l	%d0,%a5
	moveq	#14,%d7
0:
	movem.l	%d0-%d6/%a1-%a5,-(%a0)
	movem.l	%d0-%d6/%a1-%a5,-(%a0)
	movem.l	%d0-%d6/%a1-%a5,-(%a0)
	movem.l	%d0-%d6/%a1-%a5,-(%a0)
	dbf	%d7,0b
	movem.l	%d0-%d6/%a1-%a5,-(%a0)
	movem.l	%d0-%d6/%a1-%a5,-(%a0)
	movem.l	%d0-%d5,-(%a0)
	movem.l	(%sp)+,%d3-%d7/%a2-%a5
	rts
