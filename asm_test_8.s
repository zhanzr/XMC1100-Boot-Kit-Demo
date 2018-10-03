;Description: Instruction Test 8
;State change, Others
;Operation							Description Assembler	Cycles
;Supervisor Call 					SVC #<imm> 				- e
;Disable interrupts 				CPSID i 				1
;Enable interrupts 					CPSIE i 				1
;Read special register 				MRS Rd, <specreg> 		4
;Write special register 			MSR <specreg>, Rn 		4

	AREA    |.text|, CODE, READONLY
		
;Supervisor Call
	ALIGN
asm_svc_1 FUNCTION    
	EXPORT asm_svc_1
	
	SVC #1
	
	bx lr
	
	ENDP

;Test special register write
;special register is one of the special-purpose registers: APSR, IPSR, EPSR, IEPSR, IAPSR,
;EAPSR, PSR, MSP, PSP, PRIMASK, or CONTROL.
	ALIGN
asm_test_msr FUNCTION    
	EXPORT asm_test_msr
	;Write 0x00000001 to disable all exceptions with configurable priority
	;Write 0x00000000 to enable all exceptions with configurable priority
	MSR PRIMASK,r0
	
	bx lr
	
	ENDP	
		
;Test special register read
;special register is one of the special-purpose registers: APSR, IPSR, EPSR, IEPSR, IAPSR,
;EAPSR, PSR, MSP, PSP, PRIMASK, or CONTROL.
	ALIGN
asm_test_mrs FUNCTION    
	EXPORT asm_test_mrs
	
	MRS r0, PRIMASK
	
	bx lr
	
	ENDP	
		
	END
