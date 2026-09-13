/* Shared declarations for the PDP-11 single precision conversions.
   Copyright (C) 2000-2025 Free Software Foundation, Inc.

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

/* The generic soft float in fp-bit.c is hardwired to IEEE: fp-bit.h
   gives single precision an exponent bias of 127.  This target's float
   modes are the DEC formats instead (see pdp11_f_format in
   gcc/config/pdp11/pdp11.cc), where the bias is 128 over a fraction
   normalized to [0.5, 1), so one bit pattern stands for a value four
   times smaller than IEEE would read it as.

   In a linear operation that factor cancels, which is why fp-bit's
   addition and subtraction come out right anyway, and comparisons are
   fine because the ordering is the same in both formats.  Conversions do
   not get away with it: they build an exponent from scratch or take one
   apart, so fp-bit's versions are wrong by that factor of four in both
   directions and are replaced here.  libgcc/config/pdp11/t-pdp11
   excludes them from the fp-bit build.

   The compiler folds constants using the real format, so these routines
   have to agree with it bit for bit, rounding included: to nearest, ties
   to even.

   Comparisons have a narrower version of the same problem, described in
   fpconv-cmp.c.

   The 64-bit conversions live in their own file so that a program doing
   no more than an ordinary int to float conversion does not drag in
   64-bit shift and compare helpers along with them.  */

typedef int CMPtype __attribute__ ((mode (__libgcc_cmp_return__)));
typedef int SItype __attribute__ ((mode (SI)));
typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef int DItype __attribute__ ((mode (DI)));
typedef unsigned int UDItype __attribute__ ((mode (DI)));

/* A float and its bits.  The word holding the sign and the exponent is
   at the lower address in an F format value, and so is the high word of
   a 32-bit integer on this machine, so the exponent lands in the high
   bits here.  */
union fbits
{
  float f;
  USItype i;
};

/* Bits of fraction actually stored; the leading one is hidden.  */
#define FRAC_BITS 23

/* Exponent of a value whose significand is read as being in [1, 2).  The
   format's own bias is 128 over a fraction in [0.5, 1), which is the
   same statement differently phrased.  So 1.0 has exponent 129.  */
#define EXP_BIAS 129

/* Assemble the float for (-1)**SIGN * M * 2**SCALE.  M must not be zero.
   If bits were dropped on the way to M, its low bit must be set, which
   keeps the rounding decision here the one the full value calls for.  */
extern float __pdp11_pack_f (int, USItype, int);

/* fp-bit.c and libgcc2.c declare these through machinery of their own,
   which is not in play here, so declare them to keep
   -Wmissing-prototypes quiet.  */
extern float __floatsisf (SItype);
extern float __floatunsisf (USItype);
extern SItype __fixsfsi (float);
extern float __floatdisf (DItype);
extern float __floatundisf (UDItype);
extern DItype __fixsfdi (float);
extern UDItype __fixunssfdi (float);
extern CMPtype __cmpsf2 (float, float);
extern CMPtype __unordsf2 (float, float);
