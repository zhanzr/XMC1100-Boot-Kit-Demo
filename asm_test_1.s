;Description: Instruction Test 1
;Move
;Operation				Description Assembler	Cycles
;Move 8-bit immediate 	MOVS Rd, #<imm>			1
;Lo to Lo 				MOVS Rd, Rm 			1
;Any to Any 			MOV Rd, Rm 				1
;Any to PC 				MOV PC, Rm 				3

	AREA    |.text|, CODE, READONLY

;Get an 8bit immediate number
	ALIGN        ; Ensures alignment.
asm_get_8bit_number   FUNCTION     ; Without the ALIGN directive this might not be word-aligned.
	EXPORT asm_get_8bit_number
		
	MOVS r0, #234
	bx lr
	
	ENDP
		
;Get the XOR result
	ALIGN
asm_get_xor   FUNCTION    
	EXPORT asm_get_xor

	eors r0, r0, r1

	bx lr
	
	ENDP
		
;Direct Jump Test
	ALIGN
asm_direct_jump   FUNCTION    
	EXPORT asm_direct_jump

	mov PC,r0
;asm_test_1.s(42): warning: A1608W: MOV pc,<rn> instruction used, but BX <rn> is preferred
;Two kinds of direct jumping. BX <rn> is preferred.
	;bx r0
	bx lr
	
	ENDP	
	
	END
