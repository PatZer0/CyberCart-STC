#include "key.h"
#include "laser_ranging.h"
#include "qmc5883.h"

unsigned char key1_up_store = 1; // ����1״̬�ݴ�
unsigned char key2_up_store = 1; // ����2״̬�ݴ�
unsigned char key3_up_store = 1; // ����3״̬�ݴ�
unsigned char key4_up_store = 1; // ����4״̬�ݴ�
bit LaserRanging_State = 0; // ������״̬

void keys_ui_init()
{
    // ��ʼ������
    oled_p6x8str_spi(0, 7, "[1][2][3][4]");
}

void key1_short_press()
{
    // �̰���1����
    led_1 = !led_1; // �л�
}

void key2_short_press()
{
    // �̰���2����
}

void key3_short_press()
{
    // �̰���3����
}

void key4_short_press()
{
    // �̰���4����
    LaserRanging_State = !LaserRanging_State;
    if(LaserRanging_State)  // �򿪼�����
    {
        oled_p6x8str_spi(6*18, 0, " ON");
        laser_ranging('x', &lrcmd_continous);                                                                        
        laser_ranging('y', &lrcmd_continous);
    }
    else                    // �رռ�����
    {
        oled_p6x8str_spi(6*18, 0, "OFF");
        laser_ranging('x', &lrcmd_laseroff);
        laser_ranging('y', &lrcmd_laseroff);
    }
}

void key1_check() 
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(0, 7, ">1<");
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // ��ǰ������ͷ�
        key1_short_press(); // �����̰���1����
        oled_p6x8str_spi(0, 7, "[1]");
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(3*6, 7, ">2<");
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // ��ǰ������ͷ�
        key2_short_press(); // �����̰���2����
        oled_p6x8str_spi(3*6, 7, "[2]");
    }
}

void key3_check()
{
    if(key3 == 0 && key3_up_store == 1)
    {
        key3_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(6*6, 7, ">3<");
    }
    if(key3 == 1 && key3_up_store == 0)
    {
        key3_up_store = 1; // ��ǰ������ͷ�
        key3_short_press(); // �����̰���3����
        oled_p6x8str_spi(6*6, 7, "[3]");
    }
}

void key4_check()
{
    if(key4 == 0 && key4_up_store == 1)
    {
        key4_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(9*6, 7, ">4<");
    }
    if(key4 == 1 && key4_up_store == 0)
    {
        key4_up_store = 1; // ��ǰ������ͷ�
        key4_short_press(); // �����̰���4����
        oled_p6x8str_spi(9*6, 7, "[4]");
    }
}