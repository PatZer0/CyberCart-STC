#include "headfile.h"
#include "ui.h"
#include "uart.h"
#include "key.h"
#include "qmc5883.h"
#include "laser_ranging.h"
#include "host_comm.h"
#include "motor_driver_boards.h"

void gpio_init()
{
    P1M0 = 0xff; P1M1 = 0x00; 
    P6M0 = 0xff; P6M1 = 0x00; 
    P1M0 = 0x77; P1M1 = 0x00; // 将LED使用的IO口配置为双向口
    P6M0 = 0xbf; P6M1 = 0x00; // 将LED使用的IO口配置为双向口
}

void motor_drivers_pwm_init()
{
    // 参数：模块_端口, 频率, 占空比(÷10000)
    pwm_init(PWMA_CH1P_P60, 30000, 0);   // X方向驱动板PB, 30KHz, 10%占空比
    pwm_init(PWMA_CH2N_P63, 30000, 0);   // X方向驱动板PA，30KHz, 10%占空比
    pwm_init(PWMA_CH4N_P67, 30000, 0);   // Y方向驱动板PB, 30KHz, 10%占空比
    pwm_init(PWMA_CH3P_P14, 30000, 0);   // Y方向驱动板PA, 30KHz, 10%占空比
    wheel_adjust(X_ALL, 0);
    wheel_adjust(Y_ALL, 0);
}

void laser_ranging_uart_init()
{
    // uart_init(UART_2, UART1_RX_P43, UART1_TX_P44, 9600, TIM_2);
    // uart_init(UART_4, UART2_RX_P46, UART2_TX_P47, 9600, TIM_2);
}

void qmc5883_init()
{
    //初始化串口，使用串口3，波特率460800
    uart_init(UART_3, UART3_RX_P50, UART3_TX_P51, 115200, TIM_2);
    delay_ms(100);
    uart_sendstring(3, "AT+PRATE=10\r\n");
}

void servo_chassis_pwm_init()
{
    // 初始化旋转底盘舵机PWM接口
    pwm_init(PWMB_CH1_P00, 5000, 0);
}

void servo_mech_arm_pwm_init()
{
    // 初始化机械臂舵机PWM接口
    pwm_init(PWMB_CH2_P01, 5000, 0);
}

void stepper_pwm_init()
{
    // 初始化步进电机PWM接口
    // P3.2 <-> DIR
    // P3.3 <-> STP (PWM) 频率:0-20kHz
    // P3.5 <-> EN        1:使能步进电机；0:步进电机脱机
    pwm_init(PWMB_CH3_P02, 10000, 0);
}

void main()
{   
    led_1 = 0;
    led_2 = 0;
    led_3 = 0;

	board_init();			        // 初始化寄存器,勿删除此句代码。
    motor_drivers_pwm_init();	    // 初始化PWM
    gpio_init();                    // 初始化GPIO
    oled_init_spi();                // 初始化OLED显示屏
    ui_init();                      // 初始化UI
    qmc5883_init();                 // 初始化QMC5883L传感器
    oled_p6x8str_spi(0, 5, "Yaw:");



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
    
    // 步进电机测试代码
    P32 = 1;
    P35 = 1;
    pwm_init(PWMB_CH3_P33, 5000, 0);
    gpio_mode(P3_5, GPO_PP);
    gpio_mode(P3_2, GPO_PP);

    host_comm_uart_init();

    while(1)
    {
        key1_check();
        key2_check();
        key3_check();
        key4_check();
        // ui_running();
        host_comm_sender();
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