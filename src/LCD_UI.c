#include "LCD_UI.h"
#include <string.h>
#include "delay.h"

extern struct LCD_UI_config UI_config_global;

void init_LCD_UI(struct LCD_UI_config *config)
{
  UI_config_global = *config;
  UI_config_global.selected = 99;

  render_LCD_UI();
}

void render_LCD_UI()
{
  LCD_I2C_Clear();
  char printed[16] = "> ";
  strcat(printed, UI_config_global.pages[UI_config_global.scroll]);
  LCD_I2C_Print(printed);
  uint8_t scrollerB = UI_config_global.scroll + 2 > UI_config_global.pages_length ? 0 : UI_config_global.scroll + 1;
  LCD_I2C_SetCursor(2, 1);
  LCD_I2C_Print(UI_config_global.pages[scrollerB]);

  LCD_I2C_SetCursor(0, 0);
  LCD_I2C_blink(1);
}

void scroll_up_LCD_UI()
{
  if (UI_config_global.selected == 99)
  {
    if ((UI_config_global.scroll + 2) > UI_config_global.pages_length)
      UI_config_global.scroll = 0;
    else
      UI_config_global.scroll++;

    render_LCD_UI();
  }
}

void scroll_down_LCD_UI()
{
  if (UI_config_global.selected == 99)
  {
    if ((UI_config_global.scroll - 1) < 0)
      UI_config_global.scroll = UI_config_global.pages_length - 1;
    else
      UI_config_global.scroll--;

    render_LCD_UI();
  }
}

void select_LCD_UI()
{
  if (UI_config_global.selected == 99)
  {
    UI_config_global.selected = UI_config_global.scroll;
    UI_config_global.pages_funcs[UI_config_global.scroll]();
  }
}

void home_LCD_UI()
{
  UI_config_global.selected = 99;
  render_LCD_UI();
}