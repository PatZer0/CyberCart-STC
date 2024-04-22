#ifndef __KEY_H__
#define __KEY_H__

#include "headfile.h"

// 定义按键引脚
sbit key1 = P3^4;
sbit key2 = P3^5;
sbit key3 = P3^6;
sbit key4 = P3^7;

void keys_ui_init();
void key1_check();
void key2_check();
void key3_check();
void key4_check();

#endif // __KEY_H__