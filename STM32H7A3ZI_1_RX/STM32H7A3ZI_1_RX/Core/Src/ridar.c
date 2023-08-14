#include "ridar.h"


void ridar (void){


	  for(int j=0;j<3;j++)
	  {b[j]=RxData_From_Node1[j]-'0';}

	  int Distance4 = 100* b[0]  +10*b[1] +b[2];//rider
  	  if(Distance4<=20)
  	  {
  		  htim1.Instance->CCR1=0;
  		  htim1.Instance->CCR2=0;
  	  }
}
