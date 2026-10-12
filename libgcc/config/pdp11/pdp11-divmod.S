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

/* The machine's own DIV divides 32 bits by 16 and is signed, so
   anything wider or unsigned is a subroutine.  The four entry points
   below share one loop: thirty-two shift-and-subtract steps, with the
   dividend and the quotient in the same register pair -- the dividend
   leaves R1 at the top as the quotient's bits enter at the bottom --
   the running remainder in R4:R5 and the divisor in R2:R3.  Six
   registers, nothing spilled, and no normalisation pass, which is what
   makes this several times faster than the generic C in udivmodsi4.c.

   The ABI is the compiler's own for a two-word type: the arguments are
   on the stack, high word first, and the result comes back in R0:R1
   with the high word in R0.  A word of operation type is pushed by the
   entry point: bit 0 asks for the remainder rather than the quotient,
   bit 8 says the operation is unsigned.

   Division by zero yields zero.  C leaves it undefined and the machine
   has no trap for it.  */

	.text

	.globl ___divsi3
	.globl ___modsi3
	.globl ___udivsi3
	.globl ___umodsi3

/* On entry, past the pushed type word:
     14(sp) dividend high   16(sp) dividend low
     18(sp) divisor high    20(sp) divisor low  */

___divsi3:
	clr	-(sp)		/* quotient, signed */
	br	.Ldivmod

___modsi3:
	mov	$1, -(sp)	/* remainder, signed */
	br	.Ldivmod

___udivsi3:
	mov	$256, -(sp)	/* quotient, unsigned */
	br	.Ldivmod

___umodsi3:
	mov	$257, -(sp)	/* remainder, unsigned */

.Ldivmod:
	mov	r2, -(sp)
	mov	r3, -(sp)
	mov	r4, -(sp)
	mov	r5, -(sp)
	mov	$32, -(sp)	/* step counter */

	mov	16(sp), r1	/* R0:R1 = dividend */
	mov	14(sp), r0
	bpl	1f		/* positive */
	tstb	11(sp)		/* unsigned? */
	bne	2f
	neg	r1		/* no: take its absolute value */
	adc	r0
	neg	r0
	incb	1(sp)		/* and remember to flip the quotient */
	br	2f
1:	bne	2f		/* high word is not zero */
	tst	r1
	beq	9f		/* dividend is zero: the answer is zero */
2:	mov	20(sp), r3	/* R2:R3 = divisor */
	mov	18(sp), r2
	bpl	3f
	tstb	11(sp)
	bne	4f
	neg	r3
	adc	r2
	neg	r2
	incb	1(sp)
	br	4f
3:	bne	4f
	tst	r3
	beq	9f		/* divisor is zero: the answer is zero */
4:	clr	r4		/* R4:R5 = running remainder */
	clr	r5

5:	asl	r1		/* dividend out at the top, quotient in */
	rol	r0
	rol	r5		/* the bit that left enters the remainder */
	rol	r4
	cmp	r4, r2		/* remainder >= divisor? */
	blo	7f
	bne	6f
	cmp	r5, r3
	blo	7f
6:	sub	r3, r5		/* yes: subtract and set the quotient bit */
	sbc	r4
	sub	r2, r4
	inc	r1
7:	decb	(sp)
	bne	5b

	tstb	10(sp)		/* quotient wanted? */
	beq	8f
	mov	r5, r1		/* no: the remainder is the answer */
	mov	r4, r0
	tstb	11(sp)
	bne	10f		/* unsigned: it has no sign to fix */
	tst	14(sp)		/* signed: the remainder takes the sign of
				   the dividend, whose high word this is --
				   not 16(sp), the low one, which says
				   nothing about the sign of the pair */
	bpl	10f
	neg	r1
	adc	r0
	neg	r0
	br	10f
8:	rorb	1(sp)		/* quotient: flip it if the signs differed */
	bcc	10f
	neg	r1
	adc	r0
	neg	r0
	br	10f

9:	clr	r0
	clr	r1

10:	tst	(sp)+		/* drop the counter */
	mov	(sp)+, r5
	mov	(sp)+, r4
	mov	(sp)+, r3
	mov	(sp)+, r2
	tst	(sp)+		/* and the operation type */
	rts	pc
