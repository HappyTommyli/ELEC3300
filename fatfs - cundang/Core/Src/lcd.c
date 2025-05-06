#include "lcd.h"
#include "main.h"

void Draw_Point(uint16_t x,uint16_t y,uint16_t color)
{
	//  设置区域
  ILI9341_OpenWindow(x,y,x+1,y+1);
	//   写入数据
  ILI9341_Write_Cmd(0x2C);
	ILI9341_Write_Data(color); 
}

uint16_t  RGB888ToRGB565(uint8_t  r , uint8_t g , uint8_t b)
{
    uint16_t   color = 0;
	  color  |=  (uint16_t)(b>>3)        ;    //  0~4
	  color  |=  (uint16_t)(g>>2) << 5 ;    //  5~10
	  color  |=  (uint16_t)(r>>3) << 11;    // 11~15
	  return color;
}

void LCD_Draw_Picture_Pro(uint16_t *xstr,uint16_t *ystr,uint16_t sendbyte,uint8_t * pic)
{    
    uint8_t temp,t1;
    uint16_t width_byte;
    uint16_t height_total,y_t;
    uint16_t x_t = *xstr;
		uint8_t *pusMsk=pic;//指向指针的指针，为了不改变pic这个指针
    uint16_t width_total = INIT_X + PICTURE_WIDTH;
    uint16_t flag = 0;
		
    width_byte = PICTURE_WIDTH % 8 ? (PICTURE_WIDTH / 8 + 1) : (PICTURE_WIDTH / 8);//宽度是否是8的整数倍，不是对一行的最后一个字节做特殊处理
    height_total = sendbyte % width_byte ? (sendbyte / width_byte + 1) : (sendbyte / width_byte);//计算一共有多少行
    
    
    for (y_t = *ystr; y_t < height_total+*ystr+1;) {
				temp = *pusMsk;
        flag++;
				if (flag>sendbyte) {//判断数据是否已经读取完了
            break;
        }
				
        if (x_t >= width_total - (PICTURE_WIDTH % 8)) {       //如果像素点是8的倍数，或者大于整数倍一
            if (PICTURE_WIDTH % 8) {    //整数倍
                for (t1 = 0; t1 < 4; t1++) {
                    //判断画点
									if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                }
                    temp <<= 1;
                    x_t++;

                x_t = INIT_X;
                if(++y_t>=INIT_Y+PICTURE_HEIGHT){*ystr=INIT_Y;y_t=*ystr;}
								
            
            }
            else
            {
                x_t = INIT_X;
                if(++y_t>=INIT_Y+PICTURE_HEIGHT){*ystr=INIT_Y;y_t=*ystr;}
								
                for (t1 = 0; t1 < 8; t1++) {
                   
                    //判断画点
									if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                    temp <<= 1;
                    x_t++;

                }
            }
        }
        else {
            for (t1 = 0; t1 < 8; t1++) {
                
                //判断画点
								if (temp & 0x80) {
                        LCD_Color_DrawPoint(x_t, y_t, BLACK);
                    }else{
                        LCD_Color_DrawPoint(x_t, y_t, WHITE);
                    }
                    temp <<= 1;
                x_t++;

            }
        
        }
				pusMsk++;
        
    }
    *xstr = x_t;
    *ystr = y_t;
 
}
