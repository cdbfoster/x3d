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
