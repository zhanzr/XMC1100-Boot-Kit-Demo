;Description: Instruction Test 6
;Load, Store, Push, Pop
;Operation							Description Assembler	Cycles
;Word, immediate offset 			LDR Rd, [Rn, #<imm>] 	2
;Halfword, immediate offset 		LDRH Rd, [Rn, #<imm>] 	2
;Byte, immediate offset 			LDRB Rd, [Rn, #<imm>] 	2
;Word, register offset 				LDR Rd, [Rn, Rm] 		2
;Halfword, register offset 			LDRH Rd, [Rn, Rm] 		2
;Signed halfword, register offset 	LDRSH Rd, [Rn, Rm] 		2
;Byte, register offset 				LDRB Rd, [Rn, Rm] 		2
;Signed byte, register offset 		LDRSB Rd, [Rn, Rm] 		2
;PC-relative 						LDR Rd, <label> 		2
;SP-relative 						LDR Rd, [SP, #<imm>] 	2
;Multiple, excluding base 			LDM Rn!, {<loreglist>} 	1+Nb
;Multiple, including base 			LDM Rn, {<loreglist>} 	1+Nb
;Word, immediate offset 			STR Rd, [Rn, #<imm>] 	2
;Halfword, immediate offset 		STRH Rd, [Rn, #<imm>] 	2
;Byte, immediate offset 			STRB Rd, [Rn, #<imm>] 	2
;Word, register offset 				STR Rd, [Rn, Rm] 		2
;Halfword, register offset 			STRH Rd, [Rn, Rm] 		2
;Byte, register offset 				STRB Rd, [Rn, Rm] 		2
;SP-relative 						STR Rd, [SP, #<imm>] 	2
;Multiple 							STM Rn!, {<loreglist>} 	1+Nb
;Push								PUSH {<loreglist>} 		1+Nb
;Push with link register 			PUSH {<loreglist>, LR} 	1+Nb
;Pop 								POP {<loreglist>} 		1+Nb
;Pop and return 					POP {<loreglist>, PC} 	4+Nc

	AREA    |.text|, CODE, READONLY
		;According to the AAPCS:
;R0-R3, R12 
;Caller saved register. Contents in these registers can be changed by a function.
;Assembly code calling a function might need to save the values in these registers if
;they are required for operations in later stages.

;R4-R11 
;Caller saved register. Contents in these registers must be retained by a function. If
;a function needs to use these registers for processing, they need to be saved on to
;the stack memory and restored before the function returns.

;R14 (LR)
;Content in the Link Register needs to be saved to stack if the function contains
;a ¡°BL¡±/¡±BLX¡± instruction (calling another function) because the value in LR will be
;overwritten when ¡°BL¡±/¡±BLX¡± is executed.

;R13 (SP), R15 (PC)
;Should not be used for normal processing.

;load a 32bit
	ALIGN
asm_ldr32 FUNCTION    
	EXPORT asm_ldr32
	
	mov r1,r0
	LDR r0, [r1, #0]
	bx lr
	
	ENDP	
		
;store a 32bit
	ALIGN
asm_str32 FUNCTION    
	EXPORT asm_str32
	
	STR r1, [r0, #0]
	bx lr
	
	ENDP	
		
;push & pop
	ALIGN
asm_test_push_pop FUNCTION    
	EXPORT asm_test_push_pop
	
	push {r0,r1}
	
	pop {r2,r3}
	
	ADDS r2, r2, r3
	ADDS r3, r3, #4
	
	push {r2,r3}
	
	pop {r0,r1}
	
	adds r0, r0, r1
	
	bx lr
	
	ENDP	
		
	END
