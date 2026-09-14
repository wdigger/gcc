/* { dg-do run { target pdp11-*-rt11* } } */
/* { dg-options "-O2" } */

/* Weak symbols, which a.out could not represent and ELF can.  The
   linker does the rest: an undefined weak reference resolves to zero
   rather than failing the link, and a strong definition overrides the
   weak one.  newlib leans on exactly this for _printf_float.  */

extern void abort (void);

int __attribute__((weak)) pick (void) { return 1; }
extern int absent (void) __attribute__((weak));

int main (void)
{
  if (pick () != 1)
    abort ();
  if (absent != 0)
    abort ();
  return 0;
}
