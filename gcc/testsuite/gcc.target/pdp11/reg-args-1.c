/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2" } */
/* A function this unit can see every call to takes its first two words
   of arguments in R0 and R1, so the body works on the registers and
   reads nothing off the stack.  */

static int __attribute__((noinline,noclone)) loc (int a, int b)
{
  return a - b;
}

extern int ext (int a, int b);

int use (int x, int y)
{
  /* { dg-final { scan-assembler "sub\tr1,r0" } } */
  /* { dg-final { scan-assembler-not "sub\t0?4\\(sp\\),r0" } } */
  /* The external one cannot be seen, so it keeps the stack
     convention.  */
  /* { dg-final { scan-assembler "jsr\tpc,_?ext" } } */
  return loc (x, y) + ext (x, y);
}
