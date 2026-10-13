/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2" } */
/* The attribute puts the register convention on a call the compiler
   cannot see whole: a function from another unit.  */

extern int att (int a, int b) __attribute__((regargs));
extern int plain (int a, int b);

int use (int x, int y)
{
  /* The attributed one takes its arguments in R0 and R1 ... */
  /* { dg-final { scan-assembler "mov\t0?6\\(sp\\),r1" } } */
  /* ... and the plain one has both of its pushed instead.  */
  /* { dg-final { scan-assembler "jsr\tpc,_?plain" } } */
  /* { dg-final { scan-assembler "mov\t0?6\\(sp\\),-\\(sp\\)" } } */
  return att (x, y) + plain (x, y);
}
