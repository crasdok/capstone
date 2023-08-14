#include "canfd.h"



void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
   if(FDCAN1 == hfdcan->Instance)
   {
	  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	  {

		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData_From_Node3) != HAL_OK)
		{
		Error_Handler();
		}

	  }
   }

 }//choumpa jodo
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
   if(FDCAN1 == hfdcan->Instance)
   {
	  if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET)
	  {
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &RxHeader, RxData_From_Node1) != HAL_OK)
		{
		Error_Handler();
		}

	  }
   }
 }//rider
void HAL_FDCAN_RxBufferNewMessageCallback(FDCAN_HandleTypeDef *hfdcan)
{

    if (FDCAN1 == hfdcan->Instance)
    {
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_BUFFER0, &RxHeader, RxData_From_Node4) != HAL_OK)
        {
            Error_Handler();
        }
    }

}//raspi
