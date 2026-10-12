/* 32-bit integer division for the PDP-11.
   Copyright (C) 2026 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */


/* Single-precision negation and the widening to double, for the PDP-11.

   Both are a matter of moving words about: negation flips the sign bit
   of the DEC F format, and a float widened to the D format is the same
   two words with two zero words below them.  The result goes where the
   hidden pointer ahead of the arguments says, which is how the compiler
   returns a value of either type.  */

	.text

	.globl ___negsf2
	.globl ___extendsfdf2

/* 2(sp) result pointer   4(sp) a high   6(sp) a low  */

___negsf2:
	mov	4(sp), r1
	mov	$0100000, r0
	xor	r0, r1		/* flip the sign bit */
	mov	2(sp), r0
	mov	r1, (r0)
	mov	6(sp), 2(r0)
	rts	pc

/* 6(sp) result pointer   8(sp) a high   10(sp) a low, past the saves  */

___extendsfdf2:
	mov	r0, -(sp)
	mov	r1, -(sp)

	mov	6(sp), r0	/* where the double goes */
	mov	8(sp), (r0)+
	mov	10(sp), (r0)+
	clr	(r0)+		/* the two words a float does not have */
	clr	(r0)+

	mov	(sp)+, r1
	mov	(sp)+, r0
	rts	pc
