/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2" } */
/* Two types that pass their arguments differently are not compatible,
   and a variable argument list cannot start in registers at all.  */

extern int att (int a, int b) __attribute__((regargs));

int (*p) (int, int) = att;   /* { dg-error "incompatible pointer type" } */

extern int v (int n, ...) __attribute__((regargs)); /* { dg-error "variable argument list" } */
