/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -mno-reg-args" } */
/* -mno-reg-args puts every argument back on the stack.  */

static int __attribute__((noinline,noclone)) loc (int a, int b)
{
  return a - b;
}

int use (int x, int y)
{
  /* { dg-final { scan-assembler "sub\t0?4\\(sp\\),r0" } } */
  /* { dg-final { scan-assembler-not "sub\tr1,r0" } } */
  return loc (x, y);
}
