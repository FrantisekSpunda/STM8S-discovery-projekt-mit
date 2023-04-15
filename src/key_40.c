#include <stdio.h>
#include "stm8s.h"
#include "key_40.h"

uint8_t stateCLKprev;
uint8_t stateCLK;
uint8_t stateSWprev;
uint8_t stateSW;
uint8_t stateOTHERprev;
uint8_t stateOTHER;
uint8_t stateDT;

struct KEY_40_config KEY_40_config_global;

INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  // Rotating logic
  stateCLK = READ_KEY_40(KEY_40_config_global.clk);
  if (stateCLK != stateCLKprev)
  {
    stateDT = READ_KEY_40(KEY_40_config_global.dt);
    if (stateDT == stateCLK)
    {
      KEY_40_config_global.onLeft();
    }
    else
    {
      KEY_40_config_global.onRight();
    }
  }
  stateCLKprev = stateCLK;

  // Push logic
  stateSWprev = stateSW;
  stateSW = READ_KEY_40(KEY_40_config_global.sw);
  if (stateSW && stateSW != stateSWprev)
  {
    KEY_40_config_global.onPush();
  }

  // ! OTHER -------------------------------
  stateOTHERprev = stateOTHER;
  stateOTHER = READ_KEY_40(KEY_40_config_global.other);
  if (stateOTHER && stateOTHER != stateOTHERprev)
  {
    KEY_40_config_global.onOther();
  }
}

void init_key_40(struct KEY_40_config *config)
{
  GPIO_Init(KEY_40_PORT, config->clk, GPIO_MODE_IN_PU_IT);
  GPIO_Init(KEY_40_PORT, config->dt, GPIO_MODE_IN_PU_IT);
  GPIO_Init(KEY_40_PORT, config->sw, GPIO_MODE_IN_PU_IT);
  // ! OTHER -------------------------------
  GPIO_Init(KEY_40_PORT, config->other, GPIO_MODE_IN_PU_IT);

  KEY_40_config_global = *config;

  stateCLKprev = READ_KEY_40(config->clk);
}