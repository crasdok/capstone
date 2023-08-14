#include "light_sensor.h"

void light_sensor (void){

	 for(int l=12;l<=14;l++)
	 {a[l]=RxData_From_Node3[l]-'0';}
	 int jodo = 100* a[12]  +10*a[13] +a[14];
	      htim3.Instance->CCR1=jodo;
	      if (jodo<45)
	      {
	    	  htim3.Instance->CCR1=0;
	      }
}
