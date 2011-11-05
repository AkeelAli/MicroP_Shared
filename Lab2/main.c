#include "LSM303DLH.h"
#include "iNEMO_led.h"
#include "acc_lib.h"
//#include <stm32f10x.h>
#include <stdio.h>
#include <math.h>
//#include "fdacoefs.h"
#include "FIRFilter.h"

// Raw data arrays
s16 acc_data[3];

//Filtered data arrays:
float filt_data[3];

double alpha, beta;
int angle1, angle2;

//double alpha_f, beta_f;
static double a_calib=0;
static double b_calib=0; //initialized to 0

static int calibrated = 0;

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
	alpha = find_angle(filt_data[0], filt_data[2]);
	beta = find_angle(filt_data[1], filt_data[2]);

	if (calibrated){
		alpha=360-(alpha-a_calib);

		beta=360-(beta-b_calib);
		
		
		if (alpha > 360)
			alpha-=360;
		if (beta > 360)
			beta-=360;

		/* reverse direction of rotation */
		alpha = 360 - alpha;
		beta = 360 - beta;

	}
	else{
		a_calib = alpha;
		b_calib = beta;
		calibrated = 1;
	}

	angle1 = (int) alpha;
	angle2 = (int) beta;
}

int main(void){
 
	AM_Configuration();


	//calibration: place on straight surface at this point (roll=0, pitch=0)
	update_data();
	filter_data();
	calculate_angles();


	while(1){
		//TODO improve efficiency by combining both into 1 call if needed
		update_data();
        filter_data();
		calculate_angles();
	}
	
	return 0;	
}
