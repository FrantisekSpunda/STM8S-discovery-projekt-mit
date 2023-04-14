#include "DHT_11.h"
#include "milis.h"
#include <stdio.h>
#include <stdlib.h>

uint16_t index = 0;
uint16_t times[Mindex];
uint16_t last_counter = 0;
uint64_t data = 0;

void init_DHT_11 (void) {
    GPIO_Init(TRIGGER_PORT, TRIGGER_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_Init(DHT11_PORT, DHT11_PIN, GPIO_MODE_IN_PU_IT);

    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 0xFFFF);
    TIM2_Cmd(ENABLE);
}


uint64_t read_DHT_11() {
    uint32_t lasttime = milis();
    TIM2_SetCounter(0);
    last_counter = 0;
    index = 0;
    data = 0LL;
    state_t state = WAKE;
    uint64_t response = 0;

    while(state != SLEEEP) {
        switch (state)
        {
        case SLEEEP:
            lasttime = milis();
            break;

        case WAKE:
            if (milis() - lasttime < 40)
            {
                LOW(TRIGGER);
            }
            else
            {
                lasttime = milis();
                HIGH(TRIGGER);
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
                printf("vlhkost: %d %%, teplota: %d.%d °C\n\r", humidityH,
                        temperatureH, temperatureL);
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

char * getValue(uint64_t data, char value)
{
    uint8_t tempH = data >> 16;
    uint8_t tempL = data >> 8;
    uint8_t humidityH = data >> 32;
    uint8_t humidityL = data >> 24;
    char* myString = (char*)malloc(sizeof(char) * 20);
    switch(value) {
        case 'T':
            sprintf(myString, "%d.%d", tempH, tempL);
            return myString;
            
        case 'H':
            sprintf(myString, "%d %%", humidityH);
            return myString;
    }
}

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