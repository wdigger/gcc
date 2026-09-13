/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -mbm2" } */

/* Byte shifts by four or more are done by widening to a word and handing
   that to ASH, which shifts the whole word; -mbm2 because ASH is what the
   1801BM2 has.  Three things have gone wrong there, one after another,
   and each was silently wrong code rather than a build failure:

   ASH shifts left for a positive count and right for a negative one, so a
   right shift has to negate the count.  The arithmetic shift did; the
   logical one, copied from it, did not, and b >> 4 compiled as b << 4.

   Then the widening: an arithmetic shift needs the byte's sign carried
   into the upper half, and a logical shift needs zeros there.  The
   arithmetic path widened with zeros, so every negative value came back
   shifted logically -- (signed char) -91 >> 4 giving 10 rather than -6.

   And the logical path, which did widen with zeros, had them taken away
   again by the optimizers: told the shift was a left shift by a negative
   count, they concluded the upper half could not matter and replaced the
   widening with a plain byte load, which on this machine extends the
   sign.  So it is not enough to widen correctly, the RTL has to say what
   is really happening -- which is why the logical path now hands the
   widened value to the HImode logical shift rather than to ASH itself,
   and pays two instructions for it.  */

unsigned char lshr_qi4 (unsigned char b)
{
  return b >> 4;
}

unsigned char lshr_qi7 (unsigned char b)
{
  return b >> 7;
}

signed char ashr_qi4 (signed char b)
{
  return b >> 4;
}

/* The two logical shifts widen with zeros, and that has to survive to the
   assembly: a movb into a register would bring the sign along with it.  */
/* { dg-final { scan-assembler-times "clr\tr0\n\tbisb\t" 2 } } */

/* The arithmetic one wants the sign, and movb is how this machine brings
   it.  */
/* { dg-final { scan-assembler "movb\t\[^\n\]*,r0\n\tash\t.-04,r0" } } */

/* Every one of them shifts right, never left.  */
/* { dg-final { scan-assembler-times "ash\t.-0" 3 } } */
/* { dg-final { scan-assembler-not "ash\t.0?\[0-9\]+,r0" } } */
