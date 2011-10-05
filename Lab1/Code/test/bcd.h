//bcd.h
typedef unsigned long bcd32_t;

/**
 * bcdadd: Assembly coded function that adds two bcd numbers
 * 
 * Functionality: The function adds nibble by nibble (performs 7 such sums)
 *
 * Inputs: 
 * 		c1, c2: 2 binary coded decimal numbers of type bcd32_t (could be negative)
 * 		Encoding: Bit 31: sign, Bit 30: overflow, Bits 29,28: don't care, 
 *				  every other 4 bits: decimal digit (0-9) 
 *
 * Output: returns a bcd32_t number representing the sum of the 2 inputs
 *
 * Error conditions:
 *			If any of the passed numbers are already overflown, a 0x30000000 is returned
 *			An overflow in the sum is signaled in the overflow bit 30
 */
extern bcd32_t bcdadd(bcd32_t c1, bcd32_t c2); // function is coded in assembly

/**
 * babbage: Function computing a the value of a polynomial
 * 
 * Functionality: 
 *				The function uses the babbage difference engine algorithm.
 *				It is passed initial computed elements of the polynomial, from which
 *				it computes other values of the polynomial. The number of initial elements
 *				required is = order of the polynomial + 1. 
 *
 * Inputs: 
 * 		unsigned int PolyOrder: order of the polynomial
 *		unsigned int NumItems: number of initial computed elements + elements to compute
 *		bcd32_t* Elements: array of initial elements + zero initialized elements to compute  
 *
 * Output: None
 *
 * Error conditions:
 *			The function uses bcdadd, and in the event of an overflow returns ___
 *			
 */
void babbage(unsigned int PolyOrder, unsigned int NumItems, bcd32_t* Elements);
