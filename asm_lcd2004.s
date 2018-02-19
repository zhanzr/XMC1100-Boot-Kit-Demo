		THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.DB4_Wr||, CODE, READONLY, ALIGN=2
			
XMC_GPIO_MODE_OUTPUT_PUSH_PULL	EQU 0x80
XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN	EQU 0xC0
PORT0_BASE EQU      0x40040000
PORT1_BASE EQU      0x40040100	
RS_PIN	EQU	0
RW_PIN	EQU	1
E_PIN	EQU	2
	
DB4_Wr PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        MOVS     r0,#8
        ANDS     r0,r0,r4
        CMP      r0,#0
        BNE      D4_High_DB4
        NOP      
        MOVS     r1,#8
        LDR      r0,XMC_GPIO_PORT0_DB4W
        BL       XMC_GPIO_SetOutputLow
        NOP      
        B        |L0.40|
D4_High_DB4
        NOP      
        MOVS     r1,#8
        LDR      r0,XMC_GPIO_PORT0_DB4W
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        NOP      
|L0.40|
        MOVS     r0,#4
        ANDS     r0,r0,r4
        CMP      r0,#0
        BNE      D3_High_DB4
        NOP      
        MOVS     r1,#0
        LDR      r0,XMC_GPIO_PORT1_DB4W
        BL       XMC_GPIO_SetOutputLow
        NOP      
        B        |L0.76|
D3_High_DB4
        NOP      
        MOVS     r1,#0
        LDR      r0,XMC_GPIO_PORT1_DB4W
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        NOP      
|L0.76|
        MOVS     r0,#2
        ANDS     r0,r0,r4
        CMP      r0,#0
        BNE      D2_High_DB4
        NOP      
        MOVS     r1,#1
        LDR      r0,XMC_GPIO_PORT1_DB4W
        BL       XMC_GPIO_SetOutputLow
        NOP      
        B        |L0.112|
D2_High_DB4
        NOP      
        MOVS     r1,#1
        LDR      r0,XMC_GPIO_PORT1_DB4W
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        NOP      
|L0.112|
        LSLS     r0,r4,#31
        LSRS     r0,r0,#31
        CMP      r0,#0
        BNE      D0_High_DB4
        NOP      
        MOVS     r1,#9
        LDR      r0,XMC_GPIO_PORT0_DB4W
        BL       XMC_GPIO_SetOutputLow
        NOP      
        B        exit_DB4
D0_High_DB4
        NOP      
        MOVS     r1,#9
        LDR      r0,XMC_GPIO_PORT0_DB4W
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        NOP      
exit_DB4
        POP      {r4,pc}
        ENDP

XMC_GPIO_PORT0_DB4W DCD      PORT0_BASE
XMC_GPIO_PORT1_DB4W DCD      PORT1_BASE	

        AREA ||i.E_H||, CODE, READONLY, ALIGN=2

E_H PROC
        PUSH     {lr}
        MOVS     r1,#E_PIN
        LDR      r0,XMC_GPIO_PORT0_EH
        BL       XMC_GPIO_SetOutputHigh
        POP      {pc}
        ENDP

XMC_GPIO_PORT0_EH DCD      PORT0_BASE


        AREA ||i.E_L||, CODE, READONLY, ALIGN=2

E_L PROC
        PUSH     {lr}
        MOVS     r1,#E_PIN
        LDR      r0,XMC_GPIO_PORT0_EL
        BL       XMC_GPIO_SetOutputLow
        POP      {pc}
        ENDP

XMC_GPIO_PORT0_EL DCD      PORT0_BASE


        AREA ||i.LCD_Initialize||, CODE, READONLY, ALIGN=2

LCD_Initialize PROC
        PUSH     {r4,lr}
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_PUSH_PULL
        MOVS     r1,#RS_PIN
        LDR      r0,XMC_GPIO_PORT0_LCDINIT
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_PUSH_PULL
        MOVS     r1,#RW_PIN
        LDR      r0,XMC_GPIO_PORT0_LCDINIT
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_PUSH_PULL
        MOVS     r1,#E_PIN
        LDR      r0,XMC_GPIO_PORT0_LCDINIT
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#9
        LDR      r0,XMC_GPIO_PORT0_LCDINIT
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#1
        LDR      r0,XMC_GPIO_PORT1_LCDINIT
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#0
        LDR      r0,XMC_GPIO_PORT1_LCDINIT
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#8
        LDR      r0,XMC_GPIO_PORT0_LCDINIT
        BL       XMC_GPIO_SetMode
		
        BL       E_L
		
        MOVS     r0,#0x28
        BL       LCD_WrCmd_4
		
        MOVS     r0,#8
        BL       LCD_WrCmd_4
		
        MOVS     r0,#1
        BL       LCD_WrCmd_4
		
        MOVS     r0,#6
        BL       LCD_WrCmd_4
		
        MOVS     r0,#0xc
        BL       LCD_WrCmd_4
		
        BL       LCD_PWM_VO_Init
        POP      {r4,pc}
        ENDP

XMC_GPIO_PORT0_LCDINIT DCD      PORT0_BASE
XMC_GPIO_PORT1_LCDINIT DCD      PORT1_BASE	

        AREA ||i.LCD_SetPos||, CODE, READONLY, ALIGN=2

LCD_SetPos PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        LDR      r1,POS_TAB_ADDR
        LDRB     r1,[r1,r4]
        ADDS     r1,r1,r5
        UXTB     r0,r1
        BL       LCD_WrCmd_4
        POP      {r4-r6,pc}
        ENDP

