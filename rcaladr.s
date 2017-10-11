/* ################################################################# */
#
/* #       File Name  :  rcaladr.asm */
/* #       Functions  :  Search calling function address */
/* #       INPUT      :  Counter     [ebp+8] */
/* #                  :  SAVE AREA   [ebp+c] */
/* #       OUTPUT     :  calling function address */
/* #       Revision   :  T.Ishikawa(NEEC)1996.01.30 */
#
/* ################################################################# */
/* #.      text */
        .global rcaladr
rcaladr: 

        pushl   %ebp
        movl    %esp,%ebp
        pushl   %ebx
        pushl   %ecx
        pushl   %edx
        pushl   %esi

        movl    (%ebp),%ebx     /* # Set first EBP */
        movl    8(%ebp),%ecx    /* # COUNTER */
        movl    0xc(%ebp),%esi  /* # SAVE AREA ADR POINTER */
        jmp     START

/* ################################################################# */
#
/* #       Functions  :  Search calling function address 2 */
/* #       INPUT      :  Counter      [ebp+8] */
/* #                  :  SAVE AREA   4[ebp+8] */
/* #                  :  First EBP   8[ebp+8] */
/* #       OUTPUT     :  calling function address */
#
/* #       Revision   :  T.Ishikawa(NEEC)1996.01.30 */
#
/* ################################################################# */
        .global rcaladr2
rcaladr2: 

        pushl   %ebp
        movl    %esp,%ebp
        pushl   %ebx
        pushl   %ecx
        pushl   %edx
        pushl   %esi
        movl    8(%ebp),%ecx    /* # COUNTER */
        movl    0xc(%ebp),%esi  /* # SAVE AREA ADR POINTER */
        movl    0x10(%ebp),%ebx /* # Set first EBP */
START: 
        testl   %ecx,%ecx       /* # COUNTER check */
        jz      RETUR
        testl   %esi,%esi       /* # SAVE AREA check */
        jz      RETUR
        testl   %ebx,%ebx       /* # EBP check */
        jz      RETUR
LLOOP: 
/* #       SAVE EIP */
        movl    4(%ebx),%eax    /* # Read call address */
        movl    %eax,(%esi)     /* # Save call address */
        addl    $4,%esi
        decl    %ecx            /* # Del COUNTER */
        jz      RETUR
#
/* # Check EBP and EIP */
        movl    %ebx,%edx       /* # EBP copy */
        movl    (%ebx),%eax     /* # Next EBP load */
        cmpl    %edx,%eax       /* # Check next EBP  No1 */
        jbe     SARCH           /* # Next EBP is bad address */

        andl    $0xfffff000,%eax
        andl    $0xfffff000,%edx
        cmpl    %edx,%eax       /* # Check next EBP  No2 */
        jnz     SARCH                   /* #Next EBP is bad address */

        movl    (%ebx),%eax
        andl    $0xfff,%eax
        cmpl    $0xfe0,%eax     /* # Check next EBP  No3 */
        jae     RETUR           /* # EBP is Last EBP */

        movl    (%ebx),%edx     /* # Next EBP address is good */
        movl    4(%edx),%eax    /* # Load EIP */
        cmpl    $0x300000,%eax  /* # Check EIP over I/O area(EVU85-3284) */
        jbe     SARCH           /* # EIP is bad address */

        movl    (%ebx),%ebx     /* # Good EBP address */
        jmp     LLOOP

/* #       EBP or EIP is bad Number */
SARCH: 
        addl    $8,%ebx         /* # Set next EBP */
CHECK: 
        movl    %ebx,%edx
        movl    (%ebx),%eax
        cmpl    %edx,%eax       /* # Check EBP  No1 */
        jbe     NXEBP           /* # EBP is bad address */
        andl    $0xfffff000,%eax
        andl    $0xfffff000,%edx
        cmpl    %edx,%eax       /* # Check EBP  No2 */
        jnz     NXEBP                   /* #EBP is bad address */

        movl    4(%ebx),%eax
        cmpl    $0x300000,%eax  /* # Check EIP over I/O area(EVU85-3284) */
        jbe     NXEBP           /* # This is Not EIP */
        jmp     LLOOP           /* # OK EBP and EIP Let's Go to LLOOP */

NXEBP: 
        addl    $4,%ebx
        movl    %ebx,%edx
        andl    $0xfff,%edx     /* # Check EBP  No3 */
        cmpl    $0xfe0,%edx
        jae     RETUR           /* # EBP is Last EBP */
        jmp     CHECK

RETUR: 
        movl    4(%ebp),%eax    /* # Load Calling address */
        popl    %esi
        popl    %edx
        popl    %ecx
        popl    %ebx
        popl    %ebp
        ret



