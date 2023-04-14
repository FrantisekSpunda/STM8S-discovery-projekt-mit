#ifndef __DHT_11_H__
#define __DHT_11_H__

#include "stm8s.h"

#define TRIGGER_PORT GPIOB
#define TRIGGER_PIN GPIO_PIN_4

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_5


#define TRIGGER_PORT GPIOB
#define TRIGGER_PIN GPIO_PIN_4

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_5

#define LOW(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)
#define HIGH(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define REVERSE(BAGR) GPIO_WriteReverse(BAGR##_PORT, BAGR##_PIN)

#define READ(BAGR) GPIO_ReadInputPin(BAGR##_PORT, BAGR##_PIN)
#define PUSH(BAGR) (GPIO_ReadInputPin(BAGR##_PORT, BAGR##_PIN)==RESET)
#define Mindex 99

typedef enum { WAKE, DATA, SLEEEP } state_t;

void init_DHT_11();
uint64_t read_DHT_11();
char * getValue(uint64_t data, char value);


#endif