/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O1 -mbm1a" } */
/* -O1 in the options is significant. Without it some operations 
 * may not be optimized to custom instructions. 
 * */

volatile int m1, m2, m3;
volatile long l1, l2;
volatile unsigned int u1, u2, u3;

volatile unsigned long ul1, ul2;

int test_mul01() {
  /* { dg-final { scan-assembler "__mulhi3" } } */
    m1 = m2 * m3 ;
    m3 = __pdp11_model;
}
int test_mul02() {
  /* { dg-final { scan-assembler "dec\tr2" } } */
    m3 = m1 * 1234 ;    
}
int test_mul06() {
  /* { dg-final { scan-assembler "mov\t.017,r1" } } */
  /* { dg-final { scan-assembler "asl\tr0" } } */
    m3 = m2 << 15;
}
int test_mul07() {
  /* { dg-final { scan-assembler "asl\tr1" } } */
  /* { dg-final { scan-assembler "dec\tr0" } } */
    m2 = m1 << m3;
}
int test_div10() {
  /* { dg-final { scan-assembler "asr\tr0" } } */
    m3 = m2 >> 15;
}
int test_div11() {
  /* { dg-final { scan-assembler "asr\tr1" } } */
    m2 = m1 >> m3;
} 
int test_div12() {
  /* { dg-final { scan-assembler "___divhi3" } } */
    m1 = m2 / m1;
}
int test_div13() {
  /* { dg-final { scan-assembler "___udivhi3" } } */
    u1 = u2 / u3;    
}
int test_ne14() {
  /* { dg-final { scan-assembler "ror\tr0" } } */
    m3 = (m3 != m1);
}
int test_eq15() {
  /* { dg-final { scan-assembler "cmp\tr2,r1" } } */
    return (m1 == m2);
}

