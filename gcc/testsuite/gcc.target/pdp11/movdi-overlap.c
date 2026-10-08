/* { dg-do run { target pdp11-*-* } } */
/* { dg-options "-O2 -mlra" } */

/* A four-word load into a register group whose address register sits
   inside that group.  The word that lands in the base register has to
   be fetched last: with the words in order, the third one overwrites
   r2 and the fourth is then read through it.

   pdp11_expand_operands copes with the two-word case by choosing which
   end to start from; for four words the base can be in the middle,
   where no order works and output_move_multiple has to permute.  */

static long long mem = 0x0123456789abcdefLL;

/* Volatile, so the address has to be loaded at run time: given the
   symbol outright the compiler addresses it absolutely and the base
   register never appears.  */
static long long *volatile where = &mem;

long long __attribute__((noinline))
load (void)
{
  register long long *p asm ("r2");
  register long long v asm ("r0");

  /* Through an asm, so the address really is in r2 and nothing may
     re-materialise it somewhere more convenient.  */
  asm volatile ("mov\t%1,%0" : "=r" (p) : "r" (where));
  v = *p;
  asm volatile ("" : : "r" (v));
  return v;
}

int
main (void)
{
  return load () != 0x0123456789abcdefLL;
}
