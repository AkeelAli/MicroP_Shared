/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 *
 * Generated by MATLAB(R) 7.9 and the Signal Processing Toolbox 6.12.
 *
 * Generated on: 20-Oct-2011 15:49:51
 *
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 64
 * Stable            : Yes
 * Linear Phase      : Yes (Type 2)
 */

/* General type conversion for MATLAB generated C-code  */
//#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * C:\Program Files\MATLAB\R2009b\extern\include\tmwtypes.h 
 */
/*
 * Warning - Filter coefficients were truncated to fit specified data type.  
 *   The resulting response may not match generated theoretical response.
 *   Use the Filter Design & Analysis Tool to design accurate
 *   int16 filter coefficients.
 */
#include <stm32f10x.h>
//#include <stm32f10x.h>
//#include <stm32f10x.h>

//extern s16 circBuffx[BL];
//extern s16 circBuffy[BL];
//extern int index_x;
//extern int index_y;

float process_filter(s16, int);

