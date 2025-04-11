#ifdef __IMAGE_H
#define __IMAGE_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bsp_ili9341_lcd.h"
#include "ff.h" 


//=======================================================================================
//声名函数
void LCD_Color_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
// void LCD_Draw_Picture_Pro(uint16_t *xstr,uint16_t *ystr,uint16_t sendbyte,uint8_t * pic);
void LCD_open_directory(uint16_t xstr, uint16_t ystr, char filepath[]);
void LoadAndDisplayCFile1(uint16_t *xstr,uint16_t *ystr,char filepath[]);
//=======================================================================================
//函数声明结束

#endif /* __IMAGE_H */