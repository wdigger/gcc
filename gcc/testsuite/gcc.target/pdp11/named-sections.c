/* { dg-do compile { target pdp11-*-rt11* } } */
/* { dg-options "-O2 -ffunction-sections -fdata-sections" } */

/* The rt11 OS's objects are ELF, so a section can have a name, and both
   these options work rather than being refused with a warning -- which
   is what the a.out format left the compiler saying, since it has
   .text, .data and .bss and no way to name a fourth.  The point of them
   is --gc-sections, which on a machine with 64K of address space is
   worth having.  */

int keep_me (int x)
{
  return x + 1;
}

int drop_me (int x)
{
  return x * 3;
}

int writable = 7;
const char readable[] = "x";

/* { dg-final { scan-assembler "\\.section\t\\.text\\.keep_me" } } */
/* { dg-final { scan-assembler "\\.section\t\\.text\\.drop_me" } } */
/* { dg-final { scan-assembler "\\.section\t\\.data\\.writable" } } */
/* { dg-final { scan-assembler "\\.section\t\\.rodata\\.readable" } } */
