#include "headfile.h"
#include "hal.h"
#include "ui.h"
#include "key.h"

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
    led_4 = 0;

	board_init();			        // ��ʼ���Ĵ���,��ɾ���˾���롣
    motor_drivers_pwm_init();	    // ��ʼ��PWM
    gpio_init();                    // ��ʼ��GPIO
    oled_init_spi();                // ��ʼ��OLED��ʾ��
    wheel_oled_init();              // ��ʼ���ٶ���ʾOLED��ʾ��
    ui_init();                      // ��ʼ��UI
    delay_ms(100);
    laser_ranging_init();           // ��ʼ�����ģ��
    host_comm_uart_init();          // ��ʼ������ͨ��UART

    servo_init();                   // ��ʼ�����
    
    while(1)
    {
        key1_check();
        key2_check();
        key3_check();
        key4_check();
        key5_check();
        host_comm_sender();
        wheel_dynamic_adjusting();
        uart_tx_send_buffer();
        wheel_oled_update();
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