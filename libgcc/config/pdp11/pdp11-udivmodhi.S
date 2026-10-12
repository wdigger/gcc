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


/* 16-bit unsigned division for the PDP-11.

   DIV is signed, so a 16-bit unsigned divide cannot use it either: the
   loop below does sixteen shift-and-subtract steps with the dividend
   and quotient sharing R0, the remainder in R1 and the divisor in R2.
   The arguments are the compiler's own: two words on the stack, the
   result in R0.  Division by zero yields zero.  */

	.text

	.globl ___udivhi3
	.globl ___umodhi3

___udivhi3:
	mov	r2, -(sp)
	mov	r3, -(sp)

	mov	6(sp), r0	/* dividend */
	mov	8(sp), r2	/* divisor */
	beq	3f

	clr	r1
	mov	$16, r3

1:	asl	r0		/* dividend out at the top, quotient in */
	rol	r1		/* the bit that left enters the remainder */
	cmp	r1, r2
	blo	2f
	sub	r2, r1
	inc	r0
2:	sob	r3, 1b

	br	4f

3:	clr	r0		/* divisor is zero */
4:	mov	(sp)+, r3
	mov	(sp)+, r2
	rts	pc

___umodhi3:
	mov	r2, -(sp)
	mov	r3, -(sp)

	mov	6(sp), r0
	mov	8(sp), r2
	beq	3f

	clr	r1
	mov	$16, r3

1:	asl	r0
	rol	r1
	cmp	r1, r2
	blo	2f
	sub	r2, r1
	inc	r0
2:	sob	r3, 1b

	mov	r1, r0		/* the remainder is the answer */
	br	4f

3:	clr	r0
4:	mov	(sp)+, r3
	mov	(sp)+, r2
	rts	pc
