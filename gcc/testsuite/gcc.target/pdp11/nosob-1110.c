/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O1 -m10" } */
/* -O1 in the options is significant. Without it some operations 
 * may not be optimized to custom instructions. 
 * */

int test_nosob01(int n) {
  /* { dg-final { scan-assembler "cmp\tr2,r1" } } */
    int c = 0;
    for(int i=0; i<n; i++)
        c += i;
    return c;
}

int test_nosob02(int n) {
  /* { dg-final { scan-assembler "tst\tr1" } } */
    int c = 0, i = n;
    while(i)
        c += i--;
    return c;
}
