#ifndef _MAX7219_H_
#define _MAX7219_H_ 1

#include "stm8s.h"

#define DIN_PORT GPIOD // data in
#define DIN_PIN GPIO_PIN_4
#define CS_PORT GPIOD // chip select
#define CS_PIN GPIO_PIN_5
#define CLK_PORT GPIOD // clock
#define CLK_PIN GPIO_PIN_6

#define LOW(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)
#define HIGH(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define REVERSE(BAGR) GPIO_WriteReverse(BAGR##_PORT, BAGR##_PIN)

/////////////
#define NOOP 0          // No operation
#define DIGIT0 1        // zápis hodnoty na 1. cifru
#define DIGIT1 2        // zápis hodnoty na 1. cifru
#define DIGIT2 3        // zápis hodnoty na 1. cifru
#define DIGIT3 4        // zápis hodnoty na 1. cifru
#define DIGIT4 5        // zápis hodnoty na 1. cifru
#define DIGIT5 6        // zápis hodnoty na 1. cifru
#define DIGIT6 7        // zápis hodnoty na 1. cifru
#define DIGIT7 8        // zápis hodnoty na 1. cifru
#define DECODE_MODE 9   // Aktivace/Deaktivace znakové sady (my volíme vždy hodnotu DECODE_ALL)
#define INTENSITY 10    // Nastavení jasu - argument je číslo 0 až 15 (větší číslo větší jas)
#define SCAN_LIMIT 11   // Volba počtu cifer (velikosti displeje) - argument je číslo 0 až 7 (my dáváme vždy 7)
#define SHUTDOWN 12     // Aktivace/Deaktivace displeje (ON / OFF)
#define DISPLAY_TEST 15 // Aktivace/Deaktivace "testu" (rozsvítí všechny segmenty)
                        //
/*----- makra argumentů  --- */

// argumenty pro SHUTDOWN
#define DISPLAY_ON 1  // zapne displej
#define DISPLAY_OFF 0 // vypne displej
// argumenty pro DISPLAY_TEST
#define DISPLAY_TEST_ON 1  // zapne test displeje
#define DISPLAY_TEST_OFF 0 // vypne test displeje
// argumenty pro DECODE_MOD
#define DECODE_ALL 0b11111111 // (lepší zápis 0xff) zapíná znakovou sadu pro všechny cifry
#define DECODE_NONE 0         // vypíná znakovou sadu pro všechny cifry
/////////////

void send_max(uint8_t address, uint8_t data);
void init_max();

#endif