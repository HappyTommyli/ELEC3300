// #include "image.h"
// #include <stdio.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
// #include "bsp_ili9341_lcd.h"
// #include "ff.h" 

// #define INIT_X 0	//起始x坐标
// #define INIT_Y 0	//起始y坐标
// #define	PICTURE_WIDTH 240
// #define PICTURE_HEIGHT	320
// #define TOTAL_BYTE	9600	//一张图片存储的总字节数，一个字节中有8个像素
// #define BUFF_TOTAL_BYTE 512
// #define LCD_WIDTH 240
// #define LCD_HEIGHT 320
// //一张图片存储的总位数，即总像素，以300*230的图片为例，像素总个数为：300*230=69000
// //但是为什么软件输出的图像8740*8=69920
// //原因：图像的宽度300，即300个像素点，8个像素点对应一个字节，300\8=37余4，余下的4个像素被装入了一个字节的前四位中，后四位补0
// //因此：一行相当于38个字节一共230行，总字节数：38*230=8740，多出来了：230*4=920补0像素
// #define TOTAL_BIT		TOTAL_BYTE*8







// /****************************************************************************
// * 名    称: void LCD_Draw_Picture_Pro(uint16_t *xstr,uint16_t *ystr,uint16_t sendbyte,uint8_t * pic)
// * 功    能：根据传入的字节数据画图片
// * 入口参数：xstr,ystr:起始坐标，该坐标会一直变化，因为一次传入的数据有限
//             sendbyte:传入的字节大小
// 						pic:数据的指针，指向buff缓冲数组的第一个元素的地址
// * 返回参数：无
// * 说    明：该字模取模方向为先从左到右，再从上到下  低位在前  
// ****************************************************************************/
// // void LCD_Draw_Picture_Pro(uint16_t *xstr,uint16_t *ystr,uint16_t sendbyte,uint8_t * pic)
// // {

    
// //     uint8_t temp,t1;
// //     uint16_t width_byte;
// //     uint16_t height_total,y_t;
// //     uint16_t x_t = *xstr;
// // 	uint8_t *pusMsk=pic;
// //     uint16_t width_total = INIT_X + PICTURE_WIDTH;
// //     uint16_t flag = 0;
		
// //     width_byte = PICTURE_WIDTH % 8 ? (PICTURE_WIDTH / 8 + 1) : (PICTURE_WIDTH / 8);//宽度是否是8的整数倍，不是对一行的最后一个字节做特殊处理
// //     height_total = sendbyte % width_byte ? (sendbyte / width_byte + 1) : (sendbyte / width_byte);//计算一共有多少行
    
    
// //     for (y_t = *ystr; y_t < height_total+*ystr+1;) {
// // 				temp = *pusMsk;
// //         flag++;
// // 				if (flag>sendbyte) {
// //             break;
// //         }
				
// //         if (x_t >= width_total - (PICTURE_WIDTH % 8)) {
// //             if (PICTURE_WIDTH % 8) {
// //                 for (t1 = 0; t1 < 4; t1++) {
// //                     //判断画点
// // 									if (temp & 0x80) {
// //                         LCD_Color_DrawPoint(x_t, y_t, BLACK);
// //                     }else{
// //                         LCD_Color_DrawPoint(x_t, y_t, WHITE);
// //                     }
// //                     temp <<= 1;
// //                     x_t++;
// //                 }
// //                 x_t = INIT_X;
// //                 if(++y_t>=INIT_Y+PICTURE_HEIGHT){*ystr=INIT_Y;y_t=*ystr;}
								
            
// //             }
// //             else
// //             {
// //                 x_t = INIT_X;
// //                 if(++y_t>=INIT_Y+PICTURE_HEIGHT){*ystr=INIT_Y;y_t=*ystr;}
								
// //                 for (t1 = 0; t1 < 8; t1++) {
                   
// //                     //判断画点
// // 									if (temp & 0x80) {
// //                         LCD_Color_DrawPoint(x_t, y_t, BLACK);
// //                     }else{
// //                         LCD_Color_DrawPoint(x_t, y_t, WHITE);
// //                     }
// //                     temp <<= 1;
// //                     x_t++;

// //                 }
// //             }
// //         }
// //         else {
// //             for (t1 = 0; t1 < 8; t1++) {
                
// //                 //判断画点
// // 								if (temp & 0x80) {
// //                         LCD_Color_DrawPoint(x_t, y_t, BLACK);
// //                     }else{
// //                         LCD_Color_DrawPoint(x_t, y_t, WHITE);
// //                     }
// //                     temp <<= 1;
// //                 x_t++;

// //             }
        
// //         }
// // 				pusMsk++;
        
// //     }
// //     *xstr = x_t;
// //     *ystr = y_t;
 
// // }

// void LCD_open_directory1(uint16_t *xstr, uint16_t *ystr, char filepath[]){
//     DIR dir; // 目录对象
//     FILINFO fno; // 文件信息对象
//     FRESULT res; // 文件系统结果
//     res = f_opendir(&dir, "D:\\ELEC3300 PROJECT\\Final_project\\batch");
//     if (res == FR_OK)
//     {
//         while (1)
//         {
//             res = f_readdir(&dir, &fno); // 读取目录项
//             if (res != FR_OK || fno.fname[0] == 0) break; // 如果读取失败或到达目录末尾，则退出循环
//             sprintf(filepath, "D:\\ELEC3300 PROJECT\\Final_project\\batch\\%s", fno.fname); // 拼接文件路径
            
//             LoadAndDisplayCFile1();


//         }
//         f_closedir(&dir); // 关闭目录
//     }
//     // else
//     // {
//     //     printf("Failed to open directory\n"); // 打印错误信息
//     // }
    
// }

// void LoadAndDisplayCFile1(uint16_t *xstr,uint16_t *ystr,char filepath[]) {
//     FIL file;
//     char line[256];
//     int pixelCount = 0;

//     if (f_open(&file, filepath, FA_READ) == FR_OK) {
//         while (f_gets(line, sizeof(line), &file) != NULL) {
//             // 简单解析-实际项目需要更健壮的解析器
//             char* p = line;
//             while (*p) {
//                 while (*p && (*p == ' ' || *p == ',' || *p == '\t')) p++;
            
//             // 检测 "0x" 前缀
//                 if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
//                     p += 2;  // 跳过 "0x"
//                     if (isxdigit((unsigned char)p[0]) && isxdigit((unsigned char)p[1])) {

//                         // 提取2字节十六进制值
//                         char *end;
//                         pixels[pixelCount++] = strtol(p, &end, 16);
//                         if (&end > &p) { // 成功解析到数字
//                             p = end;       // 手动更新指针
//                         }
//                     }
//                 } else {
//                     p++;
//                 }
//                 if (pixelCount >= 320*240) break;
//             }
//         }
//         f_close(&file);
//         int drawcount = 0;
//         uint16_t x_t = *xstr;
//         uint16_t y_t = *ystr;

//         // 显示图片
//         for(int i = 0; i < 320; i++){
//             for(int j = 0; j<240; j++){
//                 LCD_Color_DrawPoint(x_t, y_t, pixels[drawcount++]);
//                 x_t++;
//             }
//             x_t = INIT_X;
//             y_t++;
//         }
//     }
// }
