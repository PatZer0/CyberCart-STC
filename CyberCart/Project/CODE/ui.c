#include "headfile.h"
#include "ui.h"

unsigned char ui_loading_index = 0;

void ui_init()
{
    oled_p6x8str_spi(0, 0, "CYBERCART");
    oled_p6x8str_spi(0, 1, "DEBUG");
    oled_p6x8str_spi(6*18, 0, "VER");
    oled_p6x8str_spi(6*18, 1, "1.0");
    
    ui_keys_init();                 // ��ʼ������UI
}

void ui_keys_init()
{
    // ��ʼ������UI
    oled_p6x8str_spi(0, 7, "[1][2][3][4]");
}

void ui_running_init_timer()
{                           //500����@33.1776MHz
	TM0PS = 0x15;			//���ö�ʱ��ʱ��Ԥ��Ƶ ( ע��:��������ϵ�ж��д˼Ĵ���,������鿴�����ֲ� )
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0x8C;				//���ö�ʱ��ʼֵ
	TH0 = 0x0A;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;				//ʹ�ܶ�ʱ��0�ж�
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