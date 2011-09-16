
	AREA	PROGRAM, CODE, READONLY

	EXPORT bcdadd
	ENTRY

bcdadd

	MOV r0, #0x02380000	
	MOV r1, #0x09000000

	BL add
	

stop	B stop 

   
   
   
   
   
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
		
next
	AND r2, r0, r5
	AND r3, r1, r5

	ADD r3, r2, r3

	ADD r4, r4, r3	
	CMP r4, r6 ;perform comparison here in order to include carry from previous nibble add	
	ADDGT r4, r7
	
	;shifting of special values
	;note that shifting is not done during operation (using barrel shifter) because the shifted value is not persisted
	LSL r5, #4
	LSL r6, #4
	LSL r7, #4

	;determine if we need to pursue adding nibbles
	CMP	r5, #0xf0000000
	BNE next ;when the comparison produces a result of 0, don't branch again

	;determine if overflow, and set overflow bit if the case
	TST r4,#0x10000000
	ORRNE r4,#0x40000000 ;set the overflow bit if there was an overflow

	MOV r0, r4	;store result in r0
	pop {r2-r7} ;restore context
	
	BX LR

	END