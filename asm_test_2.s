;Description: Instruction Test 2
;Add
;Operation				Description Assembler	Cycles
;3-bit immediate 		ADDS Rd, Rn, #<imm> 	1
;All registers Lo 		ADDS Rd, Rn, Rm 		1
;Any to Any 			ADD Rd, Rd, Rm 			1
;Any to PC 				ADD PC, PC, Rm 			3
;8-bit immediate 		ADDS Rd, Rd, #<imm> 	1
;With carry 			ADCS Rd, Rd, Rm 		1
;Immediate to SP 		ADD SP, SP, #<imm> 		1
;Form address from SP 	ADD Rd, SP, #<imm> 		1
;Form address from PC 	ADR Rd, <label> 		1
        THUMB
        REQUIRE8
        PRESERVE8
	AREA    |.text|, CODE, READONLY

;Add 3bit immediate
	ALIGN
asm_add2  FUNCTION    
	EXPORT asm_add2
	
	ADDS r0, r0, #2

	bx lr
	
	ENDP	
	
;Simple Add
	ALIGN
asm_simple_add  FUNCTION    
	EXPORT asm_simple_add
	
	ADDS r0, r0, r1

	bx lr
	
	ENDP	
		
		
;PC Add
	ALIGN
asm_pc_add  FUNCTION    
	EXPORT asm_pc_add
	
	MOVS r0, #6
	;Cortex M0 has 3 levels of pipeline, when this instruction is to be executed, the PC points to test_label
	ADD PC, PC, r0
	MOVS r0, #1

test_label
	bx lr
	
	MOVS r0, #5
	bx lr
	
	MOVS r0, #7
	bx lr
	
	ENDP	
		

	END
