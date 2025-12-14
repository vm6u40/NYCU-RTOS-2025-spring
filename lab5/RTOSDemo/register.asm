		.include data_model.h
		.cdecls C,NOLIST, "msp430.h"

;commit 1

        .global  commit_register1
        .global  register1

commit_register1:   .asmfunc

        dint
        nop

		mov_x r1, register1 + 4*1
		mov_x r2, register1 + 4*2
		mov_x r3, register1 + 4*3
		mov_x r4, register1 + 4*4
		mov_x r5, register1 + 4*5
		mov_x r6, register1 + 4*6
		mov_x r7, register1 + 4*7
		mov_x r8, register1 + 4*8
		mov_x r9, register1 + 4*9
		mov_x r10, register1 + 4*10
		mov_x r11, register1 + 4*11
		mov_x r12, register1 + 4*12
		mov_x r13, register1 + 4*13
		mov_x r14, register1 + 4*14
		mov_x r15, register1 + 4*15

        ret_x


        .endasmfunc


;commit 2
        .global  commit_register2
        .global  register2

commit_register2:   .asmfunc

        dint
        nop

		mov_x r1, register2 + 4*1
		mov_x r2, register2 + 4*2
		mov_x r3, register2 + 4*3
		mov_x r4, register2 + 4*4
		mov_x r5, register2 + 4*5
		mov_x r6, register2 + 4*6
		mov_x r7, register2 + 4*7
		mov_x r8, register2 + 4*8
		mov_x r9, register2 + 4*9
		mov_x r10, register2 + 4*10
		mov_x r11, register2 + 4*11
		mov_x r12, register2 + 4*12
		mov_x r13, register2 + 4*13
		mov_x r14, register2 + 4*14
		mov_x r15, register2 + 4*15

        ret_x

        .endasmfunc



;restore1
        .global  restore_register1

restore_register1:   .asmfunc

        dint
        nop

		mov_x register1 + 4*1, r1
		mov_x register1 + 4*2, r2
		mov_x register1 + 4*3, r3
		mov_x register1 + 4*4, r4
		mov_x register1 + 4*5, r5
		mov_x register1 + 4*6, r6
		mov_x register1 + 4*7, r7
		mov_x register1 + 4*8, r8
		mov_x register1 + 4*9, r9
		mov_x register1 + 4*10, r10
		mov_x register1 + 4*11, r11
		mov_x register1 + 4*12, r12
		mov_x register1 + 4*13, r13
		mov_x register1 + 4*14, r14
		mov_x register1 + 4*15, r15

        ret_x


        .endasmfunc

;restore2
        .global  restore_register2

restore_register2:   .asmfunc

        dint
        nop

		mov_x register2 + 4*1, r1
		mov_x register2 + 4*2, r2
		mov_x register2 + 4*3, r3
		mov_x register2 + 4*4, r4
		mov_x register2 + 4*5, r5
		mov_x register2 + 4*6, r6
		mov_x register2 + 4*7, r7
		mov_x register2 + 4*8, r8
		mov_x register2 + 4*9, r9
		mov_x register2 + 4*10, r10
		mov_x register2 + 4*11, r11
		mov_x register2 + 4*12, r12
		mov_x register2 + 4*13, r13
		mov_x register2 + 4*14, r14
		mov_x register2 + 4*15, r15

        ret_x


        .endasmfunc

;power_off
        .global  power_off

power_off:   .asmfunc

        BIC #GIE, SR
        MOV.B #PMMPW_H, &PMMCTL0_H
        BIS.B #PMMREGOFF, &PMMCTL0_L
        BIC.B #SVSHE, &PMMCTL0_L
        MOV.B #000h, &PMMCTL0_H
        BIS #CPUOFF+OSCOFF+SCG0+SCG1, SR

        .endasmfunc

        .end
