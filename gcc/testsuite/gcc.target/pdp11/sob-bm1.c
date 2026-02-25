/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O1 -mbm1" } */
/* -O1 in the options is significant. Without it some operations 
 * may not be optimized to custom instructions. 
 * */

int test_sob01(int n) {
  /* { dg-final { scan-assembler "sob" } } */
    int c = 0;
    for(int i=0; i<n; i++)
        c += i;
    return c;
}

int test_sob02(int n) {
  /* { dg-final { scan-assembler "sob" } } */
    int c = 0, i = n;
    while(i)
        c += i--;
    return c;
}
