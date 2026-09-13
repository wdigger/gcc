/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -fomit-frame-pointer -mfis" } */

/* -mfis turns the four single precision operations into FIS
   instructions instead of calls to the library.  FIS addresses its two
   operands through a general register, the right one at (R) and the
   left one at (R)+4, so each operation lays them out in a stack
   temporary first.  */

float
add (float a, float b)
{
  return a + b;
}

float
sub (float a, float b)
{
  return a - b;
}

float
mul (float a, float b)
{
  return a * b;
}

float
divide (float a, float b)
{
  return a / b;
}

/* { dg-final { scan-assembler "fadd\t" } } */
/* { dg-final { scan-assembler "fsub\t" } } */
/* { dg-final { scan-assembler "fmul\t" } } */
/* { dg-final { scan-assembler "fdiv\t" } } */
/* Eight bytes of scratch, and the pointer loaded from the stack
   pointer, in each of the four.  */
/* { dg-final { scan-assembler-times "add\t.-010,sp" 4 } } */
/* { dg-final { scan-assembler-times "mov\tsp,r" 4 } } */
/* No library call is left for any of them.  */
/* { dg-final { scan-assembler-not "___addsf3" } } */
/* { dg-final { scan-assembler-not "___subsf3" } } */
/* { dg-final { scan-assembler-not "___mulsf3" } } */
/* { dg-final { scan-assembler-not "___divsf3" } } */
