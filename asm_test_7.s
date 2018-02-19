;Description: Instruction Test 7
;Extend, Reverse
;Operation							Description Assembler	Cycles
;Signed halfword to word 			SXTH Rd, Rm 			1
;Signed byte to word 				SXTB Rd, Rm 			1
;Unsigned halfword 					UXTH Rd, Rm 			1
;Unsigned byte 						UXTB Rd, Rm 			1
;Bytes in word 						REV Rd, Rm 				1
;Bytes in both halfwords 			REV16 Rd, Rm 			1
;Signed bottom half word 			REVSH Rd, Rm 			1

	AREA    |.text|, CODE, READONLY
		
;Signed halfword to word 
	ALIGN
asm_s16ext FUNCTION    
	EXPORT asm_s16ext
	
	SXTH r0, r0
	bx lr
	
	ENDP
		
;Signed byte to word 
	ALIGN
asm_s8ext FUNCTION    
	EXPORT asm_s8ext
	
	SXTB r0, r0
	bx lr
	
	ENDP
	
;Unsigned halfword to word 
	ALIGN
asm_u16ext FUNCTION    
	EXPORT asm_u16ext
	
	UXTH r0, r0
	bx lr
	
	ENDP
		
;Unsigned byte to word 
	ALIGN
asm_u8ext FUNCTION    
	EXPORT asm_u8ext
	
	UXTB r0, r0
	bx lr
	
	ENDP
		
;Reverse Bytes in word 
	ALIGN
asm_rev FUNCTION    
	EXPORT asm_rev
	
	REV r0, r0
	bx lr
	
	ENDP	

;Reverse Bytes in both halfwords
	ALIGN
asm_rev16 FUNCTION    
	EXPORT asm_rev16
	
	REV16 r0, r0
	bx lr
	
	ENDP	
		
;Reverse Signed bottom half word
	ALIGN
asm_revsh FUNCTION    
	EXPORT asm_revsh
	
	REVSH r0, r0
	bx lr
	
	ENDP			

	END
