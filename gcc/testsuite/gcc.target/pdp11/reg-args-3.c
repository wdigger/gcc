/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2" } */
/* Two kinds of local function that still take their arguments on the
   stack: one whose address is taken, because the indirect call cannot
   know anything else, and one with a variable argument list, which is
   walked as one block of stack.  */

static int __attribute__((noinline,noclone)) taken (int a, int b)
{
  return a - b;
}

static int __attribute__((noinline,noclone)) va (int n, ...)
{
  return n;
}

int (*volatile pf) (int, int) = taken;

int use (int x, int y)
{
  /* { dg-final { scan-assembler "sub\t0?4\\(sp\\),r0" } } */
  /* { dg-final { scan-assembler-not "sub\tr1,r0" } } */
  return taken (x, y) + pf (x, y) + va (x, y);
}
