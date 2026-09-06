/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -mbm2" } */

/* The hardware MUL is "MUL src,Rn": Rn must be *even*, holds one 16-bit
   multiplicand on entry, and receives the 32-bit product in Rn:Rn+1.
   mulhisi3 used to express that by tying its HImode operand to its
   SImode destination with a plain "%0,0" matching constraint; reload/LRA
   does not reliably honor a digit-matching constraint between operands
   of different sizes and could satisfy the tie by putting the operand
   in Rn+1 (the odd half of the destination pair), leaving Rn -- the
   register MUL actually reads -- uninitialized.  The product was then
   silently wrong whenever an operand's halves differed (masked for -1 *
   -1, where they don't).  The fix expands mulhisi3 explicitly: copy the
   operand into the low word of a fresh SImode pseudo, then let the real
   MUL reference that same register via match_dup/subreg.  -mbm2 for MUL
   (SUPP_INSN_MUL).  */

long mul_hisi (int a, int b)
{
  return (long) a * (long) b;
}

/* A 32-bit product must be produced into an even register (r0 in this
   leaf function), never into the odd half of the pair.  */
/* { dg-final { scan-assembler "mul\t\[^,\]*,r0" } } */
/* { dg-final { scan-assembler-not "mul\t\[^,\]*,r1" } } */
