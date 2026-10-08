/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -mlra" } */

/* An asm that clobbers the stack pointer makes a frame pointer
   necessary, and under LRA that decision comes after the dataflow has
   stopped looking -- so r5 was never marked live, the prologue built a
   frame in it without saving it, and the epilogue returned without
   putting it back.  r5 is call-saved here, so the caller simply lost
   it.  */

extern void g (int *);

int
f (int n)
{
  int a[4];

  g (a);
  asm volatile ("" ::: "sp");
  return a[n & 3];
}

/* Saved on the way in and restored on the way out.  */
/* { dg-final { scan-assembler "mov\tr5,-\\(sp\\)" } } */
/* { dg-final { scan-assembler "mov\t\\(sp\\)\\+,r5" } } */
