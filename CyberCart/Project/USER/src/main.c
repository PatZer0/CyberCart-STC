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
    P1M0 = 0x77; P1M1 = 0x00; // ��LEDʹ�õ�IO������Ϊ˫���
    P6M0 = 0xbf; P6M1 = 0x00; // ��LEDʹ�õ�IO������Ϊ˫���
}

void motor_drivers_pwm_init()
{
    // ������ģ��_�˿�, Ƶ��, ռ�ձ�(��10000)
    pwm_init(PWMA_CH1P_P60, 30000, 0);   // X����������PB, 30KHz, 10%ռ�ձ�
    pwm_init(PWMA_CH2N_P63, 30000, 0);   // X����������PA��30KHz, 10%ռ�ձ�
    pwm_init(PWMA_CH4N_P67, 30000, 0);   // Y����������PB, 30KHz, 10%ռ�ձ�
    pwm_init(PWMA_CH3P_P14, 30000, 0);   // Y����������PA, 30KHz, 10%ռ�ձ�
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
    //��ʼ�����ڣ�ʹ�ô���3��������460800
    uart_init(UART_3, UART3_RX_P50, UART3_TX_P51, 115200, TIM_2);
    delay_ms(100);
    uart_sendstring(3, "AT+PRATE=10\r\n");
}

void servo_chassis_pwm_init()
{
    // ��ʼ����ת���̶��PWM�ӿ�
    pwm_init(PWMB_CH1_P00, 5000, 0);
}

void servo_mech_arm_pwm_init()
{
    // ��ʼ����е�۶��PWM�ӿ�
    pwm_init(PWMB_CH2_P01, 5000, 0);
}

void stepper_pwm_init()
{
    // ��ʼ���������PWM�ӿ�
    // P3.2 <-> DIR
    // P3.3 <-> STP (PWM) Ƶ��:0-20kHz
    // P3.5 <-> EN        1:ʹ�ܲ��������0:��������ѻ�
    pwm_init(PWMB_CH3_P02, 10000, 0);
}

void main()
{   
    led_1 = 0;
    led_2 = 0;
    led_3 = 0;

	board_init();			        // ��ʼ���Ĵ���,��ɾ���˾���롣
    motor_drivers_pwm_init();	    // ��ʼ��PWM
    gpio_init();                    // ��ʼ��GPIO
    oled_init_spi();                // ��ʼ��OLED��ʾ��
    ui_init();                      // ��ʼ��UI
    qmc5883_init();                 // ��ʼ��QMC5883L������
    oled_p6x8str_spi(0, 5, "Yaw:");



    // laser_ranging_uart_init();      // ��ʼ�������മ��
    // oled_p6x8str_spi(0, 0, "LASER RANGING EXAMPLE");
    // oled_p6x8str_spi(0, 1, "- REALTIME RANGING -");
    // oled_p6x8str_spi(0, 2, "DistX:");
    // oled_p6x8str_spi(0, 3, "DistY:");
    // delay_ms(255);
    // laser_ranging_init();           // ��ʼ��������
    // delay_ms(255);
    // laser_ranging('x', &lrcmd_continous);
    // laser_ranging('y', &lrcmd_continous);
    
    // ����������Դ���
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
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/