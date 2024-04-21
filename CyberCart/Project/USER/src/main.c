#include "headfile.h"

void gpio_init()
{
    P1M0 = 0xff; P1M1 = 0x00; 
    P6M0 = 0xff; P6M1 = 0x00; 
    P1M0 = 0x77; P1M1 = 0x00; // 将LED使用的IO口配置为双向口
    P6M0 = 0xbf; P6M1 = 0x00; // 将LED使用的IO口配置为双向口
}

void motor_drivers_pwm_init()
{

}

void laser_ranging_uart_init()
{
    uart_init(UART_1, UART1_RX_P43, UART1_TX_P44, 9600, TIM_2);
    uart_init(UART_2, UART2_RX_P46, UART2_TX_P47, 9600, TIM_2);
}

void qmc5883_init()
{
    //初始化串口，使用串口3，波特率460800  
    uart_init(UART_3, UART3_RX_P00, UART3_TX_P01, 115200, TIM_3);
}

void main()
{   
    led_1 = 0;
    led_2 = 0;
    led_3 = 0;

	board_init();			        // 初始化寄存器,勿删除此句代码。
    gpio_init();                    // 初始化GPIO
    oled_init_spi();                // 初始化OLED显示屏
    ui_init();                      // 初始化UI
    qmc5883_init();                 // 初始化QMC5883L传感器
    oled_p6x8str_spi(0, 5, "Yaw:");

    motor_drivers_pwm_init();	    // 初始化PWM
    wheel_adjust(X_ALL, 2000);
    wheel_adjust(Y_ALL, 2000);

    // laser_ranging_uart_init();      // 初始化激光测距串口
    // oled_p6x8str_spi(0, 0, "LASER RANGING EXAMPLE");
    // oled_p6x8str_spi(0, 1, "- REALTIME RANGING -");
    // oled_p6x8str_spi(0, 2, "DistX:");
    // oled_p6x8str_spi(0, 3, "DistY:");
    // delay_ms(255);
    // laser_ranging_init();           // 初始化激光测距
    // delay_ms(255);
    // laser_ranging('x', &lrcmd_continous);
    // laser_ranging('y', &lrcmd_continous);
    while(1)
    {
        key1_check();
        key2_check();
        key3_check();
        key4_check();
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