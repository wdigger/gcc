/* { dg-do compile { target pdp11-*-rt11* } } */
/* { dg-options "-O1 -g -dA" } */

/* Debug information, which this target could not produce at all while
   its objects were a.out: there was nowhere to put it, and -g came back
   as "target system does not support debug output".  Addresses in it
   are two bytes, the size of a pointer here.

   The four-byte fields DWARF is full of -- unit lengths, offsets into
   other debug sections -- go out as .long, which in an ELF file is a
   plain little-endian four-byte datum.  That is deliberately not the
   order this machine stores a C long in (high word first); see
   TARGET_ASM_UNALIGNED_SI_OP in pdp11.cc, and include/elf/pdp11.h in
   binutils.  A C long still comes out as two .word directives, which is
   what the second check here is about.  */

long wide = 0x12345678L;

int add (int a, int b)
{
  return a + b;
}

/* { dg-final { scan-assembler "\\.section\t\\.debug_info" } } */
/* { dg-final { scan-assembler "\\.section\t\\.debug_line" } } */
/* { dg-final { scan-assembler "\\.long" } } */
/* { dg-final { scan-assembler-not "\\.long\t0x12345678" } } */
