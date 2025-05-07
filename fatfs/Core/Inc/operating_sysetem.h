#ifndef __OPERATING_SYSTEM_H
#define __OPERATING_SYSTEM_H

#include "main.h"
#include "bsp_xpt2046_lcd.h"
  
typedef char TCHAR;

void toggle_state(uint8_t *state);
void display_picture(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image);
void main_page();
void gesture_page();
void show_page();
void gesture_dir_arrow(const TCHAR* path);



















#endif /*  __OPERATING_SYSTEM_H  */