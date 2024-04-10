/*
激光测距模块

通信参数：波特率9600bps, 8位数据位，1位起始位，1位停止位，无奇偶校验
电气参数：3.0~3.3V 100mA

功能描述                       | 指令                      | 说明                                                   | 返回值(成功/错误)
-------------------------------|---------------------------|--------------------------------------------------------|-------------------------
读取版本号	                    FA 06 00 FF                                                                          FA 06 81 ADDR xx xx xx	CS
读取参数	                    FA 06 01 FF                                                                          FA 06 84 “DAT1 DAT2……DAT16”
读取机器号	                    FA 06 04 FC                                                                          FA 04 81 81 / FA 84 81 02 FF
设置地址	                    FA 04 01 ADDR CS                                                                     FA 04 8B 77 / FA 84 8B 01 F6
距离修改	                    FA 04 06 符号 0xXX CS       符号为-/+，-为0x2d，+为0x2b)，(0下XX为修正值，一个字节   
连续测量时设置数据返回时间间隔	FA 04 05 MeaInterver CS
设置距离起止点	                FA 04 08 Position CS        注释：起始点可前、后端选择
设定量程	                    FA 04 09 range CS           range : 05,10,30,50,80m
设定频率                        FA 04 0A Freq CS            Freq : 05 10 20
设定分辨率	                    FA 04 0C Resolution CS      Resolution : 1(1mm),2(0.1mm)
设定上电即测	                FA 04 0D Start CS           Start : 0(关闭),1(开启)
单次测量                        FA 06 06 FA                 广播命令，返回结果存入模块缓存
读取缓存	                    ADDR 06 07 CS
单次测量（1mm）	                ADDR 06 02 CS
单次测量 (0.1mm)	            ADDR 06 02 CS
连续测量（1mm）	                ADDR 06 03 CS	
连续测量（0.1mm）	            ADDR 06 03 CS
控制激光打开或关闭	            ADDR 06 05 LASER CS         LASER : 00 关闭，01 开启
关机	                        ADDR 04 02 CS

出厂设置可直接使用的指令（ADDR出厂为0x80）：
单次测量            80 06 02 78
连续测量            80 06 03 77
关机                80 04 02 7A
设置地址            FA 04 01 80 81
距离修改-1          FA 04 06 2D 01 CE
距离修改+1          FA 04 06 2B 01 D0
时间间隔(1S)        FA 04 05 01 FC
设置起始点 顶端     FA 04 08 01 F9
设置起始点 尾端     FA 04 08 00 FA
设置量程5m          FA 04 09 05 F4
设置量程10m         FA 04 09 0A EF 10m
设置量程30m         FA 04 09 1E DB 30m
设置量程50m         FA 04 09 32 C7 50m
设置量程80m         FA 04 09 50 A9 80m
设置频率1hz         FA 04 0A 00 F8
设置频率5hz         FA 04 0A 05 F3
设置频率10hz        FA 04 0A 0A EE
设置频率20hz        FA 04 0A 14 E4
设定分辨率1mm       FA 04 0C 01 F5
设定分辨率0.1mm     FA 04 0C 02 F4
设定上电就测关闭    FA 04 0D 00 F5
设定上电就测开启    FA 04 0D 01 F4
单次测量（广播）    FA 06 06 FA
读取缓存            80 06 07 73
控制激光开启        80 06 05 01 74
控制激光关闭        80 06 05 00 75
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
    float valuedata;              // 存储测距值
    unsigned char chardata[16];   // 存储测距值字符
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