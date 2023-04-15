#ifndef _KY_40_H_
#define _KY_40_H_ 1

#include "stm8s.h"

// CLK pin
#define KEY_40_PORT GPIOD

/**
 * For read pins. Enter CLK, DT or SW
 */
#define READ_KEY_40(PIN) (GPIO_ReadInputPin(KEY_40_PORT, PIN) == RESET)
struct KEY_40_config
{
  void (*onRight)();
  void (*onLeft)();
  void (*onPush)();
  void (*onOther)();
  GPIO_Pin_TypeDef clk;
  GPIO_Pin_TypeDef dt;
  GPIO_Pin_TypeDef sw;
  GPIO_Pin_TypeDef other;
};

// Functions for encoder
void init_key_40(struct KEY_40_config *config);

#endif