				
				
				AREA asm_functions, CODE, READONLY				
                EXPORT  ricerca_massimo_negativo
ricerca_massimo_negativo
				; save current SP for a faster access
				; to parameters in the stack
				MOV   r12, sp
				; save volatile registers
				STMFD sp!,{r4-r8,r10-r11,lr}		/* salvataggio dello stato tranne r0-r3 fatti apposta per passare parametri */ 
	
				MOV r5, #0
				SUB r1, #1
first_negative				
				LDR	r3, [r0,r1, LSL #2]				
				CMP r3, #0
				BLT loop				
				SUBS r1, #1				
				BLT fine
				B	first_negative
				
loop			SUBS r1, #1
				MOVMI R5, R3
				BMI	fine
				LDR r2, [r0,r1, LSL #2]
				CMP r2, #0
				SUBLTS r4, r3, r2
				MOVLT r3, r2
				B	loop
								
				; setup a value for R0 to return
fine				
				MOV	  r0, r5
				; restore volatile registers
				LDMFD sp!,{r4-r8,r10-r11,pc}
				
                END