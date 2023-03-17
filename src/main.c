#include <stdio.h>
#include "stm8s.h"

#include "milis.h"
#include "delay.h"

#include "delay.h"
#include "uart2.h"
// #include "key_40.h"
#include "LCD_I2C.h"

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
    LCD_I2C_Init(0x27, 16, 2);

    // init_key_40();
}

int main(void)
{
    setup();
    LCD_I2C_Clear();    
    uint32_t time = 0;

    LCD_I2C_NoBacklight();
    delay_ms(1000);
    LCD_I2C_Backlight();
    delay_ms(1000);
    while (1)
    {
        // uint8_t encoder1 = position_key_40(&onRight, &onLeft);

        if (milis() - time > 500)
        {
            time = milis();
            LCD_I2C_Print("Bobek");
            printf("PIN 0: %i\r\n", 3);
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
