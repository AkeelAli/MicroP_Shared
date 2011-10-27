#include "FIRFilter.h"

#define BL 5
float B[BL] = {
     0.112319462,   0.4295506179,    0.275788486,   0.4295506179,    0.112319462
};

float circBuffx[BL];
float circBuffy[BL];
float circBuffz[BL];
int index_x;
int index_y;
int index_z;


float process_filter(s16 x, int axis)
{
	int k, buff_index = 0;
	float y = 0;

	switch(axis){
				case 0:
				{

					++index_x;
					if (index_x == BL){ index_x = 0;}
					circBuffx[index_x] = x;
					buff_index = index_x;

					for (k = 0; k < BL; k++)
					{
						y += B[k] * circBuffx[buff_index];
						--buff_index;
						
						if (buff_index == -1){ buff_index = BL-1;}
					}
				}
				break;
				
				case 1:
				{

					++index_y;
					if (index_y == BL){index_y= 0;}
					circBuffy[index_y] = x;
					buff_index = index_y;

					for (k = 0; k < BL; k++)
					{
						y += B[k] * circBuffy[buff_index];
						--buff_index;
						
						if (buff_index == -1){ buff_index = BL-1;}
					}
				}
				break;

				case 2:
				{

					++index_z;
					if (index_z == BL){index_z= 0;}
					circBuffz[index_z] = x;
					buff_index = index_z;

					for (k = 0; k < BL; k++)
					{
						y += B[k] * circBuffz[buff_index];
						--buff_index;
						
						if (buff_index == -1){ buff_index = BL-1;}
					}
				}
				break;
		}
	
	return y;
}
