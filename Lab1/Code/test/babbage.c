/*
// A Main Program to call the subroutines 
*/
//TODO check if we can use stdlib for malloc
#include <stdlib.h>
#include <stdio.h>
#include <stm32f10x_lib.h>
#include "bcd.h" // declare bdc_t – could be just unsigned long or alike

typedef struct TestCase {
	int PolyOrder;
	int NumItems;
	bcd32_t *elements;
	bcd32_t *expected;
} TestCase;

/* AA The following definition of elements and expected are outside main to give them a constant address */
bcd32_t elements1[9] = { 0x1, 0x8, 0x17, 0x34, 0x0, 0x0, 0x0, 0x0, 0x0 };
bcd32_t expected1[9] = { 0x1, 0x8, 0x17, 0x34, 0x65, 0x116, 0x193, 0x302, 0x449 };

bcd32_t elements2[6] = { 0x80000001, 0x80000002, 0x80000005, 0x2, 0x31, 0x116 };
bcd32_t expected2[6] = { 0x80000001, 0x80000002, 0x80000005, 0x2, 0x31, 0x94 };

bcd32_t elements3[5] = { 0x6, 0x6, 0x1, 0x2, 0x0 };
bcd32_t expected3[5] = { 0x6, 0x6, 0x6, 0x6, 0x6 };

bcd32_t elements4[5] = { 0x4, 0x8, 0x36, 0x112, 0x0 };
bcd32_t expected4[5] = { 0x4, 0x8, 0x36, 0x112, 0x260 };

bcd32_t elements5[6] = { 0x80000005, 0x80000011, 0x80000023, 0x80000047, 0x0, 0x0 };
bcd32_t expected5[6] = { 0x80000005, 0x80000011, 0x80000023, 0x80000047, 0x80000089, 0x80000155 };

bcd32_t elements6[11] = { 0x1, 0x2, 0x129, 0x2188, 0x16385, 0x78126, 0x279937, 0x823544, 0x0, 0x0, 0x0 };
bcd32_t expected6[11] = { 0x1, 0x2, 0x129, 0x2188, 0x16385, 0x78126, 0x279937, 0x823544, 0x2097153, 0x4782970, 0x40000000 };

void main(void){ // main neither has arguments nor returns anything
	
	TestCase tests[6] = {{3, 9, &(elements1[0]), &(expected1[0])},
                               {3, 6, &(elements2[0]), &(expected2[0])},
                               {0, 5, &(elements3[0]), &(expected3[0])},
                               {3, 5, &(elements4[0]), &(expected4[0])},
                               {3, 6, &(elements5[0]), &(expected5[0])},
                               {7, 11, &(elements6[0]), &(expected6[0])}
                              };

	int i;
	int j;

	for (i=0;i<6;i++){
		printf("Expected: ");
		for (j=0;j<tests[i].NumItems;j++)
			printf("%x ", tests[i].expected[j]);
		printf("\n");		

		babbage(tests[i].PolyOrder,tests[i].NumItems,tests[i].elements);
	}
}

// babbage uses finite differences to evaluate the function value x=f(p)
// list parameters here as bcd numbers
void babbage(unsigned int PolyOrder, unsigned int NumItems, bcd32_t* Elements) { // Your code
	//must perform checks that all arguments are properly sent

	//this method uses only 1 array and overwrites no-longer-needed values with differences
	//uses a moving window of 2 elements

	//declarations
	int endI=PolyOrder;
	int i;
	int resultsI; //where to store the computed sum
	//scratch x for iteration
	int x;

	//copy the Elements into an array which will contain results
	bcd32_t* results;
	results=(bcd32_t*)malloc(sizeof(bcd32_t)*NumItems);
	for (x=0;x<NumItems;x++){
		results[x]=Elements[x];
	}

	//obtain the differences till constant row
	while(endI>0){
		for (i=0;i<endI;i++){
			//Elements[i]=Elements[i+1]-Elements[i];
			//*-1 for subtraction
			Elements[i]=Elements[i] ^ 0x80000000  ;
			Elements[i]=bcdadd(Elements[i+1],Elements[i] );
		}
		endI--;
	}

	//perform sums to obtain new results (result will be placed in last non-zero cell)
	//TODO improve on this by switching the arrays (let the given one be for results, and 
	//create a small one as big as non-zero elements)
	resultsI=PolyOrder+1;
	while (resultsI<NumItems){
		for(i=0;i<PolyOrder;i++){
			Elements[i+1]=bcdadd(Elements[i],Elements[i+1]);
		}
		//copy last computed sum which will be a result of the function
		results[resultsI]=Elements[i];
		resultsI++;
	}

	printf("Results: ");
		for (x=0;x<NumItems;x++)
			printf("%x ", results[x]);
	printf("\n");


	free(results);
}
