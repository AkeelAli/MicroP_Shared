/*
// A Main Program to call the subroutines 
*/
//TODO check if we can use stdlib for malloc
#include <stdlib.h>
#include <stdio.h>
#include <stm32f10x_lib.h>
#include "bcd.h" // declare bdc_t – could be just unsigned long or alike

void main(void){ // main neither has arguments nor returns anything
	unsigned int order=3;	
	unsigned int items=10;

	bcd32_t e[14]={0};

	//compute values for as many items as order+1
	int x;
	for(x=0;x<=order;x++){
		e[x]=(1+(8*x)-(2*(x*x))+(x*x*x));	
	} 

	babbage(order,items,e);
}

// babbage uses finite differences to evaluate the function value x=f(p)
// list parameters here as bcd numbers
void babbage(unsigned int PolyOrder, unsigned int NumItems, bcd32_t* Elements) { // Your code
	//must perform checks that all arguments are properly sent

	//this method uses only 1 array and overwrites no-longer-needed values with differences
	//uses a moving window of 2 elements

	//declarations
	int numElements=NumItems+PolyOrder+1;
	int endI=PolyOrder;
	int i;
	int resultsI; //where to store the computed sum
	//scratch x for iteration
	int x;

	//copy the Elements into an array which will contain results
	bcd32_t* results;
	results=(bcd32_t*)malloc(sizeof(Elements));
	for (x=0;x<numElements;x++){
		results[x]=Elements[x];
	}

	//obtain the differences till constant row
	while(endI>0){
		for (i=0;i<endI;i++){
			Elements[i]=Elements[i+1]-Elements[i];
		}
		endI--;
	}

	//perform sums to obtain new results (result will be placed in last non-zero cell)
	//TODO improve on this by switching the arrays (let the given one be for results, and 
	//create a small one as big as non-zero elements)
	resultsI=PolyOrder+1;
	while (resultsI<numElements){
		for(i=0;i<PolyOrder;i++){
			Elements[i+1]=Elements[i]+Elements[i+1];
		}
		//copy last computed sum which will be a result of the function
		results[resultsI]=Elements[i];
		resultsI++;
	}


	for (x=0;x<13;x++)
		printf("%d ",Elements[x]);
	printf("\n");

	for (x=0;x<13;x++)
		printf("%d ",results[x]);

	free(results);
}

// other functions come here, if needed
// other parameters interpreted as other data types
//void other_functions(/*parameters … */) { // Your code
//}