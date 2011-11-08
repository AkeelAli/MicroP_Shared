#include "LSM303DLH.h"
#include "iNEMO_led.h"
#include "acc_lib.h"
#include <stdio.h>
#include <math.h>

// Raw data arrays
s16 acc_data[3];
double alpha, beta;
static double a_calib, b_calib; //initialized to 0

// Process raw data from peripherals
void update_data() {
	LSM303DLH_Acc_Read_Acc(acc_data);	// Read accelerometer data
}

void filter_data() {

}

void calculate_angles(){
	alpha=arctan(acc_data[0]/sqrt( (acc_data[1]*acc_data[1]+acc_data[2]*acc_data[2]) *1.0 ));
	beta=arctan(acc_data[1]/sqrt(  (acc_data[0]*acc_data[0]+acc_data[2]*acc_data[2])  *1.0 ));

	alpha=alpha-a_calib;
	beta=beta-b_calib;
}

int main(void){
//	Led_TypeDef red_led;	
 
	AM_Configuration();
//	iNEMO_Led_Init(red_led);	

//	iNEMO_Led_Toggle(red_led);

	//calibration: place on straight surface at this point (roll=0, pitch=0)
	update_data();
	calculate_angles();
	a_calib=alpha;
	b_calib=beta;


	while(1){
		//TODO improve efficiency by combining both into 1 call if needed
		update_data();
		calculate_angles();
	}
	
	return 0;	
}
