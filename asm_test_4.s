;Description: Instruction Test 4
;Multiply, Compare, Logical
;Operation				Description Assembler	Cycles
;Multiply 				MULS Rd, Rm, Rd 		1 or 32(depends on implementation)
;Compare 				CMP Rn, Rm 				1
;Negative 				CMN Rn, Rm 				1
;Immediate 				CMP Rn, #<imm> 			1
;AND 					ANDS Rd, Rd, Rm 		1
;Exclusive OR 			EORS Rd, Rd, Rm 		1
;OR 					ORRS Rd, Rd, Rm 		1
;Bit clear 				BICS Rd, Rd, Rm 		1
;Move NOT 				MVNS Rd, Rm 			1
;AND test 				TST Rn, Rm 				1

	AREA    |.text|, CODE, READONLY
		
;Simple Multiply
	ALIGN
asm_simple_mul FUNCTION    
	EXPORT asm_simple_mul
	
	MULS r0, r1, r0

	bx lr
	
	ENDP	

;The Compare operations are identical to subtracting, for CMP, or adding, for CMN, except
;that the result is discarded. See the instruction descriptions for more information.

;Bcond is the only conditional instruction on the Cortex-M0 processor.
;Test Compare
	ALIGN
asm_test_cmp FUNCTION    
	EXPORT asm_test_cmp
	
	CMP r0, r1
;	CMP r0, #0x00001000
	BLT	cmp_low
cmp_large_eq
	movs r0, #1
	bx lr
cmp_low
	movs r0, #0
	bx lr
	
	ENDP	

;Test Compare Negative
	ALIGN
asm_test_cmn FUNCTION    
	EXPORT asm_test_cmn
	
	CMN r0, r1
	BLT	cmn_low
cmn_large_eq
	movs r0, #1
	bx lr
cmn_low
	movs r0, #0
	bx lr
	
	ENDP	
		
;Get the AND result
	ALIGN
asm_get_and  FUNCTION    
	EXPORT asm_get_and

	ANDS r1, r0, r1
	movs r0, r1
	
	bx lr
	
	ENDP

;Get the OR result
	ALIGN
asm_get_or   FUNCTION    
	EXPORT asm_get_or

	orrs r1, r0, r1
	movs r0, r1
	
	bx lr
	
	ENDP

;Get the NOT result
	ALIGN
asm_get_not   FUNCTION    
	EXPORT asm_get_not

	MVNS r1, r0
	movs r0, r1
	
	bx lr
	
	ENDP
		
	END
