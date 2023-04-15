#ifndef _LCD_UI_H_
#define _LCD_UI_H_ 1

#include "stm8s.h"
#include "LCD_I2C.h"

typedef void (*FunctionPointer)();
struct LCD_UI_config
{
  uint8_t scroll;
  uint8_t selected;
  uint8_t pages_length;
  FunctionPointer pages_funcs[3];
  char *pages[3];
};

void init_LCD_UI(struct LCD_UI_config *config);
void render_LCD_UI();
void scroll_up_LCD_UI();
void scroll_down_LCD_UI();
void select_LCD_UI();
void home_LCD_UI();

#endif