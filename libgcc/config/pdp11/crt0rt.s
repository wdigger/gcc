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
