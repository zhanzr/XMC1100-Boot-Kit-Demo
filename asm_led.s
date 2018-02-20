        THUMB
        REQUIRE8
        PRESERVE8

        AREA    |.text|, CODE, READONLY, ALIGN=2

LED_COUNT	EQU	5
XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN	EQU 0xC0
PORT0_BASE EQU      0x40040000
PORT1_BASE EQU      0x40040100	
LED0_PIN EQU      7	
LED1_PIN EQU      6	
LED2_PIN EQU      5	
LED3_PIN EQU      4	
LED4_PIN EQU      5	
	
LED_Initialize PROC
        EXPORT LED_Initialize [CODE]

		PUSH     {lr}
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#LED0_PIN
        LDR      r0, XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetMode
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#LED1_PIN
        LDR      r0, XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetMode
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#LED2_PIN
        LDR      r0, XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetMode
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#LED3_PIN
        LDR      r0, XMC_GPIO_PORT1_ON_OFF
        BL       XMC_GPIO_SetMode
        MOVS     r2,#XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN
        MOVS     r1,#LED4_PIN
        LDR      r0, XMC_GPIO_PORT1_ON_OFF
        BL       XMC_GPIO_SetMode
        POP      {pc}
        ENDP
	
;
LED_Off PROC
        EXPORT LED_Off [CODE]
	
        PUSH     {lr}
        MOV      r3,r0
        BL       __ARM_common_switch8_modify
		//Choice total
        DCB      0x05
		//Instruction offset for each case
		DCB		 0x04, 0x09, 0x0e, 0x13, 0x18
		//For default case, if the last case doesn't have a "jump to return" break, the offset should be decreased
        DCB      0x1c
		//Padding
		DCB		 0x00
        MOVS     r1,#LED0_PIN
        LDR      r0, XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetOutputLow
        B        led_off_exit
		
        MOVS     r1,#LED1_PIN
        LDR      r0,XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetOutputHigh
        B        led_off_exit
		
        MOVS     r1,#LED2_PIN
        LDR      r0,XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetOutputHigh
        B        led_off_exit
		
        MOVS     r1,#LED3_PIN
        LDR      r0, XMC_GPIO_PORT1_ON_OFF
        BL       XMC_GPIO_SetOutputHigh
        B        led_off_exit
		
        MOVS     r1,#LED4_PIN
        LDR      r0, XMC_GPIO_PORT1_ON_OFF
        BL       XMC_GPIO_SetOutputHigh
 
led_off_exit   
        POP      {pc}
        ENDP

;
LED_On PROC
        EXPORT LED_On [CODE]
			
        PUSH     {lr}
        MOV      r3,r0
        BL       __ARM_common_switch8_modify
		//Choice total
        DCB      0x05
		//Instruction offset for each case
		DCB		 0x04, 0x09, 0x0e, 0x13, 0x18
		//For default case, if the last case doesn't have a "jump to return" break, the offset should be decreased
        DCB      0x1c
		//Padding
		DCB		 0x00
        MOVS     r1,#LED0_PIN
        LDR      r0,XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetOutputHigh
        B        led_on_exit
		
        MOVS     r1,#LED1_PIN
        LDR      r0,XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetOutputLow
        B        led_on_exit
		
        MOVS     r1,#LED2_PIN
        LDR      r0,XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_SetOutputLow
        B        led_on_exit
		
        MOVS     r1,#LED3_PIN
        LDR      r0,XMC_GPIO_PORT1_ON_OFF
        BL       XMC_GPIO_SetOutputLow
        B        led_on_exit
		
        MOVS     r1,#LED4_PIN
        LDR      r0,XMC_GPIO_PORT1_ON_OFF
        BL       XMC_GPIO_SetOutputLow
		
led_on_exit
        POP      {pc}
        ENDP

;
LED_Toggle PROC
        EXPORT LED_Toggle [CODE]
			
        PUSH     {lr}
        MOV      r3,r0
        BL       __ARM_common_switch8_modify
		//Choice total
        DCB      0x05
		//Instruction offset for each case
		DCB		 0x04, 0x09, 0x0e, 0x13, 0x18
		//For default case, if the last case doesn't have a "jump to return" break, the offset should be decreased
        DCB      0x1c
		//Padding
		DCB		 0x00
        MOVS     r1,#LED0_PIN
        LDR      r0,XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_ToggleOutput
        B        led_toggle_exit
		
        MOVS     r1,#LED1_PIN
        LDR      r0,XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_ToggleOutput
        B        led_toggle_exit
		
        MOVS     r1,#LED2_PIN
        LDR      r0,XMC_GPIO_PORT0_ON_OFF
        BL       XMC_GPIO_ToggleOutput
        B        led_toggle_exit
		
        MOVS     r1,#LED3_PIN
        LDR      r0,XMC_GPIO_PORT1_ON_OFF
        BL       XMC_GPIO_ToggleOutput
        B        led_toggle_exit
		
        MOVS     r1,#LED4_PIN
        LDR      r0,XMC_GPIO_PORT1_ON_OFF
        BL       XMC_GPIO_ToggleOutput
    
led_toggle_exit
        POP      {pc}
        ENDP
			
XMC_GPIO_PORT0_ON_OFF DCD      PORT0_BASE
XMC_GPIO_PORT1_ON_OFF DCD      PORT1_BASE	


        AREA    |.text|, CODE, READONLY, ALIGN=1

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

XMC_GPIO_ToggleOutput PROC
        LDR      r2, TOGGLE_VAL
        LSLS     r2,r2,r1
        STR      r2,[r0,#4]
        BX       lr
        ENDP

TOGGLE_VAL        DCD      0x00010001

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA    |.text|, COMGROUP=__ARM_common_switch8_modify, CODE, READONLY, ALIGN=1

;Choice in r3, LR stores the return address(jump table)
;The target address to branch also stores in r3
;Used registers: r0, r1
__ARM_common_switch8_modify PROC
        MOV      r0,lr
        SUBS     r0,r0,#1
        LDRB     r1,[r0,#0]
        ADDS     r0,r0,#1
        CMP      r3,r1
        BLO      switch_label2
switch_label1
        LDRB     r3,[r0,r1]
        LSLS     r3,r3,#1
        ADDS     r3,r0,r3
        BX       r3
switch_label2
        MOV      r1,r3
        B        switch_label1
        ENDP

;
        EXPORT __ARM_common_switch8_modify [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT XMC_GPIO_SetMode [CODE]

        KEEP XMC_GPIO_SetOutputHigh
        KEEP XMC_GPIO_SetOutputLow
        KEEP XMC_GPIO_ToggleOutput

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
