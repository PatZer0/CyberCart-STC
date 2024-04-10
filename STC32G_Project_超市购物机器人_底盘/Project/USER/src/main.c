// 引脚定义写在文档中，不再使用注释备注

#include "headfile.h"
#include "laser_ranging.h"
#include "motor_driver_boards.h"     // 电机驱动板头文件
// #include "lsm303_sw.h"

void gpio_init()
{
    gpio_mode(P6, GPO_PP);          // 配置 P6.0~P6.7 为推挽输出
    gpio_mode(P1, GPO_PP);          // 配置 P1.0~P1.7 为推挽输出
}

void motor_drivers_pwm_init()
{
    // 参数：模块_端口, 频率, 占空比(÷10000)
    pwm_init(PWMA_CH1P_P60, 30000, 1000);   // X方向驱动板PB, 3KHz, 10%占空比
    pwm_init(PWMA_CH2N_P63, 30000, 1000);   // X方向驱动板PA，3KHz, 10%占空比
    pwm_init(PWMA_CH3P_P14, 30000, 1000);   // Y方向驱动板PB, 3KHz, 10%占空比
    pwm_init(PWMA_CH4N_P67, 30000, 1000);   // Y方向驱动板PA, 3KHz, 10%占空比
}

void lsm303_iic_init(void)
{
    float acc_x, acc_y, acc_z;
    float mag_x, mag_y, mag_z;
    iic_init(IIC_4, IIC4_SCL_P32, IIC4_SDA_P33, 19);
    // 需要注意SEEKFREE LIBRARY默认提供的项目文件没有导入iic相关c文件
    // 需要手动在Keil中添加
}

void laser_ranging_uart_init()
{
    uart_init(UART_1, UART1_RX_P43, UART1_TX_P44, 9600, TIM_2);
    uart_init(UART_2, UART2_RX_P46, UART2_TX_P47, 9600, TIM_2);
}

void main()
{   
	board_init();			        // 初始化寄存器,勿删除此句代码。
    gpio_init();                    // 初始化GPIO，使得P4和P6正确输出
    oled_init_spi();                // 初始化OLED显示屏
    
    // lsm303_iic_init();              // 初始化LSM303_IIC接口
    // lsm303_init_all();              // 初始化LSM303传感器
    uart_port_init();               // 初始化UART端口
    uart_initialize(1);             // 初始化UART1
    uart_initialize(2);             // 初始化UART2
    motor_drivers_pwm_init();	    // 初始化PWM
    while(1)
    {
        // oled_p6x8str_spi(0, 0, "LSM303 DATA READ TEST");
        // oled_p6x8str_spi(0, 1, "ACC X:");
        // oled_p6x8str_spi(0, 2, "ACC Y:");
        // oled_p6x8str_spi(0, 3, "ACC Z:");
        // acc_x = lsm303_read_acc('x');
        // acc_y = lsm303_read_acc('y');
        // acc_z = lsm303_read_acc('z');
        // oled_printf_float_spi(6*7, 1, acc_x, 4, 6);
        // oled_printf_float_spi(6*7, 2, acc_y, 4, 6);
        // oled_printf_float_spi(6*7, 3, acc_z, 4, 6);
        // mag_x = lsm303_read_mag('x');
        // mag_y = lsm303_read_mag('y');
        // mag_z = lsm303_read_mag('z');
        // oled_p6x8str_spi(0, 5, "MAG X:");
        // oled_p6x8str_spi(0, 6, "MAG Y:");
        // oled_p6x8str_spi(0, 7, "MAG Z:");
        // oled_printf_float_spi(6*7, 5, mag_x, 4, 6);
        // oled_printf_float_spi(6*7, 6, mag_y, 4, 6);
        // oled_printf_float_spi(6*7, 7, mag_z, 4, 6);
        // delay_ms(100);
        // wheels_x_forward();
        // wheels_y_forward();
        // delay_ms(255);
        // pwm_duty(PWMA_CH1P_P60, 2000);
        // pwm_duty(PWMA_CH2P_P62, 2000);
        // pwm_duty(PWMA_CH3P_P64, 2000);
        // pwm_duty(PWMA_CH4P_P66, 2000);
        // delay_ms(255);
        // wheels_x_backward();
        // wheels_y_backward();
        // delay_ms(255);
        // pwm_duty(PWMA_CH1P_P60, 5000);
        // pwm_duty(PWMA_CH2P_P62, 5000);
        // pwm_duty(PWMA_CH3P_P64, 5000);
        // pwm_duty(PWMA_CH4P_P66, 5000);
        // delay_ms(255);
        // wheels_all_stop();
        // delay_ms(255);
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