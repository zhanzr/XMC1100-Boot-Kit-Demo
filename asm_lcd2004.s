		THUMB
        REQUIRE8
        PRESERVE8

        AREA    |.text|, CODE, READONLY, ALIGN=2
			
XMC_GPIO_MODE_OUTPUT_PUSH_PULL	EQU 0x80
XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN	EQU 0xC0
PORT0_BASE EQU      0x40040000
PORT1_BASE EQU      0x40040100	
RS_PIN	EQU	0
RW_PIN	EQU	1
E_PIN	EQU	2
DB4_PIN	EQU	9
DB5_PIN	EQU	1
DB6_PIN	EQU	0
DB7_PIN	EQU	8
	
DB4_Wr PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        MOVS     r0,#8
        ANDS     r0,r0,r4
        CMP      r0,#0
        BNE      D4_High_DB4
        NOP      
        MOVS     r1,#DB7_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputLow
        NOP      
        B        JUDGE_BIT2
D4_High_DB4
        NOP      
        MOVS     r1,#DB7_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        NOP      
JUDGE_BIT2
        MOVS     r0,#4
        ANDS     r0,r0,r4
        CMP      r0,#0
        BNE      D3_High_DB4
        NOP      
        MOVS     r1,#DB6_PIN
        LDR      r0,XMC_GPIO_PORT1
        BL       XMC_GPIO_SetOutputLow
        NOP      
        B        JUDGE_BIT1
D3_High_DB4
        NOP      
        MOVS     r1,#DB6_PIN
        LDR      r0,XMC_GPIO_PORT1
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        NOP      
JUDGE_BIT1
        MOVS     r0,#2
        ANDS     r0,r0,r4
        CMP      r0,#0
        BNE      D2_High_DB4
        NOP      
        MOVS     r1,#DB5_PIN
        LDR      r0,XMC_GPIO_PORT1
        BL       XMC_GPIO_SetOutputLow
        NOP      
        B        JUDGE_BIT0
D2_High_DB4
        NOP      
        MOVS     r1,#DB5_PIN
        LDR      r0,XMC_GPIO_PORT1
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        NOP      
JUDGE_BIT0
        LSLS     r0,r4,#31
        LSRS     r0,r0,#31
        CMP      r0,#0
        BNE      D0_High_DB4
        NOP      
        MOVS     r1,#DB4_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputLow
        NOP      
        B        exit_DB4
D0_High_DB4
        NOP      
        MOVS     r1,#DB4_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        NOP      
exit_DB4
        POP      {r4,pc}
        ENDP

E_H PROC
        PUSH     {lr}
        MOVS     r1,#E_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputHigh
        POP      {pc}
        ENDP

E_L PROC
        PUSH     {lr}
        MOVS     r1,#E_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputLow
        POP      {pc}
        ENDP
			
;
LCD_Initialize PROC
        EXPORT LCD_Initialize [CODE]
			
        PUSH     {r4,lr}
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_PUSH_PULL
        MOVS     r1,#RS_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_PUSH_PULL
        MOVS     r1,#RW_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_PUSH_PULL
        MOVS     r1,#E_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#9
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#1
        LDR      r0,XMC_GPIO_PORT1
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#0
        LDR      r0,XMC_GPIO_PORT1
        BL       XMC_GPIO_SetMode
		
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#8
        LDR      r0,XMC_GPIO_PORT0
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
			
;
LCD_SetPos PROC
        EXPORT LCD_SetPos [CODE]
			
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

;
LCD_WaitAvail PROC
        EXPORT LCD_WaitAvail [CODE]

		PUSH     {r4,lr}
        MOVS     r0,#0xff
        BL       DB4_Wr
        BL       RS_L
        NOP      
        MOVS     r1,#1
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputHigh
        NOP      
        BL       E_H
        B        LED_WAITAVAIL_Label2
LED_WAITAVAIL_Label1
        NOP      
LED_WAITAVAIL_Label2
        NOP      
        MOVS     r0,#8
        LDR      r1,XMC_GPIO_PORT0
        LDR      r1,[r1,#0x24]
        LSRS     r1,r1,r0
        LSLS     r1,r1,#31
        LSRS     r1,r1,#31
        NOP      
        CMP      r1,#0
        BNE      LED_WAITAVAIL_Label1
        BL       E_L
        POP      {r4,pc}
        ENDP

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

LCD_WrDat_4 PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        BL       LCD_WaitAvail
        NOP      
        NOP      
        NOP      
        MOVS     r1,#RS_PIN
        LDR      r0,XMC_GPIO_PORT0
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

RS_L PROC
        PUSH     {r4,lr}
        MOVS     r1,#RS_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputLow
        POP      {r4,pc}
        ENDP

RW_L PROC
        PUSH     {r4,lr}
        MOVS     r1,#RW_PIN
        LDR      r0,XMC_GPIO_PORT0
        BL       XMC_GPIO_SetOutputLow
        POP      {r4,pc}
        ENDP
			
POS_TAB_ADDR 	DCD	POS_TAB
POS_TAB			DCB	0x80,0xc0,0x94,0xd4			
XMC_GPIO_PORT0 DCD      PORT0_BASE
XMC_GPIO_PORT1 DCD      PORT1_BASE	

        AREA    |.text|, CODE, READONLY, ALIGN=1
;
LCD_displayL PROC
        EXPORT LCD_displayL [CODE]
			
        PUSH     {lr}

        MOV      r3,r2
        BL       LCD_SetPos
		
        B        LCD_Display_Label2
LCD_Display_Label1
        BL       LCD_WrDat_4
        ADDS     r3,r3,#1
LCD_Display_Label2
        LDRB     r0,[r3,#0]
        CMP      r0,#0
        BNE      LCD_Display_Label1
        POP      {pc}
        ENDP

XMC_GPIO_SetOutputHigh PROC
        MOVS     r2,#1
        LSLS     r2,r2,r1
        STR      r2,[r0,#4]
        BX       lr
        ENDP
			
XMC_GPIO_SetOutputLow PROC
        MOVS     r2,#1
        LSLS     r2,r2,#16
        LSLS     r2,r2,r1
        STR      r2,[r0,#4]
        BX       lr
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2
        DCD      0x00000000

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
