#include "stm8s.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "milis.h"
#include "key_40.h"
#include "uart2.h"
#include "DHT_11.h"
#include "delay.h"
#include "LCD_I2C.h"
#include "LCD_UI.h"
#include "EEPROM_save.h"

struct LCD_UI_config UI_config_global;

// Global store of important variables. Lots of them are saved to EEPROM
struct EEPROM_DATA
{
    int8_t fanLevel;
    int8_t setLevel;
    uint64_t curr_temp_1;
    uint16_t max_temp_1;
    uint16_t min_temp_1;
    uint64_t curr_temp_2;
    uint16_t max_temp_2;
    uint16_t min_temp_2;
};
struct EEPROM_DATA dataStore = {
    .fanLevel = 0,
    .setLevel = 0,
    .curr_temp_1 = 0,
    .max_temp_1 = 0,
    .min_temp_1 = 0xffffffff,
    .curr_temp_2 = 0,
    .max_temp_2 = 0,
    .min_temp_2 = 0xffffffff,
};
void saveDataStore()
{
    int8_t dataWrite[10] = {
        dataStore.fanLevel,
        dataStore.setLevel,

        (uint8_t)(dataStore.max_temp_1 >> 8),
        (uint8_t)dataStore.max_temp_1,

        (uint8_t)(dataStore.min_temp_1 >> 8),
        (uint8_t)dataStore.min_temp_1,

        (uint8_t)(dataStore.max_temp_2 >> 8),
        (uint8_t)dataStore.max_temp_2,

        (uint8_t)(dataStore.min_temp_2 >> 8),
        (uint8_t)dataStore.min_temp_2,
    };
    write_eeprom(0, dataWrite, 10);
}

void loadDataStore()
{
    uint16_t temp_min;

    int8_t *dataRead = read_eeprom(0, 10);
    dataStore.fanLevel = dataRead[0];
    dataStore.setLevel = dataRead[1];
    dataStore.max_temp_1 = (uint16_t)dataRead[2] << 8 | dataRead[3];
    temp_min = (uint16_t)dataRead[4] << 8 | dataRead[5];
    if (temp_min)
        dataStore.min_temp_1 = temp_min;
    dataStore.max_temp_2 = (uint16_t)dataRead[6] << 8 | dataRead[7];
    temp_min = (uint16_t)dataRead[8] << 8 | dataRead[9];
    if (temp_min)
        dataStore.min_temp_2 = temp_min;

    free(dataRead);
}

// Variables for conditional rendering
int8_t scrollLevel;
uint16_t current_max_temp;

