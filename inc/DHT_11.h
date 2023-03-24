#ifndef _DHT_11_
#define _DHT_11_ 1

#include "stm8s.h"

#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_5
#define BTN_PORT GPIOE
#define BTN_PIN GPIO_PIN_4
#define TRIGGER_PORT GPIOB
#define TRIGGER_PIN GPIO_PIN_4
#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_5

#define LOW(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)
#define HIGH(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define REVERSE(BAGR) GPIO_WriteReverse(BAGR##_PORT, BAGR##_PIN)

#define READ(BAGR) GPIO_ReadInputPin(BAGR##_PORT, BAGR##_PIN)
#define PUSH(BAGR) (GPIO_ReadInputPin(BAGR##_PORT, BAGR##_PIN) == RESET)

#define Mindex 99

void init_DHT_11();
void get_data_DHT_11();

#endif