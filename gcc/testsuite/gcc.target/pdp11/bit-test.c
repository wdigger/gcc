/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2" } */

/* BIT computes src & dst for the condition codes and writes nothing
   back, so a bit test needs neither a temporary register nor the
   complemented mask that BIC (this machine's only AND) would want.
   All three shapes a bit test arrives in have to reach it: a mask of
   several bits, a mask held in a variable, and -- the one that took a
   second pattern, because combine canonicalises it into a zero_extract
   -- a mask of exactly one bit.  */

extern void f (void);

void
bits_several (unsigned x)
{
  if (x & 0101)
    f ();
}

void
bits_variable (unsigned x, unsigned m)
{
  if (x & m)
    f ();
}

void
bits_one (unsigned x)
{
  if (x & 0100)
    f ();
}

/* The shape this is really for: waiting on a device register.  */
void
bits_poll (volatile unsigned *r)
{
  while (!(*r & 0400))
    ;
}

/* { dg-final { scan-assembler-times "bit\t" 4 } } */
/* { dg-final { scan-assembler-not "bic\t" } } */
