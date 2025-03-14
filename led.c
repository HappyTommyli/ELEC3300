#include "led.h"




 

static void a1()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}
static void b1()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
}
static void c1()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
}
static void d1()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}
static void e1()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
}
static void f1()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
}
static void g1()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
}


static void a()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}
static void b()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
}
static void c()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
}
static void d()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}
static void e()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
}
static void f()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
}
static void g()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
}

void LEC_close(){
        a1();b1();c1();d1();e1();f1();g1();




}
void LED_Printnum(int8_t num)

 {

    switch (num)
    {
    case 0:
        a();
        b();
        c();
        d();
        e();
        f();
        break;
    case 1:
       
        b();
        c();
       
       
        break;
    case 2:
        a();
        b();
        g();
        d();
        e();
      
        break;
    case 3:
        a();
        b();
        c();
        d();
        g();
        break;
    case 4:
       
        b();
        c();
        g();
        f();
        break;
    case 5:
        a();
        g();
        c();
        d();
        f();
        break;
    case 6:
        a();
        g();
        c();
        d();
        e();
        f();
        break;
    case 7:
        a();
        b();
        c();
       
        break;
    case 8:
        a();
        b();
        c();
        d();
        g();
        e();
        f();
        break;
    case 9:
        a();
        b();
        c();
        d();
        g();
        f();
        break;

    default:
        break;
    }
 }

