#include <stdio.h>
#include "stm8s.h"
#include "DHT_11.h"
#include "milis.h"
#include "uart2.h"
#include "delay.h"

uint16_t index = 0;
uint16_t times[Mindex];
uint16_t last_counter = 0;
uint64_t data = 0;

typedef enum
{
  WAKE,
  DATA,
  SLEEEP
} state_t;
state_t state = SLEEEP;

// toto bylo v setupu
uint32_t time = 0;
uint32_t lasttime = 0;

uint16_t bagr;

INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  uint16_t pulse_length;

  pulse_length = TIM2_GetCounter() - last_counter;
  last_counter = TIM2_GetCounter(); //  uložím si na příští měření
  if (READ(DHT11) == RESET)
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

void init_DHT_11()
{
  GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(DHT11_PORT, DHT11_PIN, GPIO_MODE_IN_PU_IT);
  GPIO_Init(TRIGGER_PORT, TRIGGER_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);

  // nastavení citlivosti externího přerušení přerušení
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
  // nastavení priority přerušení
  ITC_SetSoftwarePriority(ITC_IRQ_PORTE, ITC_PRIORITYLEVEL_1);

  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 0xFFFF);
  TIM2_Cmd(ENABLE);

  // povolení přeruření
  enableInterrupts();

  init_milis();
  init_uart2();
}

/**
 * @brief Get the data DHT 11 object.
 * @brief Must be in main loop.
 *
 * @return uint16_t temprature and ...
 */
void get_data_DHT_11()
{
  if (milis() - time > 4000 /*&& !PUSH(BTN)*/)
  {
    REVERSE(LED);
    time = milis();
    bagr = TIM2_GetCounter();
    printf("\n\rJedem...%u\n\r", bagr);
    state = WAKE;
  }

  switch (state)
  {
  case SLEEEP:
    lasttime = milis();
    break;

  case WAKE:
    if (milis() - lasttime < 19)
    {
      LOW(TRIGGER);
    }
    else
    {
      lasttime = milis();
      TIM2_SetCounter(0);
      HIGH(TRIGGER);
      last_counter = 0;
      index = 0;
      data = 0LL;
      state = DATA;
    }
    break;

  case DATA:
    if (milis() - lasttime > 1000)
    {
      lasttime = milis();
      for (int i = 0; i < index; ++i)
      {
        printf("%d: %d, ", i, times[i]);
      }
      printf("\n\rdata: 0b ");
      uint64_t m = 1LL << 39;
      uint8_t i = 0;
      while (m)
      {
        if (data & m)
        {
          putchar('1');
        }
        else
        {
          putchar('0');
        }
        if (++i % 8 == 0)
          putchar(' ');
        m >>= 1;
      }
      printf("\n\r");
      uint8_t humidityH = data >> 32;
      uint8_t humidityL = data >> 24;
      uint8_t temperatureH = data >> 16;
      uint8_t temperatureL = data >> 8;
      uint8_t checksum = data;
      printf("data: %8d %8d %8d %8d\n\r", humidityH, humidityL, temperatureH, temperatureL);
      printf("checksum: ");
      printf(humidityH + humidityL + temperatureH + temperatureL == checksum ? ":-)" : ";-(");
      printf("\n\r");
      printf("vlhkost: %d.%d, teplota: %d.%d °C\n\r", humidityH, humidityL, temperatureH, temperatureL);

      state = SLEEEP;
    }
    break;

  default:
    state = SLEEEP;
    break;
  }
}