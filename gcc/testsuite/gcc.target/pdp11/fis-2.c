/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -fomit-frame-pointer -mfis -mno-fis" } */

/* FIS is off unless asked for, and -mno-fis turns it back off again,
   because whether the four instruction codes do anything is a property
   of the machine and not of the processor: some machines emulate them in
   a monitor trap handler, and elsewhere nothing handles them at all.  So
   this has to come out as the ordinary library call.  */

float
mul (float a, float b)
{
  return a * b;
}

/* { dg-final { scan-assembler "jsr\tpc,___mulsf3" } } */
/* { dg-final { scan-assembler-not "fmul" } } */
