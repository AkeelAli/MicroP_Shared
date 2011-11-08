
#include<stdio.h> 

struct __FILE { int handle;};
FILE __stdout;

int fputc (int ch, FILE *f) { 
    while ( (*((volatile unsigned long *)(0xE0000000))) == 0);
    (*((volatile unsigned char *)(0xE0000000))) = ch;
  	return ch;
}
