/* { dg-do compile { target pdp11-*-* } } */
/* { dg-options "-O2 -fomit-frame-pointer" } */

/* reload (not LRA) miscompiled a stack-passed pointer read right after
   a call to an external function while the call's pushed arguments
   were still outstanding (-fdefer-pop, on at every -O level): the
   spilled pseudo holding the pointer has a REG_EQUIV to its argument
   slot, and when that pseudo is used as an *indirect* address (pdp11
   has @n(sp)), find_reloads_address's "no reload needed" shortcut left
   it in place without flagging the insn for the final pass -- so the
   final pseudo-to-MEM substitution used reg_equiv_mem's initial-offset
   address, 2(sp), instead of the 6(sp) the two pushed words call for.
   The load then dereferenced the second call argument, not the
   pointer.  f must be external: a same-file definition changes
   register allocation and hides the bug.  */

extern int f (int a, int c);

__attribute__ ((noinline)) int
g (int *p, int a, int c)
{
  int r = f (a, c);
  int v = *p;
  if (r)
    {
      v += 4;
      *p = v;
    }
  return v & 15;
}

/* Right after the call, with both arguments still pushed, p is at
   6(sp) -- never at 2(sp), which is where the pushed c is.  */
/* { dg-final { scan-assembler "jsr\tpc,_f\n\tmov\t@06\\(sp\\)," } } */
/* { dg-final { scan-assembler-not "jsr\tpc,_f\n\tmov\t@02\\(sp\\)" } } */
/* After the deferred pop, the same slot is 2(sp) again (the store).  */
/* { dg-final { scan-assembler "add\t.04,sp" } } */
/* { dg-final { scan-assembler "@02\\(sp\\)" } } */
