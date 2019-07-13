Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; --------------------------------------------------
; Allocate data variable space
	AREA | Header Data|, DATA ; Start of Data definitions
	ALIGN 4
MyData1 DCD 0 ; Word size data
MyData2 DCW 0 ; half Word size data
MyData3 DCB 0 ; byte size data

                ALIGN
; <h> Clock system handling by SSW
;   <h> CLK_VAL1 Configuration
;    <o0.0..7>    FDIV Fractional Divider Selection
;    <i> Deafult: 0. Fractional part of clock divider, MCLK = DCO1 / (2 x (IDIV + (FDIV / 256)))
;    <o0.8..15>   IDIV Divider Selection (limited to 1-16)
;                    <0=> Divider is bypassed
;                    <1=> MCLK = 32 MHz
;                    <2=> MCLK = 16 MHz
;                    <3=> MCLK = 10.67 MHz
;                    <4=> MCLK = 8 MHz
;                    <254=> MCLK = 126 kHz
;                    <255=> MCLK = 125.5 kHz
;    <i> Deafult: 4. Interger part of clock divider, MCLK = DCO1 / (2 x (IDIV + (FDIV / 256))) = 8MHz
;    <o0.16>      PCLKSEL PCLK Clock Select
;                    <0=> PCLK = MCLK
;                    <1=> PCLK = 2 x MCLK
;    <i> Deafult: 2 x MCLK
;    <o0.17..19>  RTCCLKSEL RTC Clock Select
;                    <0=> 32.768kHz standby clock
;                    <1=> 32.768kHz external clock from ERU0.IOUT0
;                    <2=> 32.768kHz external clock from ACMP0.OUT
;                    <3=> 32.768kHz external clock from ACMP1.OUT
;                    <4=> 32.768kHz external clock from ACMP2.OUT
;                    <5=> Reserved
;                    <6=> Reserved
;                    <7=> Reserved
;    <i> Deafult: 32.768kHz standby clock 
;    <o0.31>      do not move CLK_VAL1 to SCU_CLKCR[0..19]
;   </h>
; 8 MHz
;CLK_VAL1_Val    EQU     0x00010400
; 32 MHz
CLK_VAL1_Val    EQU     0x00010100
;   <h> CLK_VAL2 Configuration
;    <o0.0>    disable VADC and SHS Gating
;    <o0.1>    disable CCU80 Gating
;    <o0.2>    disable CCU40 Gating
;    <o0.3>    disable USIC0 Gating
;    <o0.4>    disable BCCU0 Gating
;    <o0.5>    disable LEDTS0 Gating
;    <o0.6>    disable LEDTS1 Gating
;    <o0.7>    disable POSIF0 Gating
;    <o0.8>    disable MATH Gating
;    <o0.9>    disable WDT Gating
;    <o0.10>   disable RTC Gating
;    <o0.31>   do not move CLK_VAL2 to SCU_CGATCLR0[0..10]
;  </h>
; *****************************************************************************/
CLK_VAL2_Val    EQU     0x80000000      
; </h>

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size					

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     CLK_VAL1_Val              ; CLK_VAL1
                DCD     CLK_VAL2_Val              ; CLK_VAL2
__Vectors_End
		
__Vectors_Size  EQU     __Vectors_End - __Vectors
	ALIGN
		
;* ================== START OF INTERRUPT HANDLER VENEERS ==================== */
                AREA    |.ARM.__at_0x10001018|, CODE, READONLY
				EXPORT VeneerStart
				EXPORT VeneerEnd
				EXPORT VeneerSize
; Veneers are located to fix SRAM Address
VeneerStart								
HardFault_Veneer 
    LDR R0, =HardFault_Handler
    BX   R0
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
    
; ======================================================================== */
SVC_Veneer
    LDR R0, =SVC_Handler
    BX   R0
    DCD     0 
    DCD     0 
; ======================================================================== */
PendSV_Veneer
    LDR R0, =PendSV_Handler
    BX   R0
; ======================================================================== */
SysTick_Veneer
    LDR R0, =SysTick_Handler
    BX   R0
