#include "main.h"
#include "NRF24L01.h"
#include "ridar.h"
#include "canfd.h"
#include "buzzer.h"
#include "light_sensor.h"
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

void go_back(void);
void nrf_motor (void);
void rpi_motor (void);

