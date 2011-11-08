#include <stdio.h>
#include <stm32f10x.h>
#include "LSM303DLH.h"
#include <math.h>
#include <stm32f10x.h>


#define TO_DEG 57.29578

// Raw data arrays
s16 acc_data[3];
s16 magn_data[3];
double alpha, beta;


//Arctan LUT variables
int index;
double res;
double value;

void AM_Configuration(void);
void update_data(void);


double arctan(double value){
	
	
	int sign=1;
	double ans=0.0;
	return atan(value)*TO_DEG;
	if (value < 0){
		sign=-1;
		value*=-1;
	}	

	if (value == 0)
		ans=0.0;
	else if (value <= 0.017)
	    ans=1.0;
	else if (value <= 0.035)
	    ans=2.0;
	else if (value <= 0.052)
	    ans=3.0;
	else if (value <= 0.070)
	    ans=4.0;
	else if (value <= 0.087)
	    ans=5.0;
	else if (value <= 0.105)
	    ans=6.0;
	else if (value <= 0.123)
	    ans=7.0;
	else if (value <= 0.141)
	    ans=8.0;
	else if (value <= 0.158)
	    ans=9.0;
	else if (value <= 0.176)
	    ans=10.0;
	else if (value <= 0.194)
	    ans=11.0;
	else if (value <= 0.213)
	    ans=12.0;
	else if (value <= 0.231)
	    ans=13.0;
	else if (value <= 0.249)
	    ans=14.0;
	else if (value <= 0.268)
	    ans=15.0;
	else if (value <= 0.287)
	    ans=16.0;
	else if (value <= 0.306)
	    ans=17.0;
	else if (value <= 0.325)
	    ans=18.0;
	else if (value <= 0.344)
	    ans=19.0;
	else if (value <= 0.364)
	    ans=20.0;
	else if (value <= 0.384)
	    ans=21.0;
	else if (value <= 0.404)
	    ans=22.0;
	else if (value <= 0.424)
	    ans=23.0;
	else if (value <= 0.445)
	    ans=24.0;
	else if (value <= 0.466)
	    ans=25.0;
	else if (value <= 0.488)
	    ans=26.0;
	else if (value <= 0.510)
	    ans=27.0;
	else if (value <= 0.532)
	    ans=28.0;
	else if (value <= 0.554)
	    ans=29.0;
	else if (value <= 0.577)
	    ans=30.0;
	else if (value <= 0.601)
	    ans=31.0;
	else if (value <= 0.625)
	    ans=32.0;
	else if (value <= 0.649)
	    ans=33.0;
	else if (value <= 0.675)
	    ans=34.0;
	else if (value <= 0.700)
	    ans=35.0;
	else if (value <= 0.727)
	    ans=36.0;
	else if (value <= 0.754)
	    ans=37.0;
	else if (value <= 0.781)
	    ans=38.0;
	else if (value <= 0.810)
	    ans=39.0;
	else if (value <= 0.839)
	    ans=40.0;
	else if (value <= 0.869)
	    ans=41.0;
	else if (value <= 0.900)
	    ans=42.0;
	else if (value <= 0.933)
	    ans=43.0;
	else if (value <= 0.966)
	    ans=44.0;
	else if (value <= 1.000)
	    ans=45.0;
	else if (value <= 1.036)
	    ans=46.0;
	else if (value <= 1.072)
	    ans=47.0;
	else if (value <= 1.111)
	    ans=48.0;
	else if (value <= 1.150)
	    ans=49.0;
	else if (value <= 1.192)
	    ans=50.0;
	else if (value <= 1.235)
	    ans=51.0;
	else if (value <= 1.280)
	    ans=52.0;
	else if (value <= 1.327)
	    ans=53.0;
	else if (value <= 1.376)
	    ans=54.0;
	else if (value <= 1.428)
	    ans=55.0;
	else if (value <= 1.483)
	    ans=56.0;
	else if (value <= 1.540)
	    ans=57.0;
	else if (value <= 1.600)
	    ans=58.0;
	else if (value <= 1.664)
	    ans=59.0;
	else if (value <= 1.732)
	    ans=60.0;
	else if (value <= 1.804)
	    ans=61.0;
	else if (value <= 1.881)
	    ans=62.0;
	else if (value <= 1.963)
	    ans=63.0;
	else if (value <= 2.050)
	    ans=64.0;
	else if (value <= 2.145)
	    ans=65.0;
	else if (value <= 2.246)
	    ans=66.0;
	else if (value <= 2.356)
	    ans=67.0;
	else if (value <= 2.475)
	    ans=68.0;
	else if (value <= 2.605)
	    ans=69.0;
	else if (value <= 2.747)
	    ans=70.0;
	else if (value <= 2.904)
	    ans=71.0;
	else if (value <= 3.078)
	    ans=72.0;
	else if (value <= 3.271)
	    ans=73.0;
	else if (value <= 3.487)
	    ans=74.0;
	else if (value <= 3.732)
	    ans=75.0;
	else if (value <= 4.011)
	    ans=76.0;
	else if (value <= 4.331)
	    ans=77.0;
	else if (value <= 4.705)
	    ans=78.0;
	else if (value <= 5.145)
	    ans=79.0;
	else if (value <= 5.671)
	    ans=80.0;
	else if (value <= 6.314)
	    ans=81.0;
	else if (value <= 7.115)
	    ans=82.0;
	else if (value <= 8.144)
	    ans=83.0;
	else if (value <= 9.514)
	    ans=84.0;
	else if (value <= 11.430)
	    ans=85.0;
	else if (value <= 14.301)
	    ans=86.0;
	else if (value <= 19.081)
	    ans=87.0;
	else if (value <= 28.636)
	    ans=88.0;
	else if (value <= 57.290)
	    ans=89.0;
	else
	    ans=90.0;

	return ans*sign;
}

