#include "LSM303DLH.h"
#include "iNEMO_led.h"
#include "acc_lib.h"
//#include <stm32f10x.h>
#include <stdio.h>
#include <math.h>
//#include "fdacoefs.h"
#include "FIRFilter.h"

#define TILT_METHOD2

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

#ifdef TILT_METHOD1
	//goes from 0 to 90 degrees (with negative angles)
	//has problems with angles
	alpha=arctan(filt_data[0]/sqrt( (filt_data[1]*filt_data[1]+filt_data[2]*filt_data[2]) *1.0 ));
	beta=arctan(filt_data[1]/sqrt(  (filt_data[0]*filt_data[0]+filt_data[2]*filt_data[2])  *1.0 ));
	
	if (calibrated){
	   	alpha=alpha-a_calib;
		beta=beta-b_calib;
	}
	else{
		a_calib = alpha;
		b_calib = beta;
		calibrated = 1;
	}

#elif defined TILT_METHOD2
	//this method goes from 0 to 360 degrees
	//it addresses the incline problem (still have to test when incline is more than 90 degrees)
	//Problem to fix: when one angle changing, it affects the other
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
#endif

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
