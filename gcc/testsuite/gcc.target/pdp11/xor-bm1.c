/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O1 -mbm1" } */
/* -O1 in the options is significant. Without it some operations 
 * may not be optimized to custom instructions. 
 * */

int test_xor01(int n) {
  /* { dg-final { scan-assembler "xor" } } */
    return (n+1)^n;
}

