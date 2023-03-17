#include "stm8s.h"
#include "key_40.h"
#include <stdio.h>

uint8_t position = 0;
uint8_t stateCLKprev;
uint8_t stateCLK;
uint8_t stateSW;
uint8_t stateDT;
// uint8_t timing;

void init_key_40()
{
  GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_IN_PU_IT);
  GPIO_Init(DT_PORT, DT_PIN, GPIO_MODE_IN_PU_IT);
  GPIO_Init(SW_PORT, SW_PIN, GPIO_MODE_IN_PU_IT);

  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSIT  IVITY_RISE_FALL);
  ITC_SetSoftwarePriority(ITC_IRQ_PORTB, ITC_PRIORITYLEVEL_1);

  stateCLKprev = READ(CLK);
}

INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  
}

/**
 * Function for returning position of encoder.
 * MUST BE IN ENDLESS WHILE
 *
 * @return uint8_t 0-255 positions !! 255 is not one round
 */
uint8_t position_key_40(void (*onRight)(), void (*onLeft)())
{
  stateCLK = READ(CLK);

  /*
  * ! asi jen na nástupnou troubo
  */
  if (stateCLK != stateCLKprev)
  {
    stateDT = READ(DT);
    if (stateDT == stateCLK)
    {
      position--;
      onLeft();
    }
    else
    {
      position++;
      onRight();
    }
  }

  stateCLKprev = stateCLK;
  stateSW = READ(SW);

  // uint8_t returnValue[2];
  // returnValue[0] = position;
  // returnValue[1] = stateSW;
  return position;
}