/* { dg-do run { target pdp11-*-rt11* } } */
/* { dg-options "-O2" } */

/* A nested function whose address escapes needs a trampoline, and this
   target built a broken one twice over.  The function's address was
   written over the jmp opcode rather than into the word after it, so
   the trampoline began with the address itself and ran whatever that
   decoded to.  And cse replaced the call target with the trampoline's
   own first word, 012704, because a call is (call (mem:HI addr)) and
   that is indistinguishable from a two-byte load of the same address --
   the call then went to a constant pool entry holding that number.  */

extern void abort (void);

static int apply (int (*f) (int), int v) { return f (v); }

static int outer (int n)
{
  int inner (int x) { return x + n; }
  return apply (inner, 2);
}

static int counted (int n)
{
  int total = 0;
  void add (int x) { total += x; }
  int bump (int x) { add (x); return x; }
  for (int i = 1; i <= n; i++)
    apply (bump, i);
  return total;
}

int main (void)
{
  if (outer (40) != 42)
    abort ();
  if (counted (5) != 15)
    abort ();
  return 0;
}
