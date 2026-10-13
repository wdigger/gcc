/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2" } */
/* The attribute is part of the type, so a pointer carries it and the
   indirect call passes in registers too -- which the automatic rule can
   never do, a function whose address is taken not being local.  */

static int __attribute__((noinline,noclone,regargs)) taken (int a, int b)
{
  return a - b;
}

int (*volatile pf) (int, int) __attribute__((regargs)) = taken;

int use (int x, int y)
{
  /* { dg-final { scan-assembler "sub\tr1,r0" } } */
  /* { dg-final { scan-assembler "jsr\tpc,@_?pf" } } */
  /* { dg-final { scan-assembler-not "mov\tr\[01\],-\\(sp\\)" } } */
  return pf (x, y);
}
