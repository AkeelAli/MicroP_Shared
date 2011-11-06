#include "LSM303DLH.h"
#include "iNEMO_led.h"
#include "acc_lib.h"
//#include <stm32f10x.h>
#include <stdio.h>
#include <math.h>
#include "stm32f10x_tim.h"
#include "FIRFilter.h"

#include "configurations.h"

// Raw data arrays
s16 acc_data[3];


//Filtered data arrays:
float filt_data[3];

double alpha, beta;
#define DEBUG 1
#if (DEBUG == 1)
double alpha_acc, beta_acc;
#endif
//double alpha_f, beta_f;
static double a_calib=0;
static double b_calib=0; //initialized to 0

int pitch=0, roll=0;

// Process raw data from peripherals
void update_data() {
	LSM303DLH_Acc_Read_Acc(acc_data);	// Read accelerometer data
}

void filter_data() {

    filt_data[0] = process_filter(acc_data[0], 0);
    filt_data[1] = process_filter(acc_data[1], 1);
	filt_data[2] = process_filter(acc_data[2], 2);

}

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



int main(void){
//	Led_TypeDef red_led;	
 
	AM_Configuration();
//	iNEMO_Led_Init(red_led);	

//	iNEMO_Led_Toggle(red_led);

	I_Configuration();

	//calibration: place on straight surface at this point (roll=0, pitch=0)
	update_data();
	filter_data();
	calculate_angles();
	a_calib=alpha;
	b_calib=beta;


	while(1){
		//TODO improve efficiency by combining both into 1 call if needed
		
	}
	
	return 0;	
}

__irq void TIM2_IRQHandler(void) {
	update_data();
    filter_data();
	calculate_angles();
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                       	
}

