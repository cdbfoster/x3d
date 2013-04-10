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
|
|	Copyright 2013 Chris Foster

| void X3D_FillScreen(void *Plane asm("%a0"));

| Fills 3000 bytes of plane

.text
.even

.global X3D_FillScreen
X3D_FillScreen:
	movem.l	%d3 - %d7 / %a2 - %a5, -(%sp)

	lea	(3000, %a0), %a0
	moveq	#-1, %d0
	moveq	#-1, %d1
	moveq	#-1, %d2
	moveq	#-1, %d3
	moveq	#-1, %d4
	moveq	#-1, %d5
	moveq	#-1, %d6
	movea.l	%d0, %a1
	movea.l	%d0, %a2
	movea.l	%d0, %a3
	movea.l	%d0, %a4
	movea.l	%d0, %a5
	moveq	#14, %d7
0:
	movem.l	%d0 - %d6 / %a1 - %a5, -(%a0)
	movem.l	%d0 - %d6 / %a1 - %a5, -(%a0)
	movem.l	%d0 - %d6 / %a1 - %a5, -(%a0)
	movem.l	%d0 - %d6 / %a1 - %a5, -(%a0)
	dbf	%d7, 0b

	movem.l	%d0 - %d6 / %a1 - %a5, -(%a0)
	movem.l	%d0 - %d6 / %a1 - %a5, -(%a0)
	movem.l	%d0 - %d5, -(%a0)

	movem.l	(%sp)+, %d3 - %d7 / %a2 - %a5
	rts
