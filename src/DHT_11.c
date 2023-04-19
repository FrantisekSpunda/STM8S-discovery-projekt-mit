#include "DHT_11.h"
#include "milis.h"
#include <stdio.h>
#include <stdlib.h>

uint16_t index = 0;
uint16_t times[99];
uint16_t last_counter = 0;
uint64_t data = 0;

struct DHT_11_config DHT_11_config_global;

void init_DHT_11(struct DHT_11_config *config)
{
  GPIO_Init(DHT_11_PORT, config->trigger, GPIO_MODE_OUT_OD_HIZ_SLOW);
  GPIO_Init(DHT_11_PORT, config->reader, GPIO_MODE_IN_PU_IT);

  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 0xFFFF);
  TIM2_Cmd(ENABLE);
}

uint64_t read_DHT_11(struct DHT_11_config *config)
{
  DHT_11_config_global = *config;

  uint32_t lasttime = milis();
  TIM2_SetCounter(0);
  last_counter = 0;
  index = 0;
  data = 0LL;
  state_t state = WAKE;
  uint64_t response = 0;

  while (state != SLEEEP)
  {
    switch (state)
    {
    case SLEEEP:
      lasttime = milis();
      break;

    case WAKE:
      if (milis() - lasttime < 40)
      {
        DHT_11_LOW(config->trigger);
      }
      else
      {
        lasttime = milis();
        DHT_11_HIGH(config->trigger);
        state = DATA;
      }
      break;

    case DATA:
      if (milis() - lasttime > 6)
      {
        lasttime = milis();
        uint8_t humidityH = data >> 32;
        uint8_t humidityL = data >> 24;
        uint8_t temperatureH = data >> 16;
        uint8_t temperatureL = data >> 8;
        uint8_t checksum = data;
        if (humidityH + humidityL + temperatureH + temperatureL == checksum)
          response = data;
        state = SLEEEP;
      }
      break;

    default:
      state = SLEEEP;
      break;
    }
  }

  return response;
}

char *getValue(uint64_t data, char value)
{
  uint8_t tempH = data >> 16;
  uint8_t tempL = data >> 8;
  uint8_t humidityH = data >> 32;

  char *myString = (char *)malloc(sizeof(char) * 20);
  switch (value)
  {
  case 'T':
    sprintf(myString, "%d.%d C", tempH, tempL);
    return myString;

  case 'H':
    sprintf(myString, "%d %%", humidityH);
    return myString;
  case 'B':
    sprintf(myString, "%d.%d C %d %%", tempH, tempL, humidityH);
    return myString;
  default:
    return "";
  }
}

void getResponse_DHT_11(struct DHT_11_response *config, uint64_t data)
{
  uint16_t temperature = data >> 8;
  uint8_t humidityH = data >> 32;

  config->temperature = temperature;
  config->humidity = humidityH;
}

INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  uint16_t pulse_length;

  pulse_length = TIM2_GetCounter() - last_counter;
  last_counter = TIM2_GetCounter(); //  uložím si na příští měření
  if (DHT_11_READ(DHT_11_config_global.reader) == RESET)
  { // Log 0
    if (pulse_length > 15 && pulse_length < 30)
    {
      data = data << 1;
      times[index++] = pulse_length;
    }
    if (pulse_length > 40 && pulse_length < 74)
    { // Log 1
      data <<= 1;
      data = data | 1;
      times[index++] = pulse_length;
    }
  }
}