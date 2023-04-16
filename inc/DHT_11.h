#ifndef __DHT_11_H__
#define __DHT_11_H__

#include "stm8s.h"

#define DHT_11_PORT GPIOB

#define DHT_11_LOW(BAGR) GPIO_WriteLow(DHT_11_PORT, BAGR)
#define DHT_11_HIGH(BAGR) GPIO_WriteHigh(DHT_11_PORT, BAGR)
#define DHT_11_REVERSE(BAGR) GPIO_WriteReverse(DHT_11_PORT, BAGR)

#define DHT_11_READ(BAGR) GPIO_ReadInputPin(DHT_11_PORT, BAGR)
#define DHT_11_PUSH(BAGR) (GPIO_ReadInputPin(DHT_11_PORT, BAGR) == RESET)

typedef enum
{
  WAKE,
  DATA,
  SLEEEP
} state_t;

struct DHT_11_config
{
  GPIO_Pin_TypeDef trigger;
  GPIO_Pin_TypeDef reader;
};

struct DHT_11_response
{
  uint16_t temperature;
  uint8_t humidity;
};

void init_DHT_11(struct DHT_11_config *config);
uint64_t read_DHT_11(struct DHT_11_config *config);
char *getValue(uint64_t data, char value);
void getResponse_DHT_11(struct DHT_11_response *config, uint64_t data);

#endif