int main(void) {
	

	//Initialize the sensor
    I2C_DeInit(I2C1);						  			// Deinit I2C1
	I2C_DeInit(I2C2);									// Deinit I2C2
	AM_Configuration();
	//Enter an infinite loop to measure from the sensor
	while(1)
		{
			update_data();

			alpha=arctan(acc_data[0]/sqrt( (acc_data[1]*acc_data[1]+acc_data[2]*acc_data[2]) *1.0 ));
			beta=arctan(acc_data[1]/sqrt(  (acc_data[0]*acc_data[0]+acc_data[2]*acc_data[2])  *1.0 ));


		//Convert Reading to Degrees

		//Apply filter to the data
		}

}

// Initialize Accelerometer/Magnetometer Settings.
void AM_Configuration() {

	// Allocate config and init structs
   	LSM_Acc_ConfigTypeDef acc_conf;
	LSM_Magn_ConfigTypeDef magn_conf;

	LSM303DLH_I2C_Init();						   		// Init I2C bus

	// Initialize Accelerometer Settings
	acc_conf.Power_Mode = LSM_Acc_Lowpower_NormalMode;	// Set Power Mode to normal to use the normal output rate
	acc_conf.ODR = LSM_Acc_ODR_50;				   		// 50 Hz output normal
	acc_conf.Axes_Enable = LSM_Acc_XYZEN;				// Enable XYZ axes
	acc_conf.FS = LSM_Acc_FS_2;							// 2 g full-scale for better precision
	acc_conf.Data_Update = LSM_Acc_BDU_Single;		   	// Single data output s.t. high and lows are changed 
														// at the same time as recommended
	acc_conf.Endianess = LSM_Acc_Big_Endian;	   		// Big Endian for simpler display of values

	// Initialize Magnetometer Settings
	magn_conf.M_ODR = LSM_Magn_ODR_75;				    // Output Data Rate 75Hz
	magn_conf.Meas_Conf = LSM_Magn_MEASCONF_NORMAL;    	// Normal bias Measurements
	magn_conf.Gain = LSM_Magn_GAIN_1_3;			    	// 1.3 gauss scale
	magn_conf.Mode = LSM_Magn_MODE_CONTINUOS;		   	// Continuous Update
	
	LSM303DLH_Acc_Reboot_Cmd();					   		// Reboot memory to make sure it is empty
	LSM303DLH_Acc_Config(&acc_conf);				   	// Configure Accelerometer with all selected values above
	LSM303DLH_Magn_Config(&magn_conf);					// Configure Magnetometer with all selected values above
}


// Process raw data from peripherals
void update_data() {
	LSM303DLH_Acc_Read_Acc(acc_data);						// Read accelerometer data
	LSM303DLH_Magn_Read_Magn(magn_data);					// Read magnetometer data	
}



