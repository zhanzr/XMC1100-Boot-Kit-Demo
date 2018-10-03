;Description: Instruction Test 3
;Subtract
;Operation				Description Assembler	Cycles
;Lo and Lo 				SUBS Rd, Rn, Rm 		1
;3-bit immediate 		SUBS Rd, Rn, #<imm> 	1
;8-bit immediate 		SUBS Rd, Rd, #<imm> 	1
;With carry 			SBCS Rd, Rd, Rm 		1
;Immediate from SP 		SUB SP, SP, #<imm> 		1
;Subtract Negate 		RSBS Rd, Rn, #0 		1

	AREA    |.text|, CODE, READONLY
		
;8-bit immediate 
	ALIGN
asm_sub20  FUNCTION    
	EXPORT asm_sub20
	
	SUBS r0, r0, #20

	bx lr
	
	ENDP	
	
;Simple Sub
	ALIGN
asm_simple_sub  FUNCTION    
	EXPORT asm_simple_sub
	
	SUBS r0, r0, r1

	bx lr
	
	ENDP			
		
;Subtract Negate
	ALIGN
asm_get_neg   FUNCTION    
	EXPORT asm_get_neg
		
	RSBS r1, r0, #0
	
	movs r0, r1
	
	bx lr
	
	ENDP
		
	END
