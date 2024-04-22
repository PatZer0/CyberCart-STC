#include "headfile.h"
#include "ui.h"

unsigned char ui_loading_index = 0;

void ui_init()
{
    oled_p6x8str_spi(0, 0, "CYBERCART");
    oled_p6x8str_spi(0, 1, "DEBUG");
    oled_p6x8str_spi(6*18, 0, "VER");
    oled_p6x8str_spi(6*18, 1, "1.0");
    
    ui_keys_init();                 // 初始化按键UI
}

void ui_keys_init()
{
    // 初始化按键UI
    oled_p6x8str_spi(0, 7, "[1][2][3][4]");
}

void ui_running_init_timer()
{                           //500毫秒@33.1776MHz
	TM0PS = 0x15;			//设置定时器时钟预分频 ( 注意:并非所有系列都有此寄存器,详情请查看数据手册 )
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x8C;				//设置定时初始值
	TH0 = 0x0A;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}


void ui_running_timer_isr() interrupt 1
{
    ui_running();
}

void ui_running()
{
    ui_loading_index++;
    if(ui_loading_index > 3) ui_loading_index = 0;
    switch (ui_loading_index)
    {
    case 0:
        oled_p6x8str_spi(20*6, 7, '1');
        break;
    case 1:
        oled_p6x8str_spi(20*6, 7, '2');
        break;
    case 2:
        oled_p6x8str_spi(20*6, 7, '3');
        break;
    case 3:
        oled_p6x8str_spi(20*6, 7, '4');
        break;
    default:
        break;
    }
}