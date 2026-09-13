/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -mbm2" } */

/* A 16-bit rotate is ASHC on an odd register, that being the machine's
   own idiom: for an odd register the 32-bit operand is the register
   twice over, so shifting it rotates.  The instruction was emitted with
   "; limit 2261" after it, in the belief that ";" starts a comment.  It
   does not: for this assembler ";" separates statements, so what
   followed was read as an instruction and the assembly failed outright.
   Any program whose compilation produced a rotate could not be built --
   and that includes ones with no rotate written in them, a modulo by a
   constant being enough.  */

unsigned short rot (unsigned short v)
{
  return (unsigned short) ((v >> 3) | (v << 13));
}

unsigned short mod (unsigned short v)
{
  return v % 320;
}

/* { dg-final { scan-assembler "ashc\t" } } */
/* { dg-final { scan-assembler-not ";" } } */
