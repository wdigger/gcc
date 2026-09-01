/* printf_float_stub.c -- default _printf_float for pdp11-uknc-rt11
 *
 * newlib-nano's vfprintf (libc/stdio/nano-vfprintf.c) declares
 * _printf_float weak and checks "if (_printf_float == NULL)" before
 * calling it, so that a program never formatting a float doesn't pay
 * for the real implementation (libc/stdio/nano-vfprintf_float.c, which
 * pulls in dtoa/mprec and pdp11's soft double-precision arithmetic --
 * measured at 36KB for this target).  GCC's pdp11 backend doesn't
 * support weak symbols at all ("warning: weak declaration of 'foo' not
 * supported"), so that reference silently becomes an ordinary strong
 * one, and the real implementation gets linked into EVERY program that
 * calls printf/fprintf, even one that only ever uses %d.
 *
 * Fix: provide this file's own _printf_float here, injected via
 * STARTFILE_SPEC (see pdp11.h, right next to crt0rt.o/parse_args.o) so
 * it appears on the link line before -lc is even scanned.  Static
 * archives are searched lazily -- once _printf_float is already
 * resolved by an object given explicitly on the command line, the
 * archive's own nano-vfprintf_float.o is simply never pulled in, and
 * dtoa.o/mprec.o/the soft-float routines go with it.  (A more surgical
 * `-Wl,--defsym=_printf_float=0` was tried first -- exploiting
 * nano-vfprintf.c's own NULL check directly, no stub function needed --
 * but measured to NOT prevent the archive's strong definition from
 * still being pulled in on this linker; an explicit object placed
 * ahead of the archive is what actually works, confirmed by comparing
 * the linker map before and after.)
 *
 * This body is exactly nano-vfprintf.c's own "_printf_float == NULL"
 * fallback path (consume the varargs float argument so later format
 * specifiers still read the right argument, print nothing for this
 * one) -- i.e. any real target where the weak reference legitimately
 * doesn't resolve behaves identically to this.
 *
 * A program that genuinely needs %f/%e/%g formatting can still get the
 * real implementation: link with -nostartfiles and list crt0rt.o and
 * parse_args.o (but not this file) explicitly, e.g.
 *   pdp11-uknc-rt11-gcc -nostartfiles -o prog.sav \
 *     `pdp11-uknc-rt11-gcc -print-file-name=crt0rt.o` \
 *     `pdp11-uknc-rt11-gcc -print-file-name=parse_args.o` \
 *     prog.o -lc -lgcc
 * which omits this stub, so the archive's own real _printf_float
 * resolves normally (at the usual ~36KB cost).
 */

#include <stdarg.h>
#include <stdio.h>

/* Opaque -- this file never dereferences a struct _reent *, only passes
   the pointer through/ignores it, so an incomplete type is enough and
   keeps this stub from depending on reent.h's own layout. */
struct _reent;

/* Mirrors nano-vfprintf_local.h's struct _prt_data_t: this file only
   ever reads its first member (flags), so only that member needs to
   line up -- guaranteed, since C never inserts padding before a
   struct's first member.  Not #include-ing the real (private,
   libc/stdio-internal) header to avoid depending on stdio's own build
   layout from outside it.  */
struct _prt_data_t_flags_only
{
  int flags;
};

#define LONGDBL 0x100 /* Must match nano-vfprintf_local.h's own value. */

int
_printf_float (struct _reent *data,
	       struct _prt_data_t_flags_only *pdata,
	       FILE *fp,
	       int (*pfunc) (struct _reent *, FILE *, const char *, size_t len),
	       va_list *ap)
{
  (void) data;
  (void) fp;
  (void) pfunc;
  if (pdata->flags & LONGDBL)
    (void) va_arg (*ap, long double);
  else
    (void) va_arg (*ap, double);
  return 0;
}
