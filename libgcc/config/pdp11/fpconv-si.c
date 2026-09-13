/* Conversions between 32-bit integers and PDP-11 single precision.
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

/* See fpconv.h for why these exist at all.  */

#include "fpconv.h"

float
__pdp11_pack_f (int sign, USItype m, int scale)
{
  union fbits u;
  USItype t, q;
  int n, e;

  /* N is the number of significant bits in M, so that M is M / 2**N,
     which lies in [0.5, 1), times 2**N.  */
  t = m;
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

  e = n + scale + EXP_BIAS - 1;

  /* Line the value up so that the hidden one is bit 23.  */
  if (n <= FRAC_BITS + 1)
    q = m << (FRAC_BITS + 1 - n);
  else
    {
      int shift = n - (FRAC_BITS + 1);
      USItype half = (USItype) 1 << (shift - 1);
      USItype rem = m & ((half << 1) - 1);

      q = m >> shift;
      if (rem > half || (rem == half && (q & 1) != 0))
	{
	  q++;
	  /* The carry can reach a 25th bit, which means the value rounded
	     up to the next power of two.  */
	  if (q >> (FRAC_BITS + 1))
	    {
	      q >>= 1;
	      e++;
	    }
	}
    }

  u.i = ((USItype) (sign != 0) << 31) | ((USItype) e << FRAC_BITS)
	| (q & (((USItype) 1 << FRAC_BITS) - 1));
  return u.f;
}

float
__floatsisf (SItype a)
{
  if (a == 0)
    return 0.0f;
  if (a < 0)
    return __pdp11_pack_f (1, - (USItype) a, 0);
  return __pdp11_pack_f (0, (USItype) a, 0);
}

float
__floatunsisf (USItype a)
{
  if (a == 0)
    return 0.0f;
  return __pdp11_pack_f (0, a, 0);
}

SItype
__fixsfsi (float x)
{
  union fbits u;
  USItype m;
  int e, shift;

  u.f = x;
  e = (int) ((u.i >> FRAC_BITS) & 0xff);

  /* A zero, a reserved operand, and anything of magnitude below one all
     truncate to zero.  */
  if (e < EXP_BIAS)
    return 0;

  m = (u.i & (((USItype) 1 << FRAC_BITS) - 1)) | ((USItype) 1 << FRAC_BITS);
  shift = e - (EXP_BIAS + FRAC_BITS);

  /* Out of range, which C leaves undefined; saturate.  The one value
     this rejects that is in fact representable, the most negative
     integer, saturates to itself.  */
  if (shift >= 8)
    return (u.i >> 31) ? (SItype) (USItype) 0x80000000 : (SItype) 0x7fffffff;

  if (shift > 0)
    m <<= shift;
  else
    m >>= -shift;

  return (u.i >> 31) ? - (SItype) m : (SItype) m;
}
