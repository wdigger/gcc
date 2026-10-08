/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2" } */

/* A device register is reached as a volatile bit-field more often than
   as anything else, and half of this machine's registers do not take a
   byte write: some latch on the write of a whole word, some refuse a
   byte access outright.  So an access to a volatile bit-field has to
   stay the width it was declared with -- a word here -- rather than be
   narrowed to the byte the bits happen to sit in.

   Two things together make that hold: PCC_BITFIELD_TYPE_MATTERS, so
   the field's container is aligned as the declared type, and
   -fstrict-volatile-bitfields on by default, so the middle-end keeps
   the access at that width.  */

struct reg
{
  volatile unsigned low : 4, high : 4, rest : 8;
};

void
set_field (struct reg *r)
{
  r->high = 5;
}

/* Read-modify-write of the whole word, and not a byte in sight.  */
/* { dg-final { scan-assembler "mov\t" } } */
/* { dg-final { scan-assembler "bic\t" } } */
/* { dg-final { scan-assembler "bis\t" } } */
/* { dg-final { scan-assembler-not "movb\t" } } */
/* { dg-final { scan-assembler-not "bicb\t" } } */
/* { dg-final { scan-assembler-not "bisb\t" } } */
