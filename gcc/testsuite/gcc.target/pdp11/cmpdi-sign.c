/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -fomit-frame-pointer" } */

/* A four-word compare walks the words from the most significant down and
   stops at the first one that differs.  Only that first word carries a
   sign, though: the ones below it are magnitudes, and an unsigned
   comparison of them reports its answer in C, where a signed branch does
   not look.  cmpsi has always turned C into N for its one lower word;
   cmpdi did not, so a positive value whose top word was zero and whose
   next word had its top bit set -- 0xd1cf7980, for one -- tested as
   negative.  That in turn made the soft float library get the subtraction
   of two nearly equal numbers wrong, since fp-bit settles the sign of
   such a result by testing a 64-bit difference against zero.

   Each compare below should come out as three branches down the words,
   the last three of them landing on the fixup and the first one going
   past it, since the most significant word's own flags are already the
   ones a signed branch wants.  */

int
gt (long long a)
{
  return a > 0;
}

int
ge2 (long long a, long long b)
{
  return a >= b;
}

/* Three branches down the words in each of the two.  */
/* { dg-final { scan-assembler-times "bne\t" 6 } } */
/* The fixup, once in each: the words are walked, then a label, then the
   clearing of N.  */
/* { dg-final { scan-assembler-times "cln" 2 } } */
/* { dg-final { scan-assembler "bne\t\[^\n\]*\n\ttst\t\[^\n\]*\n\tbne\t\[^\n\]*\n\ttst\t\[^\n\]*\n\tbne\t\[^\n\]*\n\ttst\t\[^\n\]*\n\[^\n\]*:\n\tcln" } } */
/* Against zero there is nothing to convert, tst having left C clear, so
   only the two-operand compare carries the rest of the fixup.  */
/* { dg-final { scan-assembler-times "clv" 1 } } */
/* { dg-final { scan-assembler-times "sen" 1 } } */
/* { dg-final { scan-assembler "cln\n\tclv\n\tbcc\t\[^\n\]*\n\tsen" } } */
