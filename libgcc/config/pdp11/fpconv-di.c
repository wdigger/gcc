/* Conversions between 64-bit integers and PDP-11 single precision.
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

/* See fpconv.h for why these exist at all.  libgcc2.c has its own
   versions, but they reach single precision by way of double, which on
   this target is the DEC D format and so is served by the same IEEE
   soft float that gets single precision wrong -- except that for D
   format, whose exponent is 8 bits wide against IEEE's 11, nothing
   cancels and the results are not merely scaled but meaningless.  These
   work on the integer directly and owe double nothing.  */

#include "fpconv.h"

/* Reduce M to 32 bits, storing into *SCALE how far it was shifted.  Any
   bits dropped set the low bit of the result, which is what lets the
   rounding in __pdp11_pack_f stand for the rounding the whole value
   calls for: a tie can only be a tie if nothing was lost below it.  */

static USItype
squash (UDItype m, int *scale)
{
  USItype hi = (USItype) (m >> 32);
  USItype lo = (USItype) m;
  USItype t, lost, r;
  int n;

  if (hi == 0)
    {
      *scale = 0;
      return lo;
    }

  /* N is the number of significant bits in the high word, and so also
     the shift that brings the whole value down to 32 bits.  */
  t = hi;
  n = 1;
  if (t >> 16)
    {
      n += 16;
      t >>= 16;
    }
  if (t >> 8)
    {
      n += 8;
      t >>= 8;
    }
  if (t >> 4)
    {
      n += 4;
      t >>= 4;
    }
  if (t >> 2)
    {
      n += 2;
      t >>= 2;
    }
  if (t >> 1)
    n += 1;

  lost = (n >= 32) ? lo : (lo & (((USItype) 1 << n) - 1));
  r = (USItype) (m >> n);
  if (lost)
    r |= 1;

  *scale = n;
  return r;
}

float
__floatdisf (DItype a)
{
  USItype m;
  int scale;

  if (a == 0)
    return 0.0f;
  if (a < 0)
    {
      m = squash (- (UDItype) a, &scale);
      return __pdp11_pack_f (1, m, scale);
    }
  m = squash ((UDItype) a, &scale);
  return __pdp11_pack_f (0, m, scale);
}

float
__floatundisf (UDItype a)
{
  USItype m;
  int scale;

  if (a == 0)
    return 0.0f;
  m = squash (a, &scale);
  return __pdp11_pack_f (0, m, scale);
}

/* Magnitude of X as a 64-bit integer, truncated towards zero.  Returns
   zero for anything below one, and saturates above the range, both of
   which C leaves undefined anyway.  */

static UDItype
unpack_di (float x, int *sign)
{
  union fbits u;
  USItype m;
  int e, shift;

  u.f = x;
  *sign = (int) (u.i >> 31);
  e = (int) ((u.i >> FRAC_BITS) & 0xff);

  if (e < EXP_BIAS)
    return 0;

  m = (u.i & (((USItype) 1 << FRAC_BITS) - 1)) | ((USItype) 1 << FRAC_BITS);
  shift = e - (EXP_BIAS + FRAC_BITS);

  if (shift > 40)
    return ~ (UDItype) 0;

  if (shift > 0)
    return (UDItype) m << shift;
  return (UDItype) m >> -shift;
}

DItype
__fixsfdi (float x)
{
  int sign;
  UDItype m = unpack_di (x, &sign);

  if (m >> 63)
    {
      /* Out of the signed range, or else exactly the most negative
	 integer, whose magnitude is the one this saturates to anyway.  */
      if (sign)
	return (DItype) ((UDItype) 1 << 63);
      return (DItype) (~ (UDItype) 0 >> 1);
    }
  return sign ? - (DItype) m : (DItype) m;
}

UDItype
__fixunssfdi (float x)
{
  int sign;
  UDItype m = unpack_di (x, &sign);

  if (sign)
    return 0;
  return m;
}
