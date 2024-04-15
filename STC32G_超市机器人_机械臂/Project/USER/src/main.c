// 引脚定义写在文档中，不再使用注释备注

#include "headfile.h"
#include "laser_ranging.h"
#include "motor_driver_boards.h"     // 电机驱动板头文件
#include "key.h"
// #include "lsm303_sw.h"

void gpio_init()
{
    gpio_mode(P6, GPO_PP);          // 配置 P6.0~P6.7 为推挽输出
    gpio_mode(P3, GPO_PP);          // 配置 P3.0~P3.7 为推挽输出
    gpio_mode(P1, GPO_PP);          // 配置 P1.0~P1.7 为推挽输出
}



void main()
{   
	board_init();			        // 初始化寄存器,勿删除此句代码。

    gpio_init();                    // 初始化GPIO，使得P4和P6正确输出
    oled_init_spi();                // 初始化OLED显示屏

    keys_ui_init();                 // 初始化按键UI

    oled_p8x16str_spi(0, 0, "QMC5883L");
    oled_p6x8str_spi(0, 2, "MagX:");
    oled_p6x8str_spi(0, 3, "MagY:");
    oled_p6x8str_spi(0, 4, "MagZ:");
    oled_p6x8str_spi(0, 5, "Yaw:");


    while(1)
    {
        // oled_printf_int32_spi(5*6, 2, qmc5883_magx, 4);
        // oled_printf_int32_spi(5*6, 3, qmc5883_magy, 4);
        // oled_printf_int32_spi(5*6, 4, qmc5883_magz, 4);
        // oled_printf_float_spi(4*6, 5, qmc5883_yaw, 3, 2);
        oled_p6x8str_spi(5*6, 2, qmc5883_char_magx);
        oled_p6x8str_spi(5*6, 3, qmc5883_char_magy);
        oled_p6x8str_spi(5*6, 4, qmc5883_char_magz);
        oled_p6x8str_spi(4*6, 5, qmc5883_char_yaw);
        delay_ms(100);
    }
}


/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/