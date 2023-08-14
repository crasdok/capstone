#include "main.h"


FDCAN_FilterTypeDef sFilterConfig;
FDCAN_FilterTypeDef sFilterConfig1;
FDCAN_FilterTypeDef sFilterConfig2;
FDCAN_RxHeaderTypeDef RxHeader;
FDCAN_TxHeaderTypeDef TxHeader;
uint8_t TxData_Node3_To_Node1[16];
uint8_t TxData_Node3_To_Node3[16];
uint8_t RxData_From_Node3[16];
uint8_t RxData_From_Node1[16];
uint8_t RxData_From_Node4[8];

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs);
void HAL_FDCAN_RxBufferNewMessageCallback(FDCAN_HandleTypeDef *hfdcan);
