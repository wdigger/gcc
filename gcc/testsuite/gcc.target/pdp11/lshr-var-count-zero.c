/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -mbm2" } */

/* lshr<mode>3's HImode/SImode path for a *variable* count shifts right
   by one bit first (a single logical shift, to clear the sign bit
   safely), then does an arithmetic shift for the remaining "count - 1"
   bits.  That "count - 1" used to be computed unconditionally: at a
   runtime count of 0 it wraps to -1, and since ashr<mode>3 negates its
   shift amount before handing it to ASH, -1 turned into ASH by +1 -- a
   *left* shift by one bit, clearing the operand's low bit instead of
   leaving it untouched.  The fix guards the two-step trick with a
   runtime "count <= 0" compare-and-branch (cbranchhi4) that copies the
   operand through unshifted, the same convention pdp11_expand_shift's
   own base-machine loop path already used.  -mbm2 for ASH/ASHC.  */

unsigned int lshr_hi_var (unsigned int x, int n)
{
  return x >> n;
}

unsigned long lshr_si_var (unsigned long x, int n)
{
  return x >> n;
}

/* The count <= 0 guard: a TST of the count and a BLE around the shift,
   once per function above.  */
/* { dg-final { scan-assembler-times "tst\t" 2 } } */
/* { dg-final { scan-assembler-times "ble\t" 2 } } */
/* The remaining-bits shift itself: ASH for HImode, ASHC for SImode.  */
/* { dg-final { scan-assembler "ash\t" } } */
/* { dg-final { scan-assembler "ashc\t" } } */
