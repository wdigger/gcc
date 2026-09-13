/* Comparisons of PDP-11 single precision values.
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

/* See fpconv.h for the general story.  fp-bit's comparisons get ordinary
   values right, since both formats order values the same way, but not the
   top octave of the range: an exponent field of 255 is a perfectly
   ordinary value near 1.7e38 in the DEC format, while fp-bit reads it as
   a NaN and then answers false to every question asked about it.  The
   format has no NaN and no infinity at all, so a comparison here is
   nothing more than sign and magnitude.  */

#include "fpconv.h"

/* Minus one, zero or one as X is less than, equal to or greater than Y.

   Every comparison entry point GCC can call is satisfied by this one
   answer: each asks its own question of the sign or the zeroness of the
   result, and with no unordered case to report there is nothing to tell
   them apart.  So they are aliases rather than six more copies.  */

CMPtype
__cmpsf2 (float x, float y)
{
  union fbits ux, uy;
  USItype a, b;

  ux.f = x;
  uy.f = y;
  a = ux.i;
  b = uy.i;

  /* A zero exponent is a zero whatever the rest of the word holds.  This
     format has no negative zero, and the one pattern the hardware would
     call a reserved operand is not worth inventing an answer for.  */
  if ((a & ((USItype) 0xff << FRAC_BITS)) == 0)
    a = 0;
  if ((b & ((USItype) 0xff << FRAC_BITS)) == 0)
    b = 0;

  if (a == b)
    return 0;

  /* Differing signs settle it.  */
  if ((a ^ b) >> 31)
    return (a >> 31) ? -1 : 1;

  /* Same sign, so the pattern runs with the magnitude, which for two
     negative values runs against the value.  */
  if (a >> 31)
    return (a > b) ? -1 : 1;
  return (a < b) ? -1 : 1;
}

CMPtype __eqsf2 (float, float) __attribute__ ((alias ("__cmpsf2")));
CMPtype __nesf2 (float, float) __attribute__ ((alias ("__cmpsf2")));
CMPtype __ltsf2 (float, float) __attribute__ ((alias ("__cmpsf2")));
CMPtype __lesf2 (float, float) __attribute__ ((alias ("__cmpsf2")));
CMPtype __gtsf2 (float, float) __attribute__ ((alias ("__cmpsf2")));
CMPtype __gesf2 (float, float) __attribute__ ((alias ("__cmpsf2")));

/* Nothing is ever unordered: there is no value in this format that is
   not a number.  */

CMPtype
__unordsf2 (float x __attribute__ ((unused)), float y __attribute__ ((unused)))
{
  return 0;
}
