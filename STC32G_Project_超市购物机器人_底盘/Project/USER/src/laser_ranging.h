/*
������ģ��

ͨ�Ų�����������9600bps, 8λ����λ��1λ��ʼλ��1λֹͣλ������żУ��
����������3.0~3.3V 100mA

��������                       | ָ��                      | ˵��                                                   | ����ֵ(�ɹ�/����)
-------------------------------|---------------------------|--------------------------------------------------------|-------------------------
��ȡ�汾��	                    FA 06 00 FF                                                                          FA 06 81 ADDR xx xx xx	CS
��ȡ����	                    FA 06 01 FF                                                                          FA 06 84 ��DAT1 DAT2����DAT16��
��ȡ������	                    FA 06 04 FC                                                                          FA 04 81 81 / FA 84 81 02 FF
���õ�ַ	                    FA 04 01 ADDR CS                                                                     FA 04 8B 77 / FA 84 8B 01 F6
�����޸�	                    FA 04 06 ���� 0xXX CS       ����Ϊ-/+��-Ϊ0x2d��+Ϊ0x2b)��(0��XXΪ����ֵ��һ���ֽ�   
��������ʱ�������ݷ���ʱ����	FA 04 05 MeaInterver CS
���þ�����ֹ��	                FA 04 08 Position CS        ע�ͣ���ʼ���ǰ�����ѡ��
�趨����	                    FA 04 09 range CS           range : 05,10,30,50,80m
�趨Ƶ��                        FA 04 0A Freq CS            Freq : 05 10 20
�趨�ֱ���	                    FA 04 0C Resolution CS      Resolution : 1(1mm),2(0.1mm)
�趨�ϵ缴��	                FA 04 0D Start CS           Start : 0(�ر�),1(����)
���β���                        FA 06 06 FA                 �㲥������ؽ������ģ�黺��
��ȡ����	                    ADDR 06 07 CS
���β�����1mm��	                ADDR 06 02 CS
���β��� (0.1mm)	            ADDR 06 02 CS
����������1mm��	                ADDR 06 03 CS	
����������0.1mm��	            ADDR 06 03 CS
���Ƽ���򿪻�ر�	            ADDR 06 05 LASER CS         LASER : 00 �رգ�01 ����
�ػ�	                        ADDR 04 02 CS

�������ÿ�ֱ��ʹ�õ�ָ�ADDR����Ϊ0x80����
���β���            80 06 02 78
��������            80 06 03 77
�ػ�                80 04 02 7A
���õ�ַ            FA 04 01 80 81
�����޸�-1          FA 04 06 2D 01 CE
�����޸�+1          FA 04 06 2B 01 D0
ʱ����(1S)        FA 04 05 01 FC
������ʼ�� ����     FA 04 08 01 F9
������ʼ�� β��     FA 04 08 00 FA
��������5m          FA 04 09 05 F4
��������10m         FA 04 09 0A EF 10m
��������30m         FA 04 09 1E DB 30m
��������50m         FA 04 09 32 C7 50m
��������80m         FA 04 09 50 A9 80m
����Ƶ��1hz         FA 04 0A 00 F8
����Ƶ��5hz         FA 04 0A 05 F3
����Ƶ��10hz        FA 04 0A 0A EE
����Ƶ��20hz        FA 04 0A 14 E4
�趨�ֱ���1mm       FA 04 0C 01 F5
�趨�ֱ���0.1mm     FA 04 0C 02 F4
�趨�ϵ�Ͳ�ر�    FA 04 0D 00 F5
�趨�ϵ�Ͳ⿪��    FA 04 0D 01 F4
���β������㲥��    FA 06 06 FA
��ȡ����            80 06 07 73
���Ƽ��⿪��        80 06 05 01 74
���Ƽ���ر�        80 06 05 00 75
*/

#ifndef __LASER_RANGING_H
#define __LASER_RANGING_H

#include "headfile.h"
#include "uart.h"

typedef struct
{
    unsigned char *command;
    unsigned char length;
}LASER_RANGING_CMD_DEF;

typedef struct
{
    float valuedata;              // �洢���ֵ
    unsigned char chardata[16];   // �洢���ֵ�ַ�
} LASER_RANGING_DATA;

typedef struct
{
    LASER_RANGING_DATA x;
    LASER_RANGING_DATA y;
} LASER_RANGING_DATA_DEF;

extern float laser_ranging_value;
extern unsigned char laser_ranging_char[16];
extern unsigned char laser_ranging_range;

void laser_ranging_irqhandler(unsigned char lr_axis);
unsigned char laser_ranging_get_uart_value(unsigned char lr_axis);
void laser_ranging(unsigned char lr_axis, LASER_RANGING_CMD_DEF *specific_cmd);
void laser_ranging_init();

#endif