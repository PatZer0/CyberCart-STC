#include "key.h"
#include "laser_ranging.h"
#include "qmc5883.h"

unsigned char key1_up_store = 1; // 按键1状态暂存
unsigned char key2_up_store = 1; // 按键2状态暂存
unsigned char key3_up_store = 1; // 按键3状态暂存
unsigned char key4_up_store = 1; // 按键4状态暂存
bit LaserRanging_State = 0; // 激光测距状态

void keys_ui_init()
{
    // 初始化按键
    oled_p6x8str_spi(0, 7, "[1][2][3][4]");
}

void key1_short_press()
{
    // 短按键1功能
    led_1 = !led_1; // 切换
}

void key2_short_press()
{
    // 短按键2功能
}

void key3_short_press()
{
    // 短按键3功能
}

void key4_short_press()
{
    // 短按键4功能
    LaserRanging_State = !LaserRanging_State;
    if(LaserRanging_State)  // 打开激光测距
    {
        oled_p6x8str_spi(6*18, 0, " ON");
        laser_ranging('x', &lrcmd_continous);                                                                        
        laser_ranging('y', &lrcmd_continous);
    }
    else                    // 关闭激光测距
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
        key1_up_store = 0; // 标记按键被按下
        oled_p6x8str_spi(0, 7, ">1<");
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // 标记按键被释放
        key1_short_press(); // 触发短按键1功能
        oled_p6x8str_spi(0, 7, "[1]");
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // 标记按键被按下
        oled_p6x8str_spi(3*6, 7, ">2<");
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // 标记按键被释放
        key2_short_press(); // 触发短按键2功能
        oled_p6x8str_spi(3*6, 7, "[2]");
    }
}

void key3_check()
{
    if(key3 == 0 && key3_up_store == 1)
    {
        key3_up_store = 0; // 标记按键被按下
        oled_p6x8str_spi(6*6, 7, ">3<");
    }
    if(key3 == 1 && key3_up_store == 0)
    {
        key3_up_store = 1; // 标记按键被释放
        key3_short_press(); // 触发短按键3功能
        oled_p6x8str_spi(6*6, 7, "[3]");
    }
}

void key4_check()
{
    if(key4 == 0 && key4_up_store == 1)
    {
        key4_up_store = 0; // 标记按键被按下
        oled_p6x8str_spi(9*6, 7, ">4<");
    }
    if(key4 == 1 && key4_up_store == 0)
    {
        key4_up_store = 1; // 标记按键被释放
        key4_short_press(); // 触发短按键4功能
        oled_p6x8str_spi(9*6, 7, "[4]");
    }
}