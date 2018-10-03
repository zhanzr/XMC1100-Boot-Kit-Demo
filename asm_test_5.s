;Description: Instruction Test 5
;Shift, Rotate
;Operation							Description Assembler	Cycles
;Logical shift left by immediate 	LSLS Rd, Rm, #<shift> 	1
;Logical shift left by register 	LSLS Rd, Rd, Rs 		1
;Logical shift right by immediate 	LSRS Rd, Rm, #<shift> 	1
;Logical shift right by register 	LSRS Rd, Rd, Rs 		1
;Arithmetic shift right 			ASRS Rd, Rm, #<shift> 	1
;Arithmetic shift right by register ASRS Rd, Rd, Rs 		1
;Rotate right by register 			RORS Rd, Rd, Rs 		1

	AREA    |.text|, CODE, READONLY

;Logical shift left by register
	ALIGN
asm_logic_left  FUNCTION    
	EXPORT asm_logic_left
	
	LSLS r0, r0, r1

	bx lr
	
	ENDP
		
;Logical shift right by register
	ALIGN
asm_logic_right  FUNCTION    
	EXPORT asm_logic_right
	
	LSRS r0, r0, r1

	bx lr
	
	ENDP
				
;Arithmetic shift right by register
	ALIGN
asm_arithm_right  FUNCTION    
	EXPORT asm_arithm_right
	
	ASRS r0, r0, r1

	bx lr
	
	ENDP
		
;Rotate right by register
	ALIGN
asm_rotate_right  FUNCTION    
	EXPORT asm_rotate_right
	
	RORS r0, r0, r1

	bx lr
	
	ENDP
		
	END