POS_TAB_ADDR        DCD      pos_tab

        AREA ||i.LCD_WaitAvail||, CODE, READONLY, ALIGN=2

LCD_WaitAvail PROC
        PUSH     {r4,lr}
        MOVS     r0,#0xff
        BL       DB4_Wr
        BL       RS_L
        NOP      
        MOVS     r1,#1
        LDR      r0,|L5.60|
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        BL       E_H
        B        |L5.32|
|L5.30|
        NOP      
|L5.32|
        NOP      
        MOVS     r0,#8
        LDR      r1,|L5.60|
        LDR      r1,[r1,#0x24]
        LSRS     r1,r1,r0
        LSLS     r1,r1,#31
        LSRS     r1,r1,#31
        NOP      
        CMP      r1,#0
        BNE      |L5.30|
        BL       E_L
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L5.60|
        DCD      0x40040000

        AREA ||i.LCD_WrCmd_4||, CODE, READONLY, ALIGN=1

LCD_WrCmd_4 PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        BL       LCD_WaitAvail
        NOP      
        NOP      
        BL       RS_L
        BL       RW_L
        LSRS     r0,r4,#4
        BL       DB4_Wr
        BL       E_H
        BL       E_L
        NOP      
        NOP      
        MOV      r0,r4
        BL       DB4_Wr
        BL       E_H
        BL       E_L
        POP      {r4,pc}
        ENDP


        AREA ||i.LCD_WrDat_4||, CODE, READONLY, ALIGN=2

LCD_WrDat_4 PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        BL       LCD_WaitAvail
        NOP      
        NOP      
        NOP      
        MOVS     r1,#RS_PIN
        LDR      r0,XMC_GPIO_PORT0_WD4
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        BL       RW_L
        LSRS     r0,r4,#4
        BL       DB4_Wr
        BL       E_H
        BL       E_L
        NOP      
        NOP      
        MOV      r0,r4
        BL       DB4_Wr
        BL       E_H
        BL       E_L
        POP      {r4,pc}
        ENDP

XMC_GPIO_PORT0_WD4 DCD      PORT0_BASE


        AREA ||i.LCD_displayL||, CODE, READONLY, ALIGN=1

LCD_displayL PROC
        PUSH     {r4-r6,lr}
        MOV      r5,r0
        MOV      r6,r1
        MOV      r4,r2
        MOV      r1,r6
        MOV      r0,r5
        BL       LCD_SetPos
        B        |L8.26|
|L8.18|
        LDRB     r0,[r4,#0]
        BL       LCD_WrDat_4
        ADDS     r4,r4,#1
|L8.26|
        LDRB     r0,[r4,#0]
        CMP      r0,#0
        BNE      |L8.18|
        POP      {r4-r6,pc}
        ENDP


        AREA ||i.RS_L||, CODE, READONLY, ALIGN=2

RS_L PROC
        PUSH     {r4,lr}
        MOVS     r1,#RS_PIN
        LDR      r0,XMC_GPIO_PORT0_RSL
        BL       XMC_GPIO_SetOutputLow
        POP      {r4,pc}
        ENDP

XMC_GPIO_PORT0_RSL DCD      PORT0_BASE


        AREA ||i.RW_L||, CODE, READONLY, ALIGN=2

RW_L PROC
        PUSH     {r4,lr}
        MOVS     r1,#RW_PIN
        LDR      r0,XMC_GPIO_PORT0_RWL
        BL       XMC_GPIO_SetOutputLow
        POP      {r4,pc}
        ENDP
			
XMC_GPIO_PORT0_RWL DCD      PORT0_BASE


        AREA ||i.XMC_GPIO_SetOutputHigh||, CODE, READONLY, ALIGN=1

XMC_GPIO_SetOutputHigh PROC
        MOVS     r2,#1
        LSLS     r2,r2,r1
        STR      r2,[r0,#4]
        BX       lr
        ENDP


        AREA ||i.XMC_GPIO_SetOutputLow||, CODE, READONLY, ALIGN=1

XMC_GPIO_SetOutputLow PROC
        MOVS     r2,#1
        LSLS     r2,r2,#16
        LSLS     r2,r2,r1
        STR      r2,[r0,#4]
        BX       lr
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.constdata||, DATA, READONLY, ALIGN=0

pos_tab
        DCB      0x80,0xc0,0x94,0xd4

        EXPORT LCD_Initialize [CODE]
        EXPORT LCD_SetPos [CODE]
        EXPORT LCD_WaitAvail [CODE]
        EXPORT LCD_WrCmd_4 [CODE]
        EXPORT LCD_WrDat_4 [CODE]
        EXPORT LCD_displayL [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT XMC_GPIO_SetMode [CODE]
        IMPORT LCD_PWM_VO_Init [CODE]

        KEEP DB4_Wr
        KEEP E_H
        KEEP E_L
        KEEP RS_L
        KEEP RW_L
        KEEP XMC_GPIO_SetOutputHigh
        KEEP XMC_GPIO_SetOutputLow
        KEEP pos_tab

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,6
        ATTR SETSTRING Tag_conformance,"2.09"
        ATTR SETVALUE AV,18,1

        ASSERT {ENDIAN} = "little"
        ASSERT {INTER} = {TRUE}
        ASSERT {ROPI} = {FALSE}
        ASSERT {RWPI} = {FALSE}
        ASSERT {IEEE_FULL} = {FALSE}
        ASSERT {IEEE_PART} = {FALSE}
        ASSERT {IEEE_JAVA} = {FALSE}
        END
