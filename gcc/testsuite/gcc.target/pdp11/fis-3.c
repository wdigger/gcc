/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -fomit-frame-pointer -mfis" } */

/* A constant operand is stored into the FIS scratch area as two words,
   the sign and exponent word first because that is the lower address in
   the PDP-11 single precision format.  1.5 is 040300 0, and 3.0 is
   040500 0; getting this backwards would be the easy mistake, and the
   FIS result would then be meaningless.  */

float
addc (float a)
{
  return a + 1.5f;
}

float
mulc (float a)
{
  return a * 3.0f;
}

/* { dg-final { scan-assembler "mov\t.040300,\\(sp\\)\n\tclr\t02\\(sp\\)" } } */
/* { dg-final { scan-assembler "mov\t.040500,\\(sp\\)\n\tclr\t02\\(sp\\)" } } */
/* { dg-final { scan-assembler "fadd\t" } } */
/* { dg-final { scan-assembler "fmul\t" } } */
