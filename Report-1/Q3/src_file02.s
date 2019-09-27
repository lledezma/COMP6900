        THUMB
		AREA DATA, ALIGN = 2
N		SPACE 4 ; reserve 4 bytes in RAM for variable M
M		SPACE 4 ; 
	
	
		AREA |.text|, CODE, READONLY, ALIGN =2
		EXPORT Def
        EXPORT N 
		EXPORT M
	 
Def  	LDR R3, = M
		LDR R2, = N ; R2 = &N
		LDR R0, [R2] ; R0 = N, copy value of M to register R0
		LDR R1, = 200 
		MUL R0, R0, R1; R0= N*200
		LDR R1, = 30
		ADD R0, R0, R1; R0= N*200 + 30
		LDR R1, = 10
		UDIV R0, R0, R1; R0 = (N*200 + 30) / 10
		STR R0, [R3]; Update M value by writing content of R0 to the address of pointed by R2 (adddress of M)
		BX LR  ; get the next PC instruction from LR to return from this function
		ALIGN
		END
