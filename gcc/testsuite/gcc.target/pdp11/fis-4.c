/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -mfis -mfpu" } */
/* { dg-error "mutually exclusive" "" { target pdp11-*-* } 0 } */

/* FIS and FPP are the two alternative floating point options of the
   PDP-11 line, never both on one machine, and they do not even agree on
   where a float lives.  Asking for both is an error rather than a silent
   choice of one of them.  */

float
mul (float a, float b)
{
  return a * b;
}
