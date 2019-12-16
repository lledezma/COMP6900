		IMPORT  Funct
		IMPORT  varX
		AREA  |.text|, CODE, READONLY, ALIGN =2
		EXPORT main

myFunct 
		
		LSL R10, R0, #2
		ADD R10, R10, #0x15
		BX LR ; return 
		
main LDR R0, =varX
	 MOV R1, 0x35
	 STR R1, [R0]
	 BL myFunct ; call myFunct 
	 MOV R1, R4

	 
stop B stop
	 ALIGN
	 END