; ======================================================================== */
SCU_0_Veneer
    LDR R0, =SCU_0_IRQHandler
    BX   R0
; ======================================================================== */
SCU_1_Veneer
    LDR R0, =SCU_1_IRQHandler
    BX   R0
; ======================================================================== */
SCU_2_Veneer
    LDR R0, =SCU_2_IRQHandler
    BX   R0
; ======================================================================== */
SCU_3_Veneer
    LDR R0, =ERU0_0_IRQHandler
    BX   R0
; ======================================================================== */
SCU_4_Veneer
    LDR R0, =ERU0_1_IRQHandler
    BX   R0
; ======================================================================== */
SCU_5_Veneer
    LDR R0, =ERU0_2_IRQHandler
    BX   R0
; ======================================================================== */
SCU_6_Veneer
    LDR R0, =ERU0_3_IRQHandler
    BX   R0
    DCD     0 
    DCD     0 
; ======================================================================== */
USIC0_0_Veneer
    LDR R0, =USIC0_0_IRQHandler
    BX   R0
; ======================================================================== */
USIC0_1_Veneer
    LDR R0, =USIC0_1_IRQHandler
    BX   R0
; ======================================================================== */
USIC0_2_Veneer
    LDR R0, =USIC0_2_IRQHandler
    BX   R0
; ======================================================================== */
USIC0_3_Veneer
    LDR R0, =USIC0_3_IRQHandler
    BX   R0
; ======================================================================== */
USIC0_4_Veneer
    LDR R0, =USIC0_4_IRQHandler
    BX   R0
; ======================================================================== */
USIC0_5_Veneer
    LDR R0, =USIC0_5_IRQHandler
    BX   R0
; ======================================================================== */
VADC0_C0_0_Veneer
    LDR R0, =VADC0_C0_0_IRQHandler
    BX   R0
; ======================================================================== */
VADC0_C0_1_Veneer
    LDR R0, =VADC0_C0_1_IRQHandler
    BX   R0
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
; ======================================================================== */
CCU40_0_Veneer
    LDR R0, =CCU40_0_IRQHandler
    BX   R0
; ======================================================================== */
CCU40_1_Veneer
    LDR R0, =CCU40_1_IRQHandler
    BX   R0
; ======================================================================== */
CCU40_2_Veneer
    LDR R0, =CCU40_2_IRQHandler
    BX   R0
; ======================================================================== */
CCU40_3_Veneer
    LDR R0, =CCU40_3_IRQHandler
    BX   R0
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
    DCD     0 
VeneerEnd
VeneerSize  EQU     VeneerEnd - VeneerStart
             ALIGN
				 
				AREA    |.text|, CODE, READONLY
; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]        
; Following code initializes the Veneers at address 0x20000000 with a "branch to itself"
; The real veneers will be copied later from the scatter loader before reaching main.
; This init code should handle an exception before the real veneers are copied.
SRAM_VENEER_BASE          EQU     0x2000000C

; Copy interrupt veneer to SRAM from NV Storage
	ldr	r1, =__Vectors_End
	ldr	r2, =SRAM_VENEER_BASE
	ldr	r3, =(SRAM_VENEER_BASE + VeneerSize)
	bl  __copy_data

    ldr  r0, =asm_blink
    blx  r0
                ENDP

asm_blink PROC
                IMPORT  LED_Initialize
                IMPORT  LED_Off
                IMPORT  LED_On
                IMPORT  LED_Toggle
					
SysTick_BASE	EQU	0xE000E010
SysTick_CTRL	EQU	0
SysTick_LOAD	EQU	4
SysTick_VAL	EQU	8
	
	LDR R0,=MyData1
	LDR R1,=0x00001234
	STR R1,[R0] ; MyData1 = 0x00001234
	LDR R0,=MyData2
	LDR R1,=0x55CC
	STRH R1,[R0] ; MyData2 = 0x55CC
	LDR R0,=MyData3
	LDR R1,=0xAA
	STRB R1,[R0] ; MyData3 = 0xAA	
	
	BL LED_Initialize
	
	; Program SysTick timer at 1KHz.
	LDR R0,=SysTick_BASE
	LDR R1,=(32000000/1000 - 1)
	STR R1,[R0, SysTick_LOAD]
	MOVS R1, 0
	STR R1,[R0, SysTick_VAL]
	MOVS R1, 5
	STR R1,[R0, SysTick_CTRL]	
	