// * Functions for UI RENDERING
void UI_fan()
{
    LCD_I2C_Clear();
    if (dataStore.fanLevel == 2)
        LCD_I2C_Print("Fan: Automatic");
    else if (dataStore.fanLevel == 1)
        LCD_I2C_Print("Fan: On");
    else
        LCD_I2C_Print("Fan: Off");

    saveDataStore();
}
void UI_set_point()
{
    LCD_I2C_Clear();
    char printed[16];
    sprintf(printed, "Set Point: %d", dataStore.setLevel);
    LCD_I2C_Print(printed);
    saveDataStore();
}
void UI_temp_1()
{
    LCD_I2C_Clear();

    switch (scrollLevel)
    {
    case 0:
    {
        char row_1[16] = "Temperature";
        LCD_I2C_Print(row_1);

        LCD_I2C_SetCursor(0, 1);

        char row_2[16] = "> ";
        char *current;
        current = getValue(dataStore.curr_temp_1, 'B');
        strcat(row_2, current);
        LCD_I2C_Print(row_2);

        break;
    }
    case 1:
    {
        char row_1[16] = "Max > ";
        char *maximal;
        maximal = getValue(((uint64_t)dataStore.max_temp_1) << 8, 'T');
        strcat(row_1, maximal);
        LCD_I2C_Print(row_1);

        LCD_I2C_SetCursor(0, 1);

        char row_2[16] = "Min > ";
        char *minimal;
        minimal = getValue(((uint64_t)dataStore.min_temp_1) << 8, 'T');
        strcat(row_2, minimal);
        LCD_I2C_Print(row_2);

        break;
    }
    }
}
void UI_temp_2()
{
    LCD_I2C_Clear();

    switch (scrollLevel)
    {
    case 0:
    {
        char row_1[16] = "Temperature";
        LCD_I2C_Print(row_1);

        LCD_I2C_SetCursor(0, 1);

        char row_2[16] = "> ";
        char *current;
        current = getValue(dataStore.curr_temp_2, 'B');
        strcat(row_2, current);
        LCD_I2C_Print(row_2);

        break;
    }
    case 1:
    {
        char row_1[16] = "Max > ";
        char *maximal;
        maximal = getValue(((uint64_t)dataStore.max_temp_2) << 8, 'T');
        strcat(row_1, maximal);
        LCD_I2C_Print(row_1);

        LCD_I2C_SetCursor(0, 1);

        char row_2[16] = "Min > ";
        char *minimal;
        minimal = getValue(((uint64_t)dataStore.min_temp_2) << 8, 'T');
        strcat(row_2, minimal);
        LCD_I2C_Print(row_2);

        break;
    }
    }
}
void UI_reset()
{
    LCD_I2C_Clear();
    LCD_I2C_Print("Confirm");
}
// -----------------

// * Functions for UI EVENTS
void KEY_40_onPush()
{
    switch (UI_config_global.selected)
    {
    case 4:
    {
        LCD_I2C_Clear();
        dataStore.fanLevel = 0,
        dataStore.setLevel = 0,
        dataStore.curr_temp_1 = 0,
        dataStore.max_temp_1 = 0,
        dataStore.min_temp_1 = 0xffffffff,
        dataStore.curr_temp_2 = 0,
        dataStore.max_temp_2 = 0,
        dataStore.min_temp_2 = 0xffffffff,
        saveDataStore();
        LCD_I2C_Print("Cleared!!");
    }
    case 99:
        scrollLevel = 0;
        select_LCD_UI();
        break;
    }
}
void KEY_40_onRight()
{
    switch (UI_config_global.selected)
    {
    case 0: // Fan
        dataStore.fanLevel++;
        if (dataStore.fanLevel > 2)
            dataStore.fanLevel = 0;
        UI_fan();
        break;
    case 1: // Set Point
        dataStore.setLevel++;
        if (dataStore.setLevel > 120)
            dataStore.setLevel = 0;
        UI_set_point();
        break;
    case 2: // Temperature 1
        scrollLevel++;
        if (scrollLevel > 1)
            scrollLevel = 0;
        UI_temp_1();
        break;
    case 3: // Temperature 2
        scrollLevel++;
        if (scrollLevel > 1)
            scrollLevel = 0;
        UI_temp_2();
        break;
    case 99:
        scroll_up_LCD_UI();
        break;
    }
}
void KEY_40_onLeft()
{
    switch (UI_config_global.selected)
    {
    case 0: // Fan
        dataStore.fanLevel--;
        if (dataStore.fanLevel < 0)
            dataStore.fanLevel = 2;
        UI_fan();
        break;
    case 1: // Set Point
        dataStore.setLevel--;
        if (dataStore.setLevel < 0)
            dataStore.setLevel = 120;
        UI_set_point();
        break;
    case 2: // Temperature 1
        scrollLevel--;
        if (scrollLevel < 0)
            scrollLevel = 1;
        UI_temp_1();
        break;
    case 3: // Temperature 2
        scrollLevel--;
        if (scrollLevel < 0)
            scrollLevel = 1;
        UI_temp_2();
        break;
    case 99:
        scroll_down_LCD_UI();
    }
}
// -----------------

