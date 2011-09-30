
	AREA	PROGRAM, CODE, READONLY

	EXPORT wrapper
	EXPORT bcdadd
	ENTRY

	;wrapper to the bcdadd routine that sends it operands and tests the returned sum
wrapper
				;test cases
				;CASE 1: ro+, r1+
				LDR r1, =0x00762500	
				LDR r0, =0x00309380
                
				BL bcdadd

				LDR r2, =0x01071880
				CMP r0, r2
				BNE error

				;CASE 2: r0-,r1-
				LDR r1, =0x80039785
				LDR r0, =0x80139962
                
				BL bcdadd

				LDR r2, =0x80179747
				CMP r0, r2
				BNE error


				;CASE 3a: r0+, r1- (|ro|>|r1|)
				LDR r0, =0x09656000	
				LDR r1, =0x87847000
                
				BL bcdadd

				LDR r2, =0x01809000
				CMP r0, r2
				BNE error

				;CASE 3b: r0+, r1- (|ro|<|r1|)
				LDR r0, =0x07847000	
				LDR r1, =0x89656000
                
				BL bcdadd

				LDR r2, =0x81809000
				CMP r0, r2
				BNE error

				;CASE 3c: r0+, r1- (|ro|=|r1|)
				LDR r0, =0x09656000	
				LDR r1, =0x89656000
                
				BL bcdadd

				LDR r2, =0x00000000
				CMP r0, r2
				BNE error

				
				;CASE 4a: r0-, r1+ (|ro|>|r1|)
				LDR r0, =0x89656000	
				LDR r1, =0x07847000
                
				BL bcdadd

				LDR r2, =0x81809000
				CMP r0, r2
				BNE error

				;CASE 4b: r0-, r1+ (|ro|<|r1|)
				LDR r0, =0x87847000	
				LDR r1, =0x09656000
                
				BL bcdadd

				LDR r2, =0x01809000
				CMP r0, r2
				BNE error

				;CASE 4c: r0-, r1+ (|ro|=|r1|)
				LDR r0, =0x89656000	
				LDR r1, =0x09656000
                
				BL bcdadd

				LDR r2, =0x00000000
				CMP r0, r2
				BNE error

				;CASE 5: r0 , r1 have an overflow to start with
				LDR r0, =0xF9656000	
				LDR r1, =0x09656000
                
				BL bcdadd

				LDR r2, =0x30000000
				CMP r0, r2
				BNE error
				   
				B success

error	B error

success		B success






	;bcdadd routine, takes two well formatted BCDs in r0 and r1, and places their BCD sum in r0
bcdadd
	push {LR}	

	;determine if any of r0 and r1 have an overflow
	TST r0, #0x40000000
	BNE overflow
	TST r1, #0x40000000
	BNE overflow

	;determine sign of r0
	TST r0, #0x80000000 
	BNE r0Negative
	B r0Positive

r0Negative
	;determine sign of r1
	TST r1, #0x80000000 
	BNE r0N_r1N
	B r0N_r1P

r0Positive
	;determine sign of r1
	TST r1, #0x80000000 
	BNE r0P_r1N
	B r0P_r1P

;CASE 1: r0 Positive, r1 Positive	
r0P_r1P
	BL add
	B checkOverflow ;check overflow only if r0 and r1 are both same sign

;CASE 2: r0 Negative, r1 Negative
r0N_r1N
	BL add
	ORR r0, #0x80000000 ;set the negative bit
	B checkOverflow	 ;check overflow only if r0 and r1 are both same sign

;CASE 3a: r0 Negative, r1 Positive (r1>=|r0|)
r0N_r1P
	AND r0, #0x0fffffff ;clear last nibble since now we've already processed overflow and sign flags
	AND r1, #0x0fffffff

	CMP r0, r1	;moved comparison before the first tensComplement (didn't work after)
	BGT r0N_r1P_GT

	BL tensComplement 	
	BL add ;r1 needs to be larger than or equal to r0 (before complement operation) for the result to be positive
	B stop	

;CASE 3b: r0 Negative, r1 Positive (r1<|r0|)
r0N_r1P_GT
	BL tensComplement 
	BL add
	BL tensComplement
	;add negative sign to result
	ORR r0, #0x80000000 ;set the negative bit 
	B stop

;CASE 4: r0 Positive, r1 Negative
r0P_r1N
	;swap and call case 3
	MOV r2, r1
	MOV r1, r0
	MOV r0, r2
	B r0N_r1P


;normal exits
checkOverflow
	;determine if overflow, and set overflow bit if the case
	TST r0,#0x10000000
	ORRNE r0,#0x40000000 ;set the overflow bit if there was an overflow

stop
	;clear don't care bits for uniformity
	AND r0, #0xCFFFFFFF

	pop {LR}
	BX LR

;abnormal exits
overflow
	LDR r0, =0x30000000 ;return special value if passed values have overflow to begin with
	
	pop {LR}
	BX LR
 
 
 
 
 
 
 
   
   ;computes tensComplement of r0 and stores it in r0
   ;assumes properly formatted BCD in r0
tensComplement
	push {r1,LR}
	
	;constructing the number #0x09999999
	LDR r2, =0x09999999

	;9's complement
	RSB r0, r0, r2
	
	;add one using our adder routine
	MOV r1, #1
	BL add

	pop {r1,LR}
	BX LR   
  
  
  
  
  
  
  
   
   ;assumes well formatted BCDs (i.e. no nibble over 9)
   ;adds +ve BCDs of 7 nibbles stored in r0 and r1 and returns result in r0
add
	push {r2-r7} ;save context of used scratch registers
	 
	MOV r4, #0 ;r4 will contain the result
	MOV r3, #0 ;r2 and r3 are for additions (r3 contains result)
	MOV	r2, #0
	
	;values used for special purposes (shifted left after every nibble addition)
	MOV r5, #0x0000000f ;used as mask to extract nibble
	MOV r6, #0x00000009	;used to determine if sum greater than 9
	MOV r7, #0x00000006 ;used to add 6 to a sum if greater than 9
		
nibble_add_loop
	;isolate the nimbles to be added
	AND r2, r0, r5
	AND r3, r1, r5

   	;add the isolated nibbles
	ADD r3, r2, r3

	;add the new nibble sum to the overall sum
	ADD r4, r4, r3	
	
	CMP r4, r6 ;perform comparison here in order to include carry from previous nibble add	
	ADDGT r4, r7
	
	;shifting of special values
	;note that shifting is not done during operation (using barrel shifter) because the shifted value is not persisted
	LSL r5, #4
	LSL r6, #4 ;shift of r6 replicates the 9's for comparison purposes with r4
	ADD r6, #9
	LSL r7, #4

	;determine if we need to pursue adding nibbles
	CMP	r5, #0xf0000000
	BNE nibble_add_loop ;when the comparison produces a result of 0, don't branch again

	MOV r0, r4	;store result in r0
	pop {r2-r7} ;restore context
	
	BX LR

	END