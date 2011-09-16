
	AREA	PROGRAM, CODE, READONLY

	EXPORT bcdadd
	ENTRY

bcdadd

	MOV r0, #0x00000236	
	MOV r1, #0x00000387
	BL add
	

stop	B stop 

   ;assumes well formatted BCDs (i.e. no nibble over 9)
add
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
	CMP	r5, #0x0f000000
	BLT next



	BX LR

	END