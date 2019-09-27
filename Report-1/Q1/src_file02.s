	GLOBAL main
	AREA mycode, CODE, READONLY
	
	ENTRY
	
main	MOV R9, 0x23
		STR R2, [R9]
		
		LDR R0, [R2]
		
		MOV R1, #0x23
		
		STR R1, [R0]
		
		END
