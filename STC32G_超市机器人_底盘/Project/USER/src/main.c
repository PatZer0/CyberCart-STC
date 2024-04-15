// ���Ŷ���д���ĵ��У�����ʹ��ע�ͱ�ע

#include "headfile.h"
#include "laser_ranging.h"
#include "motor_driver_boards.h"     // ���������ͷ�ļ�
#include "key.h"
#include "qmc5883.h"


void gpio_init()
{
    gpio_mode(P6, GPO_PP);          // ���� P6.0~P6.7 Ϊ�������
    gpio_mode(P1, GPO_PP);          // ���� P1.0~P1.7 Ϊ�������
}

void motor_drivers_pwm_init()
{
    // ������ģ��_�˿�, Ƶ��, ռ�ձ�(��10000)
    pwm_init(PWMA_CH1P_P60, 30000, 1000);   // X����������PB, 3KHz, 10%ռ�ձ�
    pwm_init(PWMA_CH2N_P63, 30000, 1000);   // X����������PA��3KHz, 10%ռ�ձ�
    pwm_init(PWMA_CH3P_P14, 30000, 1000);   // Y����������PB, 3KHz, 10%ռ�ձ�
    pwm_init(PWMA_CH4N_P67, 30000, 1000);   // Y����������PA, 3KHz, 10%ռ�ձ�
}

void lsm303_iic_init(void)
{
    // float acc_x, acc_y, acc_z;
    // float mag_x, mag_y, mag_z;
    iic_init(IIC_4, IIC4_SCL_P32, IIC4_SDA_P33, 19);
    // ��Ҫע��SEEKFREE LIBRARYĬ���ṩ����Ŀ�ļ�û�е���iic���c�ļ�
    // ��Ҫ�ֶ���Keil�����
}

void laser_ranging_uart_init()
{
    uart_init(UART_1, UART1_RX_P43, UART1_TX_P44, 9600, TIM_2);
    uart_init(UART_2, UART2_RX_P46, UART2_TX_P47, 9600, TIM_2);
}

void qmc5883_init()
{
    // ��ʼ�����ڣ�ʹ�ô���3��������460800
    uart_init(UART_3, UART3_RX_P50, UART3_TX_P51, 460800, TIM_3);
}

void main()
{   
	board_init();			        // ��ʼ���Ĵ���,��ɾ���˾���롣
    gpio_init();                    // ��ʼ��GPIO��ʹ��P4��P6��ȷ���
    oled_init_spi();                // ��ʼ��OLED��ʾ��
    keys_ui_init();                 // ��ʼ������UI
    oled_p6x8str_spi(0, 0, "LASER RANGING");
    oled_p6x8str_spi(0, 1, "DISX:");
    oled_p6x8str_spi(0, 2, "DISY:");
    // lsm303_iic_init();              // ��ʼ��LSM303_IIC�ӿ�
    // lsm303_init_all();              // ��ʼ��LSM303������
    // uart_port_init();               // ��ʼ��UART�˿�
    // uart_initialize(1);             // ��ʼ��UART1
    // uart_initialize(2);             // ��ʼ��UART2
    motor_drivers_pwm_init();	    // ��ʼ��PWM
    laser_ranging_uart_init();      // ��ʼ�������മ��
    laser_ranging_init();           // ��ʼ��������
    
    qmc5883_init();                 // ��ʼ��QMC5883L������

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