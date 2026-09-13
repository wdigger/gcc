/* { dg-do compile { target pdp11-uknc-* } } */
/* { dg-options "-O2 -fomit-frame-pointer -mfpu" } */

/* Where FIS is on by default, asking for the FP11 instead has to work
   rather than report a collision: a default is not a request.  So this
   compiles, and compiles to FP11 code -- double precision in the
   accumulators, since that is how this port drives an FP11.  */

float
mul (float a, float b)
{
  return a * b;
}

/* { dg-final { scan-assembler "muld\t" } } */
/* { dg-final { scan-assembler-not "fmul" } } */