;Delay 500 * 1 ms
Blinky_loop
	LDR R0,=SysTick_BASE
	LDR R1, =1000
Blinky_inner_loop1
Blinky_inner_loop2
	LDR R2, =0x10000
	LDR R3, [R0, SysTick_CTRL]
	TST R3, R2
	BEQ Blinky_inner_loop2
	SUBS R1, R1, 1
	BNE Blinky_inner_loop1

	MOVS R0, 0
	BL LED_Toggle
	
	MOVS R0, 1
	BL LED_Toggle

	MOVS R0, 2
	BL LED_Toggle
	
	MOVS R0, 3
	BL LED_Toggle
	
	MOVS R0, 4
	BL LED_Toggle
				B Blinky_loop
                ENDP
					
__copy_data PROC
;  The ranges of copy from/to are specified by following symbols
;    r1: start of the section to copy from.
;    r2: start of the section to copy to
;    r3: end of the section to copy to
;
;  All addresses must be aligned to 4 bytes boundary.
;  Uses r0
;/
	subs	r3, r2
	ble	_L_loop_done

_L_loop
	subs	r3, 4
	ldr	r0, [r1,r3]
	str	r0, [r2,r3]
	bgt	_L_loop

_L_loop_done
	bx  lr
                ENDP

Default_Handler PROC

                EXPORT  HardFault_Handler       [WEAK]
                EXPORT  SVC_Handler             [WEAK]
                EXPORT  PendSV_Handler          [WEAK]
                EXPORT  SysTick_Handler         [WEAK]
                EXPORT  SCU_0_IRQHandler        [WEAK]
                EXPORT  SCU_1_IRQHandler        [WEAK]
                EXPORT  SCU_2_IRQHandler        [WEAK]
                EXPORT  ERU0_0_IRQHandler       [WEAK]
                EXPORT  ERU0_1_IRQHandler       [WEAK]
                EXPORT  ERU0_2_IRQHandler       [WEAK]
                EXPORT  ERU0_3_IRQHandler       [WEAK]
                EXPORT  USIC0_0_IRQHandler      [WEAK]
                EXPORT  USIC0_1_IRQHandler      [WEAK]
                EXPORT  USIC0_2_IRQHandler      [WEAK]
                EXPORT  USIC0_3_IRQHandler      [WEAK]
                EXPORT  USIC0_4_IRQHandler      [WEAK]
                EXPORT  USIC0_5_IRQHandler      [WEAK]
                EXPORT  VADC0_C0_0_IRQHandler   [WEAK]
                EXPORT  VADC0_C0_1_IRQHandler   [WEAK]
                EXPORT  CCU40_0_IRQHandler      [WEAK]
                EXPORT  CCU40_1_IRQHandler      [WEAK]
                EXPORT  CCU40_2_IRQHandler      [WEAK]
                EXPORT  CCU40_3_IRQHandler      [WEAK]

HardFault_Handler       
SVC_Handler             
PendSV_Handler          
SysTick_Handler         
SCU_0_IRQHandler        
SCU_1_IRQHandler        
SCU_2_IRQHandler        
ERU0_0_IRQHandler       
ERU0_1_IRQHandler       
ERU0_2_IRQHandler       
ERU0_3_IRQHandler       
USIC0_0_IRQHandler      
USIC0_1_IRQHandler      
USIC0_2_IRQHandler      
USIC0_3_IRQHandler      
USIC0_4_IRQHandler      
USIC0_5_IRQHandler      
VADC0_C0_0_IRQHandler   
VADC0_C0_1_IRQHandler   
CCU40_0_IRQHandler      
CCU40_1_IRQHandler      
CCU40_2_IRQHandler      
CCU40_3_IRQHandler      
                B       .

                ENDP

                ALIGN

;* ================== END OF INTERRUPT HANDLER VENEERS ====================== */

                END
