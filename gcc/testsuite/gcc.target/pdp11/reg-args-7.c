/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2" } */
/* The C library's memory and string functions pass their first two
   words in registers wherever the name appears, with no attribute and
   no declaration in sight: the compiler calls them itself, from units
   that may never have declared them.  */

void *fill (void *p, unsigned n)
{
  /* { dg-final { scan-assembler "clr\tr1" } } */
  /* { dg-final { scan-assembler "jsr\tpc,_?memset" } } */
  return __builtin_memset (p, 0, n);
}

unsigned len (const char *s)
{
  /* The only argument goes in R0, and nothing is pushed for it.  */
  /* { dg-final { scan-assembler "mov\t0?2\\(sp\\),r0" } } */
  /* { dg-final { scan-assembler "jsr\tpc,_?strlen" } } */
  /* { dg-final { scan-assembler-not "mov\t0?2\\(sp\\),-\\(sp\\)" } } */
  return __builtin_strlen (s);
}
