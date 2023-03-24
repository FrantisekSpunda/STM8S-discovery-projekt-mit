#ifndef _KY_40_H_
#define _KY_40_H_ 1

#include "stm8s.h"

// CLK pin
#define CLK_PORT GPIOB
#define CLK_PIN GPIO_PIN_0

// DT pin
#define DT_PORT GPIOB
#define DT_PIN GPIO_PIN_1

// SW pin for button
#define SW_PORT GPIOB
#define SW_PIN GPIO_PIN_2

/**
 * For read pins. Enter CLK, DT or SW
 */
#define READ_KEY_40(PIN) (GPIO_ReadInputPin(PIN##_PORT, PIN##_PIN) == RESET)

// Functions for encoder
void init_key_40();
uint8_t position_key_40(void (*onRight)(), void (*onLeft)());

#endif