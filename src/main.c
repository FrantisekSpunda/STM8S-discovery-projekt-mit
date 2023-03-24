#include <stdio.h>
#include "stm8s.h"

#include "milis.h"
#include "delay.h"

#include "uart2.h"
#include "key_40.h"
#include "LCD_I2C.h"
#include "DHT_11.h"

void onRight()
{
    // printf("turn right ->\n\r");
}
void onLeft()
{
    // printf("turn left <-\n\r");
}

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // takt MCU na 16MHz

    init_milis();
    init_uart2();
    // init_key_40();
    init_DHT_11();
    // LCD_I2C_Init(0x27, 16, 2);
}

int main(void)
{
    setup();
    /*
    uint8_t encoder1 = 2;
    uint32_t time = 0;
    char text[10];
    */

    uint32_t time = 0;
    uint32_t lasttime = 0;

    uint16_t bagr;

    while (1)
    {
        get_data_DHT_11();
        /*
        encoder1 = position_key_40(&onRight, &onLeft);

        if (milis() - time > 100)
        {
            time = milis();
            sprintf(text, "%d", encoder1);
            LCD_I2C_Clear();
            LCD_I2C_Print(text);
        }
        */
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