// I/O configuration
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
    .onRight = KEY_40_onRight,
    .onLeft = KEY_40_onLeft,
    .onPush = KEY_40_onPush,
    .onOther = home_LCD_UI,
};

// UI configuration
struct LCD_UI_config UI_config = {
    .scroll = 0,
    .pages_length = 5,
    .pages = {"Fan", "Set Point", "Temperature 1", "Temperature 2", "Reset memmory"},
    .pages_funcs = {&UI_fan, &UI_set_point, &UI_temp_1, &UI_temp_2, &UI_reset},
};

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    // Innterupt for sensors on level 2
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
    ITC_SetSoftwarePriority(ITC_IRQ_PORTB, ITC_PRIORITYLEVEL_2);

    // Innterupt for encoder on highest level
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);
    ITC_SetSoftwarePriority(ITC_IRQ_PORTD, ITC_PRIORITYLEVEL_1);

    enableInterrupts();

    init_milis();
    init_uart2();
    init_eeprom();

    // * I/O inicialization
    // Sensors
    init_DHT_11(&DHT_11_first);
    init_DHT_11(&DHT_11_second);
    // Encoder
    init_key_40(&KEY_40_conf);
    // LCD
    LCD_I2C_Init(0x27, 16, 2);
    init_LCD_UI(&UI_config);
    // Fan
    GPIO_Init(GPIOG, GPIO_PIN_0, GPIO_MODE_OUT_OD_HIZ_FAST);

    // Load data from EEPROM to struct dataStore
    loadDataStore();
}

int main(void)
{

    setup();

    // Variables for timing and saving.
    int32_t time = 0;
    int32_t time_2 = 0;
    uint64_t res;

    while (1)
    {
        if (milis() - time > 1000)
        {
            time = milis();
            struct DHT_11_response response = {0, 0, 0};
            struct DHT_11_response aditional_res = {0, 0, 0};

            // Read data from sensors and save max and min values
            res = read_DHT_11(&DHT_11_first);
            if (res)
            {
                dataStore.curr_temp_1 = res;

                getResponse_DHT_11(&response, res);

                // Read for set level (😥 not ideal but works)
                current_max_temp = response.temperature;

                // Set max and min values
                if (dataStore.max_temp_1 < response.temperature)
                    dataStore.max_temp_1 = response.temperature;

                if (dataStore.min_temp_1 > response.temperature)
                    dataStore.min_temp_1 = response.temperature;
            }

            res = read_DHT_11(&DHT_11_second);
            if (res)
            {
                dataStore.curr_temp_2 = res;

                getResponse_DHT_11(&response, res);

                // Read for set level (😥 not ideal but works)
                if (response.temperature > current_max_temp)
                    current_max_temp = response.temperature;

                // Set max and min values
                if (dataStore.max_temp_2 < response.temperature)
                    dataStore.max_temp_2 = response.temperature;

                if (dataStore.min_temp_2 > response.temperature)
                    dataStore.min_temp_2 = response.temperature;
            }
        }

        if (milis() - time_2 > 10000)
        {
            time_2 = milis();
            saveDataStore();
        }

        // Fun on/off depends on fanLevel and setLevel
        if (dataStore.fanLevel == 0)
            GPIO_WriteHigh(GPIOG, GPIO_PIN_0);
        else if (dataStore.fanLevel == 1)
            GPIO_WriteLow(GPIOG, GPIO_PIN_0);
        else if (dataStore.fanLevel == 2)
        {
            uint8_t tempH = current_max_temp >> 8;
            if (tempH > dataStore.setLevel)
                GPIO_WriteLow(GPIOG, GPIO_PIN_0);
            else
                GPIO_WriteHigh(GPIOG, GPIO_PIN_0);
        }

        // Ukládat do paměti max a min hodnotu
        // Posílat do kompu a zobrazovat v appce

        // jak poznat že bylo připojené napětí abych zavolal funkci setup()?
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"