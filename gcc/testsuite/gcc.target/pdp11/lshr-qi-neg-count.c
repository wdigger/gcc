/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -mbm2" } */

/* lshr<mode>3's QImode path (shift counts >= 4, i.e. past
   pdp11_small_shift's inline fast path) zero-extends to HImode and emits
   a single ASH.  ASH shifts left for a positive count and right for a
   negative one, so a logical *right* shift must negate the count first
   -- ashr<mode>3's own QImode case already did, lshr<mode>3's copy of it
   didn't, and `b >> 4` silently compiled as `b << 4` (same instruction
   shape, same operand count, no build error).  -mbm2: the 1801BM2 has
   ASH (SUPP_INSN_ASH), which is what this path emits.  */

unsigned char lshr_qi4 (unsigned char b)
{
  /* { dg-final { scan-assembler "ash\t.-04,r0" } } */
  return b >> 4;
}

unsigned char lshr_qi7 (unsigned char b)
{
  /* { dg-final { scan-assembler "ash\t.-07,r0" } } */
  return b >> 7;
}

/* Neither shift may come out as a left shift (positive ASH count).  */
/* { dg-final { scan-assembler-not "ash\t.0?4,r0" } } */
/* { dg-final { scan-assembler-not "ash\t.0?7,r0" } } */
