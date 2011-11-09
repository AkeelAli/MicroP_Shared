#include "LSM303DLH.h"
#include "iNEMO_led.h"
#include "acc_lib.h"
#include <stdio.h>
#include <math.h>
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "FIRFilter.h"

#include "configurations.h"

#define FREE_FALL_THRESHOLD 800
#define COMPASS_MAX_ANGLE 50
#define DEBUG

// Raw data arrays
s16 acc_data[3];
s16 magn_data[3];

//Filtered data arrays:
float filt_data[3];

double alpha, beta;

//double alpha_f, beta_f;
static double a_calib=0;
static double b_calib=0; //initialized to 0

int pitch=0, roll=0;
float heading=0;

Led_TypeDef red_led;

enum MODE_T{
	compass,
	freefall
};
/* static enum variable to keep state (either COMPASS or FREFALL) */
static enum MODE_T mode = compass;

#ifdef DEBUG 
int free_fall_value;
#endif

/**
	Update Raw Data from Accelerometer and Magnometer
*/
void update_data() {
	LSM303DLH_Acc_Read_Acc(acc_data);	

	LSM303DLH_Magn_Read_Magn(magn_data);
}

/**
	Filter Data
*/
void filter_data() {

    filt_data[0] = process_filter(acc_data[0], 0);
    filt_data[1] = process_filter(acc_data[1], 1);
	filt_data[2] = process_filter(acc_data[2], 2);

}

/**
	Pitch and Roll calculations
*/
void calculate_angles(){
	alpha=arctan(filt_data[0]/sqrt( (filt_data[1]*filt_data[1]+filt_data[2]*filt_data[2]) *1.0 ));
	beta=arctan(filt_data[1]/sqrt(  (filt_data[0]*filt_data[0]+filt_data[2]*filt_data[2])  *1.0 ));

	/* adjust for 180 degrees */
	// 2nd quadrant
	if (filt_data[0] > 0 && filt_data[2] < 0)
		alpha = 180 - alpha;
	// 3rd quadrant
	else if (filt_data[0] < 0 && filt_data[2] < 0)
		alpha = -180 - alpha;

	/* adjust for 180 degrees */
	if (filt_data[1] > 0 && filt_data[2] < 0)
		beta = 180 - beta;
	else if (filt_data[1] < 0 && filt_data[2] < 0)
		beta = -180 - beta;

	alpha=alpha-a_calib;
	beta=beta-b_calib;


	pitch = (int) alpha;
	roll = (int) beta;
}

/**
	Heading calculations stored in variable heading
*/
void calculate_heading(){

	s16 y = magn_data[1]*cos(roll) - magn_data[2]*cos(pitch)*sin(roll) + magn_data[0]*sin(pitch)*sin(roll);
	
	s16 x = magn_data[0]*cos(pitch) + magn_data[2]*sin(pitch);


	heading = arctan(y / x);
	
	if (x < 0)
		heading += 180;
	else if (x > 0 && y <= 0)
		heading += 360;
	else if (x == 0 && y < 0)
		heading = 90;
	else if (x == 0 && y > 0)
		heading = 270;		
}

/**
	COMPASS MODE: Check if exceeded max angle and light LED
*/
void check_maxAngle(){
	if (abs(roll) > COMPASS_MAX_ANGLE || abs(pitch) > COMPASS_MAX_ANGLE)
		iNEMO_Led_On(red_led);
	else
		iNEMO_Led_Off(red_led);
}

/**
	FREEFALL MODE: Check if freefall, and generate SW interrupt (that lights LED)
*/
void check_freefall(){
	free_fall_value = sqrt( acc_data[0]*acc_data[0] 
		+ acc_data[1]*acc_data[1] + acc_data[2]*acc_data[2] );
	
	if (  free_fall_value < FREE_FALL_THRESHOLD ) 
		EXTI_GenerateSWInterrupt(EXTI_Line12);
	else
		iNEMO_Led_Off(red_led);
}

/***************************************
	MAIN FUNCTION
****************************************/

int main(void){	
 
	/* Call Configuration Functions */
	iNEMO_Led_Init(red_led);

	AM_Configuration();

	TIM_Configuration();

	SWInt_Configuration();
	
	ButtonInt_Configuration();	

	/* calibration: place on straight surface at this point (roll=0, pitch=0) */
	update_data();
	filter_data();
	calculate_angles();
	a_calib=alpha;
	b_calib=beta;


	while(1){		
	}
	
	return 0;	
}


/***************************************
	INTERRUPT HANDLERS
****************************************/


/**
	main timer interrupt handler
*/
__irq void TIM2_IRQHandler(void) {
	update_data();
    filter_data();
	calculate_angles();
	calculate_heading();
	if (mode == freefall)	
		check_freefall();
	else
		check_maxAngle();
	
	//clear the pending interrupt
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                       	
}

/**
	ISR handling both the button and the freefall interrupt
*/
__irq void EXTI15_10_IRQHandler(void){
	// button interrupt
	if (EXTI_GetITStatus(EXTI_Line13) != RESET){
 		if (mode == freefall)	
			mode = compass;
		else
			mode = freefall;
			
		EXTI_ClearITPendingBit(EXTI_Line13);
	} 
	//SWInterrupt handler for freefall
	else if (EXTI_GetITStatus(EXTI_Line12) != RESET){
		iNEMO_Led_On(red_led);

		EXTI_ClearITPendingBit(EXTI_Line12);
	}

}


