#include <stdio.h>
#include "stm8s.h"

#include "milis.h"
#include "delay.h"

// #include "delay.h"
#include "uart2.h"
#include "key_40.h"

void onRight()
{
    printf("turn right");
}
void onLeft()
{
    printf("turn left");
}

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // takt MCU na 16MHz

    init_milis();
    init_uart2();
    init_key_40();
}

int main(void)
{
    setup();

    uint32_t time = 0;

    while (1)
    {
        uint8_t encoder1 = position_key_40(&onRight, &onLeft);

        if (milis() - time > 500)
        {
            time = milis();
            printf("PIN 0: %i\r\n", encoder1);
        }

        // if (milis() - time > 500)
        // {
        //
        //     time = milis();
        // }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
