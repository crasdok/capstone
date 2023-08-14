#include "buzzer.h"
enum notes {
  C = 956,
  D = 852,
  E = 758,
  F = 716,
  G = 638
};
void buzzer (void){

	  for(int i=0;i<11;i++)
	  {a[i]=RxData_From_Node3[i]-'0';}

	  int Distance1 = 100* a[1]  +10*a[2] +a[3];
	  int Distance2 = 100* a[4]  +10*a[5] +a[6];
	  int Distance3 = 100* a[8]  +10*a[9] +a[10];
    if (Distance1 <= 15 || Distance2 <= 15 || Distance3 <= 15)
    {
	 	  TIM2->ARR = C;
	 	  TIM2->CCR1 = TIM2->ARR / 2;
	 	  HAL_Delay(50);
	 	  TIM2->CCR1 = 0;
	 	  HAL_Delay(50);
	 	  }
}

