// ���Ŷ���д���ĵ��У�����ʹ��ע�ͱ�ע

#include "headfile.h"
#include "laser_ranging.h"
#include "motor_driver_boards.h"     // ���������ͷ�ļ�
#include "key.h"
// #include "lsm303_sw.h"

void gpio_init()
{
    gpio_mode(P6, GPO_PP);          // ���� P6.0~P6.7 Ϊ�������
    gpio_mode(P3, GPO_PP);          // ���� P3.0~P3.7 Ϊ�������
    gpio_mode(P1, GPO_PP);          // ���� P1.0~P1.7 Ϊ�������
}



void main()
{   
	board_init();			        // ��ʼ���Ĵ���,��ɾ���˾���롣

    gpio_init();                    // ��ʼ��GPIO��ʹ��P4��P6��ȷ���
    oled_init_spi();                // ��ʼ��OLED��ʾ��

    keys_ui_init();                 // ��ʼ������UI

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