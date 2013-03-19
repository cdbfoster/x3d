|void X3D_CopyScreen(void *src asm("%a0"),void *dst asm("%a1"));

|Copies 3000 bytes of src to dst

.text
.even

.globl X3D_CopyScreen
X3D_CopyScreen:
	movem.l	%d3-%d7/%a2-%a6,-(%sp)
	moveq	#9,%d7
0:
	movem.l	(%a0)+,%d0-%d6/%a2-%a6
	movem.l	%d0-%d6/%a2-%a6,(%a1)
	movem.l	(%a0)+,%d0-%d6/%a2-%a6
	movem.l	%d0-%d6/%a2-%a6,(48,%a1)
	movem.l	(%a0)+,%d0-%d6/%a2-%a6
	movem.l	%d0-%d6/%a2-%a6,(96,%a1)
	movem.l	(%a0)+,%d0-%d6/%a2-%a6
	movem.l	%d0-%d6/%a2-%a6,(144,%a1)
	movem.l	(%a0)+,%d0-%d6/%a2-%a6
	movem.l	%d0-%d6/%a2-%a6,(192,%a1)
	movem.l	(%a0)+,%d0-%d6/%a2-%a6
	movem.l	%d0-%d6/%a2-%a6,(240,%a1)
	lea	(%a1,288),%a1
	dbf	%d7,0b
	movem.l	(%a0)+,%d0-%d6/%a2-%a6
	movem.l	%d0-%d6/%a2-%a6,(%a1)
	movem.l	(%a0)+,%d0-%d6/%a2-%a6
	movem.l	%d0-%d6/%a2-%a6,(48,%a1)
	movem.l	(%a0)+,%d0-%d6
	movem.l	%d0-%d6,(96,%a1)
	movem.l	(%sp)+,%d3-%d7/%a2-%a6
	rts
