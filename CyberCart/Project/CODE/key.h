#ifndef __KEY_H__
#define __KEY_H__

#include "headfile.h"

sbit key4 = P4^0;
sbit key3 = P4^1;
sbit key2 = P4^2;
sbit key1 = P4^5;

sbit led_1 = P1^7;
sbit led_2 = P1^3;
sbit led_3 = P6^6;

void keys_ui_init();
void key1_check();
void key2_check();
void key3_check();
void key4_check();

#endif // __KEY_H__