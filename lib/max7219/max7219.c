#include "max7219.h"

void init_max()
{
  GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
}

void send_max(uint8_t address, uint8_t data)
{
  uint8_t mask;
  LOW(CS);

  mask = 1 << 7; // 0b10000000
  while (mask)
  {
    if (address & mask)
    {
      HIGH(DIN);
    }
    else
    {
      LOW(DIN);
    }
    HIGH(CLK);
    mask = mask >> 1;
    LOW(CLK);
  }

  mask = 1 << 7;
  while (mask)
  {
    if (data & mask)
    {
      HIGH(DIN);
    }
    else
    {
      LOW(DIN);
    }
    HIGH(CLK);
    mask = mask >> 1;
    LOW(CLK);
  }

  HIGH(CS);
}