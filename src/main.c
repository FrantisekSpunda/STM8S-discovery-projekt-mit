#include "stm8s.h"
#include "milis.h"

// #include "delay.h"
#include <stdio.h>
#include "uart2.h"
#include "DHT_11.h"
#include "delay.h"
#include "LCD_I2C.h"


void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    // nastavení citlivosti externího přerušení přerušení
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
    // nastavení priority přerušení
    ITC_SetSoftwarePriority(ITC_IRQ_PORTE, ITC_PRIORITYLEVEL_1);


    // povolení přeruření
    enableInterrupts();

    init_milis();
    init_uart2();
    init_DHT_11();
    LCD_I2C_Init(0x27, 16, 2);
}

int main(void)
{


    setup();
    LCD_I2C_Clear();

    LCD_I2C_NoBacklight();
    delay_ms(1000);
    LCD_I2C_Backlight();
    delay_ms(1000);

    int32_t time = 0;

    while (1)
    {
        if (milis() - time > 5000) {
            time = milis();
            uint64_t res;
            res = read_DHT_11();
            uint8_t temperatureH = res >> 16;
            uint8_t temperatureL = res >> 8;
            char* myRes = getValue(res, 'T');
            printf("Nice %s\n\r", myRes);
            LCD_I2C_Clear();
            LCD_I2C_Print(myRes);
        }

        // Druhý teploměr
        // Větráček
        // Ukládat do paměti max a min hodnotu
        // Posílat do kompu a zobrazovat v appce
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"