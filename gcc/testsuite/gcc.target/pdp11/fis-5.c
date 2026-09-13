/* { dg-do compile { target pdp11-uknc-* } } */
/* { dg-options "-O2 -fomit-frame-pointer" } */

/* The UKNC emulates the FIS instruction codes in its own monitor, so the
   vendor has them on by default and no option is needed here.  -mfpu
   quietly wins over that default instead of colliding with it, which
   fis-6.c checks; an explicit -mfis alongside -mfpu is still an error,
   which fis-4.c checks.  */

float
mul (float a, float b)
{
  return a * b;
}

/* { dg-final { scan-assembler "fmul\t" } } */
/* { dg-final { scan-assembler-not "___mulsf3" } } */
