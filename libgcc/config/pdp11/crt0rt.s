    .TITLE CRT0RT shim
    .IDENT "V00.00"

    .text
    .GLOBAL _main
    .GLOBAL _parse_args

argv_max   = 16
cmdbuf_max = 80

    .GLOBAL start
start:
        mov   @$042, sp   // SP comes from the SAV header (RT-11 sets it before we start)

        // That header value is only a link-time guess (max(16896, this
        // program's own _end) -- see bfd/sav-pdp11.c's own
        // sav_pdp11_write_object_contents(), which has no idea how much
        // memory the resident monitor/drivers on the machine this actually
        // boots on leave free). .SETTOP (EMT 354) asks the monitor for the
        // *real* top of memory instead: R0 in is the requested address, R0
        // out is the actual highest legal address for this job -- never
        // higher than requested, silently clamped down if the request was
        // too big (RT-11 System Macro Library Manual, .SETTOP: "the address
        // returned will be the highest legal address for the job, not the
        // requested address"). Requesting the theoretical maximum
        // (0177776, the highest even address) and taking whatever comes
        // back is the documented way to discover that ceiling at runtime.
        //
        // Guarded with a compare rather than trusting the EMT result
        // outright: if .SETTOP's answer is somehow *not higher* than the
        // header value already in SP (e.g. this ROM's own monitor/drivers
        // turn out to leave less room than sav-pdp11.c assumed), keep the
        // link-time SP instead of moving backwards.
        mov   $0177776, r0
        emt   0354
        cmp   r0, sp
        blos  6$          // r0 <= sp (unsigned): keep the link-time SP
        mov   r0, sp
6$:

        // RT-11 doesn't clear memory between programs, so a previous job's
        // leftovers can still be sitting in our .bss; zero it before touching
        // any of our own statics (including cmdbuf/argv below).  __bss_start
        // and _end come from the linker script (ld/scripttempl/pdp11rt11.sc).
        mov   $__bss_start, r1
        mov   $_end, r2
4$:     cmp   r1, r2
        bge   5$
        clrb  (r1)+
        br    4$
5$:

        // RT-11's KMON, when it runs a program via RUN (or by typing its name
        // directly), copies whatever followed the program name on that command
        // into the fixed "chain area": a byte count at address 0510 and the
        // raw (not NUL-terminated) text itself starting at 0512 -- this
        // survives untouched into our own memory as long as our SAV file
        // doesn't set the CHAIN$ bit in the JSW (bit 8 of the word at 044),
        // which sav-pdp11.c never does.  (.CSISPC with CSTRNG=0 looks similar
        // but is a red herring here: per the RT-11 System Macro Library
        // Manual it only replays this text when the program was invoked
        // *through an indirect command file*; for a plain interactive RUN it
        // instead reprompts the terminal with "*", which is why an earlier
        // version of this file using that EMT never actually saw argv.)
        mov   @$0510, r2       // r2 = byte count of the invoking command line
        cmp   r2, $cmdbuf_max
        ble   1$
        mov   $cmdbuf_max, r2
1$:     mov   $0512, r1        // r1 = raw command-line text
        mov   $cmdbuf, r3      // r3 = our own copy, which we NUL-terminate
        tst   r2
        beq   3$
2$:     movb  (r1)+, (r3)+
        dec   r2
        bne   2$
3$:     clrb  (r3)

        mov   $argv_max, -(sp)
        mov   $argv, -(sp)
        mov   $cmdbuf, -(sp)
        jsr   pc, _parse_args
        add   $6, sp      // r0 now holds argc

        mov   $argv, -(sp)
        mov   r0, -(sp)
        jsr   pc, _main
        emt   0350	// .EXIT
        nop
    .even

    .GLOBAL ___main
___main:
        rts   pc
    .even

    .bss
cmdbuf: .skip cmdbuf_max+1
    .even          // argv holds words -- must not land on an odd address
argv:   .skip (argv_max+1)*2

    .end
