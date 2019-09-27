		GLOBAL main
		AREA mycode, CODE, READONLY
pInt	DCD 0x20000002
		
main 
		LDR R0, pInt
		LDR R1, =0xBEEFFEED
		MOV R1, R1
		STR R1, [R0]
		
		LDR R2, = 0xABBCABBC
		MOV R2, R2
		STR R2, [R0, #4]
		
		LDR R3, = 0xCAADBAAD
		MOV R3, R3
		STR R3, [R0, #0x10]
		
		END
