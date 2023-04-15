#include "stm8s.h"
#include "milis.h"
#include "key_40.h"
#include <stdio.h>
#include <string.h>
#include "uart2.h"
#include "DHT_11.h"
#include "delay.h"
#include "LCD_I2C.h"
#include "LCD_UI.h"

char *temp_1;
char *temp_2;

struct LCD_UI_config UI_config_global;

int8_t fanLevel = 0;

void UI_fan()
{
    LCD_I2C_Clear();
    LCD_I2C_Print("home: testing value");
}

void UI_temp_1()
{

    LCD_I2C_Clear();

    char printed[16] = "";
    strcat(printed, temp_1);
    LCD_I2C_Print(printed);
}

void KEY_40_onPush(void)
{
    if (UI_config_global.selected == 0)
    {
        // Fan
        // GPIO_WriteReverse(GPIOG, GPIO_PIN_0);
        printf("fan\n");
    }
    else if (UI_config_global.selected == 99)
    {
        select_LCD_UI();
        printf("select\n");
    }
}

void UI_temp_2()
{
    LCD_I2C_Clear();

    char printed[16] = "";
    strcat(printed, temp_2);
    LCD_I2C_Print(printed);
}

struct DHT_11_config DHT_11_first = {
    .trigger = GPIO_PIN_4, // Port C
    .reader = GPIO_PIN_5,  // Port C
};
struct DHT_11_config DHT_11_second = {
    .trigger = GPIO_PIN_6, // Port C
    .reader = GPIO_PIN_7,  // Port C
};

struct KEY_40_config KEY_40_conf = {
    .clk = GPIO_PIN_2,   // Port D
    .dt = GPIO_PIN_3,    // Port D
    .sw = GPIO_PIN_4,    // Port D
    .other = GPIO_PIN_0, // Port D
    .onRight = scroll_up_LCD_UI,
    .onLeft = scroll_down_LCD_UI,
    .onPush = KEY_40_onPush,
    .onOther = home_LCD_UI,
};

struct LCD_UI_config UI_config = {
    .scroll = 0,
    .pages_length = 3,
    .pages = {"Vetrak", "Teplota 1", "Teplota 2"},
    .pages_funcs = {&UI_fan, &UI_temp_1, &UI_temp_2},
};

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
    ITC_SetSoftwarePriority(ITC_IRQ_PORTB, ITC_PRIORITYLEVEL_1);

    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);
    ITC_SetSoftwarePriority(ITC_IRQ_PORTD, ITC_PRIORITYLEVEL_2);

    // povolení přeruření
    enableInterrupts();

    // Fan
    GPIO_Init(GPIOG, GPIO_PIN_0, GPIO_MODE_OUT_OD_HIZ_FAST);

    init_DHT_11(&DHT_11_first);
    init_DHT_11(&DHT_11_second);

    init_key_40(&KEY_40_conf);

    init_milis();
    init_uart2();

    LCD_I2C_Init(0x27, 16, 2);

    init_LCD_UI(&UI_config);
}

int main(void)
{

    setup();

    int32_t time = 0;
    int32_t timeT = 0;
    uint64_t res;

    while (1)
    {
        if (milis() - time > 5000)
        {
            time = milis();
            // LCD_I2C_Clear();

            res = read_DHT_11(&DHT_11_first);
            if (res)
                temp_1 = getValue(res, 'B');
            // LCD_I2C_Print(temp_1);
            // delay_ms(1000);
            res = read_DHT_11(&DHT_11_second);
            if (res)
                temp_2 = getValue(res, 'B');
            // LCD_I2C_Print(DHT_11_second_res);
        }

        if (milis() - timeT > 1000)
        {
            timeT = milis();

            GPIO_WriteReverse(GPIOG, GPIO_PIN_0);
        }

        // Větráček
        // Ukládat do paměti max a min hodnotu
        // Posílat do kompu a zobrazovat v appce

        // Nastavování překlápějící se hodnoty
        // Nastavování rychlosti větráčku
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"